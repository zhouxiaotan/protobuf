// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: petar@google.com (Petar Petrov)

#include <Python.h>
#include <frameobject.h>
#include <string>
#include <unordered_map>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/pyext/descriptor.h>
#include <google/protobuf/pyext/descriptor_containers.h>
#include <google/protobuf/pyext/descriptor_pool.h>
#include <google/protobuf/pyext/message.h>
#include <google/protobuf/pyext/message_factory.h>
#include <google/protobuf/pyext/scoped_pyobject_ptr.h>
#include <google/protobuf/stubs/hash.h>

#if PY_MAJOR_VERSION >= 3
  #define PyString_FromStringAndSize PyUnicode_FromStringAndSize
  #define PyString_Check PyUnicode_Check
  #define PyString_InternFromString PyUnicode_InternFromString
  #define PyInt_FromLong PyLong_FromLong
  #define PyInt_FromSize_t PyLong_FromSize_t
  #if PY_VERSION_HEX < 0x03030000
    #error "Python 3.0 - 3.2 are not supported."
  #endif
#define PyString_AsStringAndSize(ob, charpp, sizep)                           \
  (PyUnicode_Check(ob) ? ((*(charpp) = const_cast<char*>(                     \
                               PyUnicode_AsUTF8AndSize(ob, (sizep)))) == NULL \
                              ? -1                                            \
                              : 0)                                            \
                       : PyBytes_AsStringAndSize(ob, (charpp), (sizep)))
#endif

namespace google {
namespace protobuf {
namespace python {

// Store interned descriptors, so that the same C++ descriptor yields the same
// Python object. Objects are not immortal: this map does not own the
// references, and items are deleted when the last reference to the object is
// released.
// This is enough to support the "is" operator on live objects.
// All descriptors are stored here.
std::unordered_map<const void*, PyObject*>* interned_descriptors;

PyObject* PyString_FromCppString(const string& str) {
  return PyString_FromStringAndSize(str.c_str(), str.size());
}

#ifndef PyVarObject_HEAD_INIT
#define PyVarObject_HEAD_INIT(type, size) PyObject_HEAD_INIT(type) size,
#endif
#ifndef Py_TYPE
#define Py_TYPE(ob) (((PyObject*)(ob))->ob_type)
#endif


// Helper functions for descriptor objects.

// A set of templates to retrieve the C++ FileDescriptor of any descriptor.
template<class DescriptorClass>
const FileDescriptor* GetFileDescriptor(const DescriptorClass* descriptor) {
  return descriptor->file();
}
template<>
const FileDescriptor* GetFileDescriptor(const FileDescriptor* descriptor) {
  return descriptor;
}
template<>
const FileDescriptor* GetFileDescriptor(const EnumValueDescriptor* descriptor) {
  return descriptor->type()->file();
}
template<>
const FileDescriptor* GetFileDescriptor(const OneofDescriptor* descriptor) {
  return descriptor->containing_type()->file();
}
template<>
const FileDescriptor* GetFileDescriptor(const MethodDescriptor* descriptor) {
  return descriptor->service()->file();
}

// Converts options into a Python protobuf, and cache the result.
//
// This is a bit tricky because options can contain extension fields defined in
// the same proto file. In this case the options parsed from the serialized_pb
// have unknown fields, and we need to parse them again.
//
// Always returns a new reference.
template<class DescriptorClass>
static PyObject* GetOrBuildOptions(const DescriptorClass *descriptor) {
  // Options are cached in the pool that owns the descriptor.
  // First search in the cache.
  PyDescriptorPool* caching_pool = GetDescriptorPool_FromPool(
      GetFileDescriptor(descriptor)->pool());
  std::unordered_map<const void*, PyObject*>* descriptor_options =
      caching_pool->descriptor_options;
  if (descriptor_options->find(descriptor) != descriptor_options->end()) {
    PyObject *value = (*descriptor_options)[descriptor];
    Py_INCREF(value);
    return value;
  }

  // Similar to the C++ implementation, we return an Options object from the
  // default (generated) factory, so that client code know that they can use
  // extensions from generated files:
  //    d.GetOptions().Extensions[some_pb2.extension]
  //
  // The consequence is that extensions not defined in the default pool won't
  // be available.  If needed, we could add an optional 'message_factory'
  // parameter to the GetOptions() function.
  PyMessageFactory* message_factory =
      GetDefaultDescriptorPool()->py_message_factory;

  // Build the Options object: get its Python class, and make a copy of the C++
  // read-only instance.
  const Message& options(descriptor->options());
  const Descriptor *message_type = options.GetDescriptor();
  CMessageClass* message_class = message_factory::GetOrCreateMessageClass(
      message_factory, message_type);
  if (message_class == NULL) {
    PyErr_Format(PyExc_TypeError, "Could not retrieve class for Options: %s",
                 message_type->full_name().c_str());
    return NULL;
  }
  ScopedPyObjectPtr value(
      PyEval_CallObject(message_class->AsPyObject(), NULL));
  if (value == NULL) {
    return NULL;
  }
  if (!PyObject_TypeCheck(value.get(), CMessage_Type)) {
      PyErr_Format(PyExc_TypeError, "Invalid class for %s: %s",
                   message_type->full_name().c_str(),
                   Py_TYPE(value.get())->tp_name);
      return NULL;
  }
  CMessage* cmsg = reinterpret_cast<CMessage*>(value.get());

  const Reflection* reflection = options.GetReflection();
  const UnknownFieldSet& unknown_fields(reflection->GetUnknownFields(options));
  if (unknown_fields.empty()) {
    cmsg->message->CopyFrom(options);
  } else {
    // Reparse options string!  XXX call cmessage::MergeFromString
    string serialized;
    options.SerializeToString(&serialized);
    io::CodedInputStream input(
        reinterpret_cast<const uint8*>(serialized.c_str()), serialized.size());
    input.SetExtensionRegistry(message_factory->pool->pool,
                               message_factory->message_factory);
    bool success = cmsg->message->MergePartialFromCodedStream(&input);
    if (!success) {
      PyErr_Format(PyExc_ValueError, "Error parsing Options message");
      return NULL;
    }
  }

  // Cache the result.
  Py_INCREF(value.get());
  (*descriptor_options)[descriptor] = value.get();

  return value.release();
}

// Copy the C++ descriptor to a Python message.
// The Python message is an instance of descriptor_pb2.DescriptorProto
// or similar.
template<class DescriptorProtoClass, class DescriptorClass>
static PyObject* CopyToPythonProto(const DescriptorClass *descriptor,
                                   PyObject *target) {
  const Descriptor* self_descriptor =
      DescriptorProtoClass::default_instance().GetDescriptor();
  CMessage* message = reinterpret_cast<CMessage*>(target);
  if (!PyObject_TypeCheck(target, CMessage_Type) ||
      message->message->GetDescriptor() != self_descriptor) {
    PyErr_Format(PyExc_TypeError, "Not a %s message",
                 self_descriptor->full_name().c_str());
    return NULL;
  }
  cmessage::AssureWritable(message);
  DescriptorProtoClass* descriptor_message =
      static_cast<DescriptorProtoClass*>(message->message);
  descriptor->CopyTo(descriptor_message);
  Py_RETURN_NONE;
}

// All Descriptors classes share the same memory layout.
typedef struct PyBaseDescriptor {
  PyObject_HEAD

  // Pointer to the C++ proto2 descriptor.
  // Like all descriptors, it is owned by the global DescriptorPool.
  const void* descriptor;

  // Owned reference to the DescriptorPool, to ensure it is kept alive.
  PyDescriptorPool* pool;
} PyBaseDescriptor;


// FileDescriptor structure "inherits" from the base descriptor.
typedef struct PyFileDescriptor {
  PyBaseDescriptor base;

  // The cached version of serialized pb. Either NULL, or a Bytes string.
  // We own the reference.
  PyObject *serialized_pb;
} PyFileDescriptor;


namespace descriptor {

// Creates or retrieve a Python descriptor of the specified type.
// Objects are interned: the same descriptor will return the same object if it
// was kept alive.
// 'was_created' is an optional pointer to a bool, and is set to true if a new
// object was allocated.
// Always return a new reference.
template<class DescriptorClass>
PyObject* NewInternedDescriptor(PyTypeObject* type,
                                const DescriptorClass* descriptor,
                                bool* was_created) {
  if (was_created) {
    *was_created = false;
  }
  if (descriptor == NULL) {
    PyErr_BadInternalCall();
    return NULL;
  }

  // See if the object is in the map of interned descriptors
  std::unordered_map<const void*, PyObject*>::iterator it =
      interned_descriptors->find(descriptor);
  if (it != interned_descriptors->end()) {
    GOOGLE_DCHECK(Py_TYPE(it->second) == type);
    Py_INCREF(it->second);
    return it->second;
  }
  // Create a new descriptor object
  PyBaseDescriptor* py_descriptor = PyObject_New(
      PyBaseDescriptor, type);
  if (py_descriptor == NULL) {
    return NULL;
  }
  py_descriptor->descriptor = descriptor;

  // and cache it.
  interned_descriptors->insert(
      std::make_pair(descriptor, reinterpret_cast<PyObject*>(py_descriptor)));

  // Ensures that the DescriptorPool stays alive.
  PyDescriptorPool* pool = GetDescriptorPool_FromPool(
      GetFileDescriptor(descriptor)->pool());
  if (pool == NULL) {
    // Don't DECREF, the object is not fully initialized.
    PyObject_Del(py_descriptor);
    return NULL;
  }
  Py_INCREF(pool);
  py_descriptor->pool = pool;

  if (was_created) {
    *was_created = true;
  }
  return reinterpret_cast<PyObject*>(py_descriptor);
}

static void Dealloc(PyBaseDescriptor* self) {
  // Remove from interned dictionary
  interned_descriptors->erase(self->descriptor);
  Py_CLEAR(self->pool);
  Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
}

static PyGetSetDef Getters[] = {
  {NULL}
};

PyTypeObject PyBaseDescriptor_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  FULL_MODULE_NAME ".DescriptorBase",   // tp_name
  sizeof(PyBaseDescriptor),             // tp_basicsize
  0,                                    // tp_itemsize
  (destructor)Dealloc,                  // tp_dealloc
  0,                                    // tp_print
  0,                                    // tp_getattr
  0,                                    // tp_setattr
  0,                                    // tp_compare
  0,                                    // tp_repr
  0,                                    // tp_as_number
  0,                                    // tp_as_sequence
  0,                                    // tp_as_mapping
  0,                                    // tp_hash
  0,                                    // tp_call
  0,                                    // tp_str
  0,                                    // tp_getattro
  0,                                    // tp_setattro
  0,                                    // tp_as_buffer
  Py_TPFLAGS_DEFAULT,                   // tp_flags
  "Descriptors base class",             // tp_doc
  0,                                    // tp_traverse
  0,                                    // tp_clear
  0,                                    // tp_richcompare
  0,                                    // tp_weaklistoffset
  0,                                    // tp_iter
  0,                                    // tp_iternext
  0,                                    // tp_methods
  0,                                    // tp_members
  Getters,                              // tp_getset
};

}  // namespace descriptor

const void* PyDescriptor_AsVoidPtr(PyObject* obj) {
  if (!PyObject_TypeCheck(obj, &descriptor::PyBaseDescriptor_Type)) {
    PyErr_SetString(PyExc_TypeError, "Not a BaseDescriptor");
    return NULL;
  }
  return reinterpret_cast<PyBaseDescriptor*>(obj)->descriptor;
}

namespace message_descriptor {

// Unchecked accessor to the C++ pointer.
static const Descriptor* _GetDescriptor(PyBaseDescriptor* self) {
  return reinterpret_cast<const Descriptor*>(self->descriptor);
}

static PyObject* GetName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->name());
}

static PyObject* GetFullName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->full_name());
}

static PyObject* GetFile(PyBaseDescriptor *self, void *closure) {
  return PyFileDescriptor_FromDescriptor(_GetDescriptor(self)->file());
}

static PyObject* GetConcreteClass(PyBaseDescriptor* self, void *closure) {
  // Retuns the canonical class for the given descriptor.
  // This is the class that was registered with the primary descriptor pool
  // which contains this descriptor.
  // This might not be the one you expect! For example the returned object does
  // not know about extensions defined in a custom pool.
  CMessageClass* concrete_class(message_factory::GetMessageClass(
      GetDescriptorPool_FromPool(
          _GetDescriptor(self)->file()->pool())->py_message_factory,
      _GetDescriptor(self)));
  Py_XINCREF(concrete_class);
  return concrete_class->AsPyObject();
}

static PyObject* GetFieldsByName(PyBaseDescriptor* self, void *closure) {
  return NewMessageFieldsByName(_GetDescriptor(self));
}

static PyObject* GetFieldsByCamelcaseName(PyBaseDescriptor* self,
                                          void *closure) {
  return NewMessageFieldsByCamelcaseName(_GetDescriptor(self));
}

static PyObject* GetFieldsByNumber(PyBaseDescriptor* self, void *closure) {
  return NewMessageFieldsByNumber(_GetDescriptor(self));
}

static PyObject* GetFieldsSeq(PyBaseDescriptor* self, void *closure) {
  return NewMessageFieldsSeq(_GetDescriptor(self));
}

static PyObject* GetNestedTypesByName(PyBaseDescriptor* self, void *closure) {
  return NewMessageNestedTypesByName(_GetDescriptor(self));
}

static PyObject* GetNestedTypesSeq(PyBaseDescriptor* self, void *closure) {
  return NewMessageNestedTypesSeq(_GetDescriptor(self));
}

static PyObject* GetExtensionsByName(PyBaseDescriptor* self, void *closure) {
  return NewMessageExtensionsByName(_GetDescriptor(self));
}

static PyObject* GetExtensions(PyBaseDescriptor* self, void *closure) {
  return NewMessageExtensionsSeq(_GetDescriptor(self));
}

static PyObject* GetEnumsSeq(PyBaseDescriptor* self, void *closure) {
  return NewMessageEnumsSeq(_GetDescriptor(self));
}

static PyObject* GetEnumTypesByName(PyBaseDescriptor* self, void *closure) {
  return NewMessageEnumsByName(_GetDescriptor(self));
}

static PyObject* GetEnumValuesByName(PyBaseDescriptor* self, void *closure) {
  return NewMessageEnumValuesByName(_GetDescriptor(self));
}

static PyObject* GetOneofsByName(PyBaseDescriptor* self, void *closure) {
  return NewMessageOneofsByName(_GetDescriptor(self));
}

static PyObject* GetOneofsSeq(PyBaseDescriptor* self, void *closure) {
  return NewMessageOneofsSeq(_GetDescriptor(self));
}

static PyObject* IsExtendable(PyBaseDescriptor *self, void *closure) {
  if (_GetDescriptor(self)->extension_range_count() > 0) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}

static PyObject* GetExtensionRanges(PyBaseDescriptor *self, void *closure) {
  const Descriptor* descriptor = _GetDescriptor(self);
  PyObject* range_list = PyList_New(descriptor->extension_range_count());

  for (int i = 0; i < descriptor->extension_range_count(); i++) {
    const Descriptor::ExtensionRange* range = descriptor->extension_range(i);
    PyObject* start = PyInt_FromLong(range->start);
    PyObject* end = PyInt_FromLong(range->end);
    PyList_SetItem(range_list, i, PyTuple_Pack(2, start, end));
  }

  return range_list;
}

static PyObject* GetContainingType(PyBaseDescriptor *self, void *closure) {
  const Descriptor* containing_type =
      _GetDescriptor(self)->containing_type();
  if (containing_type) {
    return PyMessageDescriptor_FromDescriptor(containing_type);
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject* GetHasOptions(PyBaseDescriptor *self, void *closure) {
  const MessageOptions& options(_GetDescriptor(self)->options());
  if (&options != &MessageOptions::default_instance()) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}

static PyObject* GetOptions(PyBaseDescriptor *self) {
  return GetOrBuildOptions(_GetDescriptor(self));
}

static PyObject* CopyToProto(PyBaseDescriptor *self, PyObject *target) {
  return CopyToPythonProto<DescriptorProto>(_GetDescriptor(self), target);
}

static PyObject* EnumValueName(PyBaseDescriptor *self, PyObject *args) {
  const char *enum_name;
  int number;
  if (!PyArg_ParseTuple(args, "si", &enum_name, &number))
    return NULL;
  const EnumDescriptor *enum_type =
      _GetDescriptor(self)->FindEnumTypeByName(enum_name);
  if (enum_type == NULL) {
    PyErr_SetString(PyExc_KeyError, enum_name);
    return NULL;
  }
  const EnumValueDescriptor *enum_value =
      enum_type->FindValueByNumber(number);
  if (enum_value == NULL) {
    PyErr_Format(PyExc_KeyError, "%d", number);
    return NULL;
  }
  return PyString_FromCppString(enum_value->name());
}

static PyObject* GetSyntax(PyBaseDescriptor *self, void *closure) {
  return PyString_InternFromString(
      FileDescriptor::SyntaxName(_GetDescriptor(self)->file()->syntax()));
}

static PyGetSetDef Getters[] = {
  { "name", (getter)GetName, NULL, "Last name"},
  { "full_name", (getter)GetFullName, NULL, "Full name"},
  { "_concrete_class", (getter)GetConcreteClass, NULL, "concrete class"},
  { "file", (getter)GetFile, NULL, "File descriptor"},

  { "fields", (getter)GetFieldsSeq, NULL, "Fields sequence"},
  { "fields_by_name", (getter)GetFieldsByName, NULL, "Fields by name"},
  { "fields_by_camelcase_name", (getter)GetFieldsByCamelcaseName, NULL,
    "Fields by camelCase name"},
  { "fields_by_number", (getter)GetFieldsByNumber, NULL, "Fields by number"},
  { "nested_types", (getter)GetNestedTypesSeq, NULL, "Nested types sequence"},
  { "nested_types_by_name", (getter)GetNestedTypesByName, NULL,
    "Nested types by name"},
  { "extensions", (getter)GetExtensions, NULL, "Extensions Sequence"},
  { "extensions_by_name", (getter)GetExtensionsByName, NULL,
    "Extensions by name"},
  { "extension_ranges", (getter)GetExtensionRanges, NULL, "Extension ranges"},
  { "enum_types", (getter)GetEnumsSeq, NULL, "Enum sequence"},
  { "enum_types_by_name", (getter)GetEnumTypesByName, NULL,
    "Enum types by name"},
  { "enum_values_by_name", (getter)GetEnumValuesByName, NULL,
    "Enum values by name"},
  { "oneofs_by_name", (getter)GetOneofsByName, NULL, "Oneofs by name"},
  { "oneofs", (getter)GetOneofsSeq, NULL, "Oneofs by name"},
  { "containing_type", (getter)GetContainingType, NULL,
    "Containing type"},
  { "is_extendable", (getter)IsExtendable, (setter)NULL},
  { "has_options", (getter)GetHasOptions, NULL, "Has Options"},
  { "_options", (getter)NULL, NULL, "Options"},
  { "_serialized_options", (getter)NULL, NULL, "Serialized Options"},
  { "syntax", (getter)GetSyntax, (setter)NULL, "Syntax"},
  {NULL}
};

static PyMethodDef Methods[] = {
  { "GetOptions", (PyCFunction)GetOptions, METH_NOARGS, },
  { "CopyToProto", (PyCFunction)CopyToProto, METH_O, },
  { "EnumValueName", (PyCFunction)EnumValueName, METH_VARARGS, },
  {NULL}
};

}  // namespace message_descriptor

PyTypeObject PyMessageDescriptor_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  FULL_MODULE_NAME ".MessageDescriptor",  // tp_name
  sizeof(PyBaseDescriptor),             // tp_basicsize
  0,                                    // tp_itemsize
  0,                                    // tp_dealloc
  0,                                    // tp_print
  0,                                    // tp_getattr
  0,                                    // tp_setattr
  0,                                    // tp_compare
  0,                                    // tp_repr
  0,                                    // tp_as_number
  0,                                    // tp_as_sequence
  0,                                    // tp_as_mapping
  0,                                    // tp_hash
  0,                                    // tp_call
  0,                                    // tp_str
  0,                                    // tp_getattro
  0,                                    // tp_setattro
  0,                                    // tp_as_buffer
  Py_TPFLAGS_DEFAULT,                   // tp_flags
  "A Message Descriptor",               // tp_doc
  0,                                    // tp_traverse
  0,                                    // tp_clear
  0,                                    // tp_richcompare
  0,                                    // tp_weaklistoffset
  0,                                    // tp_iter
  0,                                    // tp_iternext
  message_descriptor::Methods,          // tp_methods
  0,                                    // tp_members
  message_descriptor::Getters,          // tp_getset
  &descriptor::PyBaseDescriptor_Type,   // tp_base
};

PyObject* PyMessageDescriptor_FromDescriptor(
    const Descriptor* message_descriptor) {
  return descriptor::NewInternedDescriptor(
      &PyMessageDescriptor_Type, message_descriptor, NULL);
}

const Descriptor* PyMessageDescriptor_AsDescriptor(PyObject* obj) {
  if (!PyObject_TypeCheck(obj, &PyMessageDescriptor_Type)) {
    PyErr_SetString(PyExc_TypeError, "Not a MessageDescriptor");
    return NULL;
  }
  return reinterpret_cast<const Descriptor*>(
      reinterpret_cast<PyBaseDescriptor*>(obj)->descriptor);
}

namespace field_descriptor {

// Unchecked accessor to the C++ pointer.
static const FieldDescriptor* _GetDescriptor(
    PyBaseDescriptor *self) {
  return reinterpret_cast<const FieldDescriptor*>(self->descriptor);
}

static PyObject* GetFullName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->full_name());
}

static PyObject* GetName(PyBaseDescriptor *self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->name());
}

static PyObject* GetCamelcaseName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->camelcase_name());
}

static PyObject* GetJsonName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->json_name());
}

static PyObject* GetFile(PyBaseDescriptor *self, void *closure) {
  return PyFileDescriptor_FromDescriptor(_GetDescriptor(self)->file());
}

static PyObject* GetType(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->type());
}

static PyObject* GetCppType(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->cpp_type());
}

static PyObject* GetLabel(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->label());
}

static PyObject* GetNumber(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->number());
}

static PyObject* GetIndex(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->index());
}

static PyObject* GetID(PyBaseDescriptor *self, void *closure) {
  return PyLong_FromVoidPtr(self);
}

static PyObject* IsExtension(PyBaseDescriptor *self, void *closure) {
  return PyBool_FromLong(_GetDescriptor(self)->is_extension());
}

static PyObject* HasDefaultValue(PyBaseDescriptor *self, void *closure) {
  return PyBool_FromLong(_GetDescriptor(self)->has_default_value());
}

static PyObject* GetDefaultValue(PyBaseDescriptor *self, void *closure) {
  PyObject *result;

  if (_GetDescriptor(self)->is_repeated()) {
    return PyList_New(0);
  }


  switch (_GetDescriptor(self)->cpp_type()) {
    case FieldDescriptor::CPPTYPE_INT32: {
      int32 value = _GetDescriptor(self)->default_value_int32();
      result = PyInt_FromLong(value);
      break;
    }
    case FieldDescriptor::CPPTYPE_INT64: {
      int64 value = _GetDescriptor(self)->default_value_int64();
      result = PyLong_FromLongLong(value);
      break;
    }
    case FieldDescriptor::CPPTYPE_UINT32: {
      uint32 value = _GetDescriptor(self)->default_value_uint32();
      result = PyInt_FromSize_t(value);
      break;
    }
    case FieldDescriptor::CPPTYPE_UINT64: {
      uint64 value = _GetDescriptor(self)->default_value_uint64();
      result = PyLong_FromUnsignedLongLong(value);
      break;
    }
    case FieldDescriptor::CPPTYPE_FLOAT: {
      float value = _GetDescriptor(self)->default_value_float();
      result = PyFloat_FromDouble(value);
      break;
    }
    case FieldDescriptor::CPPTYPE_DOUBLE: {
      double value = _GetDescriptor(self)->default_value_double();
      result = PyFloat_FromDouble(value);
      break;
    }
    case FieldDescriptor::CPPTYPE_BOOL: {
      bool value = _GetDescriptor(self)->default_value_bool();
      result = PyBool_FromLong(value);
      break;
    }
    case FieldDescriptor::CPPTYPE_STRING: {
      const string& value = _GetDescriptor(self)->default_value_string();
      result = ToStringObject(_GetDescriptor(self), value);
      break;
    }
    case FieldDescriptor::CPPTYPE_ENUM: {
      const EnumValueDescriptor* value =
          _GetDescriptor(self)->default_value_enum();
      result = PyInt_FromLong(value->number());
      break;
    }
    case FieldDescriptor::CPPTYPE_MESSAGE: {
      Py_RETURN_NONE;
      break;
    }
    default:
      PyErr_Format(PyExc_NotImplementedError, "default value for %s",
                   _GetDescriptor(self)->full_name().c_str());
      return NULL;
  }
  return result;
}

static PyObject* GetCDescriptor(PyObject *self, void *closure) {
  Py_INCREF(self);
  return self;
}

static PyObject *GetEnumType(PyBaseDescriptor *self, void *closure) {
  const EnumDescriptor* enum_type = _GetDescriptor(self)->enum_type();
  if (enum_type) {
    return PyEnumDescriptor_FromDescriptor(enum_type);
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject *GetMessageType(PyBaseDescriptor *self, void *closure) {
  const Descriptor* message_type = _GetDescriptor(self)->message_type();
  if (message_type) {
    return PyMessageDescriptor_FromDescriptor(message_type);
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject* GetContainingType(PyBaseDescriptor *self, void *closure) {
  const Descriptor* containing_type =
      _GetDescriptor(self)->containing_type();
  if (containing_type) {
    return PyMessageDescriptor_FromDescriptor(containing_type);
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject* GetExtensionScope(PyBaseDescriptor *self, void *closure) {
  const Descriptor* extension_scope =
      _GetDescriptor(self)->extension_scope();
  if (extension_scope) {
    return PyMessageDescriptor_FromDescriptor(extension_scope);
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject* GetContainingOneof(PyBaseDescriptor *self, void *closure) {
  const OneofDescriptor* containing_oneof =
      _GetDescriptor(self)->containing_oneof();
  if (containing_oneof) {
    return PyOneofDescriptor_FromDescriptor(containing_oneof);
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject* GetHasOptions(PyBaseDescriptor *self, void *closure) {
  const FieldOptions& options(_GetDescriptor(self)->options());
  if (&options != &FieldOptions::default_instance()) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}

static PyObject* GetOptions(PyBaseDescriptor *self) {
  return GetOrBuildOptions(_GetDescriptor(self));
}

static PyGetSetDef Getters[] = {
  { "full_name", (getter)GetFullName, NULL, "Full name"},
  { "name", (getter)GetName, NULL, "Unqualified name"},
  { "camelcase_name", (getter)GetCamelcaseName, NULL, "Camelcase name"},
  { "json_name", (getter)GetJsonName, NULL, "Json name"},
  { "file", (getter)GetFile, NULL, "File Descriptor"},
  { "type", (getter)GetType, NULL, "C++ Type"},
  { "cpp_type", (getter)GetCppType, NULL, "C++ Type"},
  { "label", (getter)GetLabel, NULL, "Label"},
  { "number", (getter)GetNumber, NULL, "Number"},
  { "index", (getter)GetIndex, NULL, "Index"},
  { "default_value", (getter)GetDefaultValue, NULL, "Default Value"},
  { "has_default_value", (getter)HasDefaultValue},
  { "is_extension", (getter)IsExtension, NULL, "ID"},
  { "id", (getter)GetID, NULL, "ID"},
  { "_cdescriptor", (getter)GetCDescriptor, NULL, "HAACK REMOVE ME"},
  { "message_type", (getter)GetMessageType, NULL, "Message type"},
  { "enum_type", (getter)GetEnumType, NULL, "Enum type"},
  { "containing_type", (getter)GetContainingType, NULL,
    "Containing type"},
  { "extension_scope", (getter)GetExtensionScope, (setter)NULL,
    "Extension scope"},
  { "containing_oneof", (getter)GetContainingOneof, NULL,
    "Containing oneof"},
  { "has_options", (getter)GetHasOptions, NULL, "Has Options"},
  { "_options", (getter)NULL, NULL, "Options"},
  { "_serialized_options", (getter)NULL, NULL, "Serialized Options"},
  {NULL}
};

static PyMethodDef Methods[] = {
  { "GetOptions", (PyCFunction)GetOptions, METH_NOARGS, },
  {NULL}
};

}  // namespace field_descriptor

PyTypeObject PyFieldDescriptor_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  FULL_MODULE_NAME ".FieldDescriptor",  // tp_name
  sizeof(PyBaseDescriptor),             // tp_basicsize
  0,                                    // tp_itemsize
  0,                                    // tp_dealloc
  0,                                    // tp_print
  0,                                    // tp_getattr
  0,                                    // tp_setattr
  0,                                    // tp_compare
  0,                                    // tp_repr
  0,                                    // tp_as_number
  0,                                    // tp_as_sequence
  0,                                    // tp_as_mapping
  0,                                    // tp_hash
  0,                                    // tp_call
  0,                                    // tp_str
  0,                                    // tp_getattro
  0,                                    // tp_setattro
  0,                                    // tp_as_buffer
  Py_TPFLAGS_DEFAULT,                   // tp_flags
  "A Field Descriptor",                 // tp_doc
  0,                                    // tp_traverse
  0,                                    // tp_clear
  0,                                    // tp_richcompare
  0,                                    // tp_weaklistoffset
  0,                                    // tp_iter
  0,                                    // tp_iternext
  field_descriptor::Methods,            // tp_methods
  0,                                    // tp_members
  field_descriptor::Getters,            // tp_getset
  &descriptor::PyBaseDescriptor_Type,   // tp_base
};

PyObject* PyFieldDescriptor_FromDescriptor(
    const FieldDescriptor* field_descriptor) {
  return descriptor::NewInternedDescriptor(
      &PyFieldDescriptor_Type, field_descriptor, NULL);
}

const FieldDescriptor* PyFieldDescriptor_AsDescriptor(PyObject* obj) {
  if (!PyObject_TypeCheck(obj, &PyFieldDescriptor_Type)) {
    PyErr_SetString(PyExc_TypeError, "Not a FieldDescriptor");
    return NULL;
  }
  return reinterpret_cast<const FieldDescriptor*>(
      reinterpret_cast<PyBaseDescriptor*>(obj)->descriptor);
}

namespace enum_descriptor {

// Unchecked accessor to the C++ pointer.
static const EnumDescriptor* _GetDescriptor(
    PyBaseDescriptor *self) {
  return reinterpret_cast<const EnumDescriptor*>(self->descriptor);
}

static PyObject* GetFullName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->full_name());
}

static PyObject* GetName(PyBaseDescriptor *self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->name());
}

static PyObject* GetFile(PyBaseDescriptor *self, void *closure) {
  return PyFileDescriptor_FromDescriptor(_GetDescriptor(self)->file());
}

static PyObject* GetEnumvaluesByName(PyBaseDescriptor* self, void *closure) {
  return NewEnumValuesByName(_GetDescriptor(self));
}

static PyObject* GetEnumvaluesByNumber(PyBaseDescriptor* self, void *closure) {
  return NewEnumValuesByNumber(_GetDescriptor(self));
}

static PyObject* GetEnumvaluesSeq(PyBaseDescriptor* self, void *closure) {
  return NewEnumValuesSeq(_GetDescriptor(self));
}

static PyObject* GetContainingType(PyBaseDescriptor *self, void *closure) {
  const Descriptor* containing_type =
      _GetDescriptor(self)->containing_type();
  if (containing_type) {
    return PyMessageDescriptor_FromDescriptor(containing_type);
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject* GetHasOptions(PyBaseDescriptor *self, void *closure) {
  const EnumOptions& options(_GetDescriptor(self)->options());
  if (&options != &EnumOptions::default_instance()) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}

static PyObject* GetOptions(PyBaseDescriptor *self) {
  return GetOrBuildOptions(_GetDescriptor(self));
}

static PyObject* CopyToProto(PyBaseDescriptor *self, PyObject *target) {
  return CopyToPythonProto<EnumDescriptorProto>(_GetDescriptor(self), target);
}

static PyMethodDef Methods[] = {
  { "GetOptions", (PyCFunction)GetOptions, METH_NOARGS, },
  { "CopyToProto", (PyCFunction)CopyToProto, METH_O, },
  {NULL}
};

static PyGetSetDef Getters[] = {
  { "full_name", (getter)GetFullName, NULL, "Full name"},
  { "name", (getter)GetName, NULL, "last name"},
  { "file", (getter)GetFile, NULL, "File descriptor"},
  { "values", (getter)GetEnumvaluesSeq, NULL, "values"},
  { "values_by_name", (getter)GetEnumvaluesByName, NULL,
    "Enum values by name"},
  { "values_by_number", (getter)GetEnumvaluesByNumber, NULL,
    "Enum values by number"},

  { "containing_type", (getter)GetContainingType, NULL,
    "Containing type"},
  { "has_options", (getter)GetHasOptions, NULL, "Has Options"},
  { "_options", (getter)NULL, NULL, "Options"},
  { "_serialized_options", (getter)NULL, NULL, "Serialized Options"},
  {NULL}
};

}  // namespace enum_descriptor

PyTypeObject PyEnumDescriptor_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  FULL_MODULE_NAME ".EnumDescriptor",   // tp_name
  sizeof(PyBaseDescriptor),             // tp_basicsize
  0,                                    // tp_itemsize
  0,                                    // tp_dealloc
  0,                                    // tp_print
  0,                                    // tp_getattr
  0,                                    // tp_setattr
  0,                                    // tp_compare
  0,                                    // tp_repr
  0,                                    // tp_as_number
  0,                                    // tp_as_sequence
  0,                                    // tp_as_mapping
  0,                                    // tp_hash
  0,                                    // tp_call
  0,                                    // tp_str
  0,                                    // tp_getattro
  0,                                    // tp_setattro
  0,                                    // tp_as_buffer
  Py_TPFLAGS_DEFAULT,                   // tp_flags
  "A Enum Descriptor",                  // tp_doc
  0,                                    // tp_traverse
  0,                                    // tp_clear
  0,                                    // tp_richcompare
  0,                                    // tp_weaklistoffset
  0,                                    // tp_iter
  0,                                    // tp_iternext
  enum_descriptor::Methods,             // tp_methods
  0,                                    // tp_members
  enum_descriptor::Getters,             // tp_getset
  &descriptor::PyBaseDescriptor_Type,   // tp_base
};

PyObject* PyEnumDescriptor_FromDescriptor(
    const EnumDescriptor* enum_descriptor) {
  return descriptor::NewInternedDescriptor(
      &PyEnumDescriptor_Type, enum_descriptor, NULL);
}

const EnumDescriptor* PyEnumDescriptor_AsDescriptor(PyObject* obj) {
  if (!PyObject_TypeCheck(obj, &PyEnumDescriptor_Type)) {
    PyErr_SetString(PyExc_TypeError, "Not an EnumDescriptor");
    return NULL;
  }
  return reinterpret_cast<const EnumDescriptor*>(
      reinterpret_cast<PyBaseDescriptor*>(obj)->descriptor);
}

namespace enumvalue_descriptor {

// Unchecked accessor to the C++ pointer.
static const EnumValueDescriptor* _GetDescriptor(
    PyBaseDescriptor *self) {
  return reinterpret_cast<const EnumValueDescriptor*>(self->descriptor);
}

static PyObject* GetName(PyBaseDescriptor *self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->name());
}

static PyObject* GetNumber(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->number());
}

static PyObject* GetIndex(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->index());
}

static PyObject* GetType(PyBaseDescriptor *self, void *closure) {
  return PyEnumDescriptor_FromDescriptor(_GetDescriptor(self)->type());
}

static PyObject* GetHasOptions(PyBaseDescriptor *self, void *closure) {
  const EnumValueOptions& options(_GetDescriptor(self)->options());
  if (&options != &EnumValueOptions::default_instance()) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}

static PyObject* GetOptions(PyBaseDescriptor *self) {
  return GetOrBuildOptions(_GetDescriptor(self));
}

static PyGetSetDef Getters[] = {
  { "name", (getter)GetName, NULL, "name"},
  { "number", (getter)GetNumber, NULL, "number"},
  { "index", (getter)GetIndex, NULL, "index"},
  { "type", (getter)GetType, NULL, "index"},

  { "has_options", (getter)GetHasOptions, NULL, "Has Options"},
  { "_options", (getter)NULL, NULL, "Options"},
  { "_serialized_options", (getter)NULL, NULL, "Serialized Options"},
  {NULL}
};

static PyMethodDef Methods[] = {
  { "GetOptions", (PyCFunction)GetOptions, METH_NOARGS, },
  {NULL}
};

}  // namespace enumvalue_descriptor

PyTypeObject PyEnumValueDescriptor_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  FULL_MODULE_NAME ".EnumValueDescriptor",  // tp_name
  sizeof(PyBaseDescriptor),             // tp_basicsize
  0,                                    // tp_itemsize
  0,                                    // tp_dealloc
  0,                                    // tp_print
  0,                                    // tp_getattr
  0,                                    // tp_setattr
  0,                                    // tp_compare
  0,                                    // tp_repr
  0,                                    // tp_as_number
  0,                                    // tp_as_sequence
  0,                                    // tp_as_mapping
  0,                                    // tp_hash
  0,                                    // tp_call
  0,                                    // tp_str
  0,                                    // tp_getattro
  0,                                    // tp_setattro
  0,                                    // tp_as_buffer
  Py_TPFLAGS_DEFAULT,                   // tp_flags
  "A EnumValue Descriptor",             // tp_doc
  0,                                    // tp_traverse
  0,                                    // tp_clear
  0,                                    // tp_richcompare
  0,                                    // tp_weaklistoffset
  0,                                    // tp_iter
  0,                                    // tp_iternext
  enumvalue_descriptor::Methods,        // tp_methods
  0,                                    // tp_members
  enumvalue_descriptor::Getters,        // tp_getset
  &descriptor::PyBaseDescriptor_Type,   // tp_base
};

PyObject* PyEnumValueDescriptor_FromDescriptor(
    const EnumValueDescriptor* enumvalue_descriptor) {
  return descriptor::NewInternedDescriptor(
      &PyEnumValueDescriptor_Type, enumvalue_descriptor, NULL);
}

namespace file_descriptor {

// Unchecked accessor to the C++ pointer.
static const FileDescriptor* _GetDescriptor(PyFileDescriptor *self) {
  return reinterpret_cast<const FileDescriptor*>(self->base.descriptor);
}

static void Dealloc(PyFileDescriptor* self) {
  Py_XDECREF(self->serialized_pb);
  descriptor::Dealloc(&self->base);
}

static PyObject* GetPool(PyFileDescriptor *self, void *closure) {
  PyObject* pool = reinterpret_cast<PyObject*>(
      GetDescriptorPool_FromPool(_GetDescriptor(self)->pool()));
  Py_XINCREF(pool);
  return pool;
}

static PyObject* GetName(PyFileDescriptor *self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->name());
}

static PyObject* GetPackage(PyFileDescriptor *self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->package());
}

static PyObject* GetSerializedPb(PyFileDescriptor *self, void *closure) {
  PyObject *serialized_pb = self->serialized_pb;
  if (serialized_pb != NULL) {
    Py_INCREF(serialized_pb);
    return serialized_pb;
  }
  FileDescriptorProto file_proto;
  _GetDescriptor(self)->CopyTo(&file_proto);
  string contents;
  file_proto.SerializePartialToString(&contents);
  self->serialized_pb = PyBytes_FromStringAndSize(
      contents.c_str(), contents.size());
  if (self->serialized_pb == NULL) {
    return NULL;
  }
  Py_INCREF(self->serialized_pb);
  return self->serialized_pb;
}

static PyObject* GetMessageTypesByName(PyFileDescriptor* self, void *closure) {
  return NewFileMessageTypesByName(_GetDescriptor(self));
}

static PyObject* GetEnumTypesByName(PyFileDescriptor* self, void *closure) {
  return NewFileEnumTypesByName(_GetDescriptor(self));
}

static PyObject* GetExtensionsByName(PyFileDescriptor* self, void *closure) {
  return NewFileExtensionsByName(_GetDescriptor(self));
}

static PyObject* GetServicesByName(PyFileDescriptor* self, void *closure) {
  return NewFileServicesByName(_GetDescriptor(self));
}

static PyObject* GetDependencies(PyFileDescriptor* self, void *closure) {
  return NewFileDependencies(_GetDescriptor(self));
}

static PyObject* GetPublicDependencies(PyFileDescriptor* self, void *closure) {
  return NewFilePublicDependencies(_GetDescriptor(self));
}

static PyObject* GetHasOptions(PyFileDescriptor *self, void *closure) {
  const FileOptions& options(_GetDescriptor(self)->options());
  if (&options != &FileOptions::default_instance()) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}

static PyObject* GetOptions(PyFileDescriptor *self) {
  return GetOrBuildOptions(_GetDescriptor(self));
}

static PyObject* GetSyntax(PyFileDescriptor *self, void *closure) {
  return PyString_InternFromString(
      FileDescriptor::SyntaxName(_GetDescriptor(self)->syntax()));
}

static PyObject* CopyToProto(PyFileDescriptor *self, PyObject *target) {
  return CopyToPythonProto<FileDescriptorProto>(_GetDescriptor(self), target);
}

static PyGetSetDef Getters[] = {
  { "pool", (getter)GetPool, NULL, "pool"},
  { "name", (getter)GetName, NULL, "name"},
  { "package", (getter)GetPackage, NULL, "package"},
  { "serialized_pb", (getter)GetSerializedPb},
  { "message_types_by_name", (getter)GetMessageTypesByName, NULL,
    "Messages by name"},
  { "enum_types_by_name", (getter)GetEnumTypesByName, NULL, "Enums by name"},
  { "extensions_by_name", (getter)GetExtensionsByName, NULL,
    "Extensions by name"},
  { "services_by_name", (getter)GetServicesByName, NULL, "Services by name"},
  { "dependencies", (getter)GetDependencies, NULL, "Dependencies"},
  { "public_dependencies", (getter)GetPublicDependencies, NULL, "Dependencies"},

  { "has_options", (getter)GetHasOptions, NULL, "Has Options"},
  { "_options", (getter)NULL, NULL, "Options"},
  { "_serialized_options", (getter)NULL, NULL, "Serialized Options"},
  { "syntax", (getter)GetSyntax, (setter)NULL, "Syntax"},
  {NULL}
};

static PyMethodDef Methods[] = {
  { "GetOptions", (PyCFunction)GetOptions, METH_NOARGS, },
  { "CopyToProto", (PyCFunction)CopyToProto, METH_O, },
  {NULL}
};

}  // namespace file_descriptor

PyTypeObject PyFileDescriptor_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  FULL_MODULE_NAME ".FileDescriptor",   // tp_name
  sizeof(PyFileDescriptor),             // tp_basicsize
  0,                                    // tp_itemsize
  (destructor)file_descriptor::Dealloc,  // tp_dealloc
  0,                                    // tp_print
  0,                                    // tp_getattr
  0,                                    // tp_setattr
  0,                                    // tp_compare
  0,                                    // tp_repr
  0,                                    // tp_as_number
  0,                                    // tp_as_sequence
  0,                                    // tp_as_mapping
  0,                                    // tp_hash
  0,                                    // tp_call
  0,                                    // tp_str
  0,                                    // tp_getattro
  0,                                    // tp_setattro
  0,                                    // tp_as_buffer
  Py_TPFLAGS_DEFAULT,                   // tp_flags
  "A File Descriptor",                  // tp_doc
  0,                                    // tp_traverse
  0,                                    // tp_clear
  0,                                    // tp_richcompare
  0,                                    // tp_weaklistoffset
  0,                                    // tp_iter
  0,                                    // tp_iternext
  file_descriptor::Methods,             // tp_methods
  0,                                    // tp_members
  file_descriptor::Getters,             // tp_getset
  &descriptor::PyBaseDescriptor_Type,   // tp_base
  0,                                    // tp_dict
  0,                                    // tp_descr_get
  0,                                    // tp_descr_set
  0,                                    // tp_dictoffset
  0,                                    // tp_init
  0,                                    // tp_alloc
  0,                                    // tp_new
  PyObject_Del,                         // tp_free
};

PyObject* PyFileDescriptor_FromDescriptor(
    const FileDescriptor* file_descriptor) {
  return PyFileDescriptor_FromDescriptorWithSerializedPb(file_descriptor,
                                                         NULL);
}

PyObject* PyFileDescriptor_FromDescriptorWithSerializedPb(
    const FileDescriptor* file_descriptor, PyObject *serialized_pb) {
  bool was_created;
  PyObject* py_descriptor = descriptor::NewInternedDescriptor(
      &PyFileDescriptor_Type, file_descriptor, &was_created);
  if (py_descriptor == NULL) {
    return NULL;
  }
  if (was_created) {
    PyFileDescriptor* cfile_descriptor =
        reinterpret_cast<PyFileDescriptor*>(py_descriptor);
    Py_XINCREF(serialized_pb);
    cfile_descriptor->serialized_pb = serialized_pb;
  }
  // TODO(amauryfa): In the case of a cached object, check that serialized_pb
  // is the same as before.

  return py_descriptor;
}

const FileDescriptor* PyFileDescriptor_AsDescriptor(PyObject* obj) {
  if (!PyObject_TypeCheck(obj, &PyFileDescriptor_Type)) {
    PyErr_SetString(PyExc_TypeError, "Not a FileDescriptor");
    return NULL;
  }
  return reinterpret_cast<const FileDescriptor*>(
      reinterpret_cast<PyBaseDescriptor*>(obj)->descriptor);
}

namespace oneof_descriptor {

// Unchecked accessor to the C++ pointer.
static const OneofDescriptor* _GetDescriptor(
    PyBaseDescriptor *self) {
  return reinterpret_cast<const OneofDescriptor*>(self->descriptor);
}

static PyObject* GetName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->name());
}

static PyObject* GetFullName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->full_name());
}

static PyObject* GetIndex(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->index());
}

static PyObject* GetFields(PyBaseDescriptor* self, void *closure) {
  return NewOneofFieldsSeq(_GetDescriptor(self));
}

static PyObject* GetContainingType(PyBaseDescriptor *self, void *closure) {
  const Descriptor* containing_type =
      _GetDescriptor(self)->containing_type();
  if (containing_type) {
    return PyMessageDescriptor_FromDescriptor(containing_type);
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject* GetHasOptions(PyBaseDescriptor *self, void *closure) {
  const OneofOptions& options(_GetDescriptor(self)->options());
  if (&options != &OneofOptions::default_instance()) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}

static PyObject* GetOptions(PyBaseDescriptor *self) {
  return GetOrBuildOptions(_GetDescriptor(self));
}

static PyGetSetDef Getters[] = {
  { "name", (getter)GetName, NULL, "Name"},
  { "full_name", (getter)GetFullName, NULL, "Full name"},
  { "index", (getter)GetIndex, NULL, "Index"},

  { "containing_type", (getter)GetContainingType, NULL, "Containing type"},
  { "has_options", (getter)GetHasOptions, NULL, "Has Options"},
  { "_options", (getter)NULL, NULL, "Options"},
  { "_serialized_options", (getter)NULL, NULL, "Serialized Options"},
  { "fields", (getter)GetFields, NULL, "Fields"},
  {NULL}
};

static PyMethodDef Methods[] = {
  { "GetOptions", (PyCFunction)GetOptions, METH_NOARGS },
  {NULL}
};

}  // namespace oneof_descriptor

PyTypeObject PyOneofDescriptor_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  FULL_MODULE_NAME ".OneofDescriptor",  // tp_name
  sizeof(PyBaseDescriptor),             // tp_basicsize
  0,                                    // tp_itemsize
  0,                                    // tp_dealloc
  0,                                    // tp_print
  0,                                    // tp_getattr
  0,                                    // tp_setattr
  0,                                    // tp_compare
  0,                                    // tp_repr
  0,                                    // tp_as_number
  0,                                    // tp_as_sequence
  0,                                    // tp_as_mapping
  0,                                    // tp_hash
  0,                                    // tp_call
  0,                                    // tp_str
  0,                                    // tp_getattro
  0,                                    // tp_setattro
  0,                                    // tp_as_buffer
  Py_TPFLAGS_DEFAULT,                   // tp_flags
  "A Oneof Descriptor",                 // tp_doc
  0,                                    // tp_traverse
  0,                                    // tp_clear
  0,                                    // tp_richcompare
  0,                                    // tp_weaklistoffset
  0,                                    // tp_iter
  0,                                    // tp_iternext
  oneof_descriptor::Methods,            // tp_methods
  0,                                    // tp_members
  oneof_descriptor::Getters,            // tp_getset
  &descriptor::PyBaseDescriptor_Type,   // tp_base
};

PyObject* PyOneofDescriptor_FromDescriptor(
    const OneofDescriptor* oneof_descriptor) {
  return descriptor::NewInternedDescriptor(
      &PyOneofDescriptor_Type, oneof_descriptor, NULL);
}

namespace service_descriptor {

// Unchecked accessor to the C++ pointer.
static const ServiceDescriptor* _GetDescriptor(
    PyBaseDescriptor *self) {
  return reinterpret_cast<const ServiceDescriptor*>(self->descriptor);
}

static PyObject* GetName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->name());
}

static PyObject* GetFullName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->full_name());
}

static PyObject* GetFile(PyBaseDescriptor *self, void *closure) {
  return PyFileDescriptor_FromDescriptor(_GetDescriptor(self)->file());
}

static PyObject* GetIndex(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->index());
}

static PyObject* GetMethods(PyBaseDescriptor* self, void *closure) {
  return NewServiceMethodsSeq(_GetDescriptor(self));
}

static PyObject* GetMethodsByName(PyBaseDescriptor* self, void *closure) {
  return NewServiceMethodsByName(_GetDescriptor(self));
}

static PyObject* FindMethodByName(PyBaseDescriptor *self, PyObject* arg) {
  Py_ssize_t name_size;
  char* name;
  if (PyString_AsStringAndSize(arg, &name, &name_size) < 0) {
    return NULL;
  }

  const MethodDescriptor* method_descriptor =
      _GetDescriptor(self)->FindMethodByName(string(name, name_size));
  if (method_descriptor == NULL) {
    PyErr_Format(PyExc_KeyError, "Couldn't find method %.200s", name);
    return NULL;
  }

  return PyMethodDescriptor_FromDescriptor(method_descriptor);
}

static PyObject* GetOptions(PyBaseDescriptor *self) {
  return GetOrBuildOptions(_GetDescriptor(self));
}

static PyObject* CopyToProto(PyBaseDescriptor *self, PyObject *target) {
  return CopyToPythonProto<ServiceDescriptorProto>(_GetDescriptor(self),
                                                   target);
}

static PyGetSetDef Getters[] = {
  { "name", (getter)GetName, NULL, "Name", NULL},
  { "full_name", (getter)GetFullName, NULL, "Full name", NULL},
  { "file", (getter)GetFile, NULL, "File descriptor"},
  { "index", (getter)GetIndex, NULL, "Index", NULL},

  { "methods", (getter)GetMethods, NULL, "Methods", NULL},
  { "methods_by_name", (getter)GetMethodsByName, NULL, "Methods by name", NULL},
  {NULL}
};

static PyMethodDef Methods[] = {
  { "GetOptions", (PyCFunction)GetOptions, METH_NOARGS },
  { "CopyToProto", (PyCFunction)CopyToProto, METH_O, },
  { "FindMethodByName", (PyCFunction)FindMethodByName, METH_O },
  {NULL}
};

}  // namespace service_descriptor

PyTypeObject PyServiceDescriptor_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  FULL_MODULE_NAME ".ServiceDescriptor",  // tp_name
  sizeof(PyBaseDescriptor),             // tp_basicsize
  0,                                    // tp_itemsize
  0,                                    // tp_dealloc
  0,                                    // tp_print
  0,                                    // tp_getattr
  0,                                    // tp_setattr
  0,                                    // tp_compare
  0,                                    // tp_repr
  0,                                    // tp_as_number
  0,                                    // tp_as_sequence
  0,                                    // tp_as_mapping
  0,                                    // tp_hash
  0,                                    // tp_call
  0,                                    // tp_str
  0,                                    // tp_getattro
  0,                                    // tp_setattro
  0,                                    // tp_as_buffer
  Py_TPFLAGS_DEFAULT,                   // tp_flags
  "A Service Descriptor",               // tp_doc
  0,                                    // tp_traverse
  0,                                    // tp_clear
  0,                                    // tp_richcompare
  0,                                    // tp_weaklistoffset
  0,                                    // tp_iter
  0,                                    // tp_iternext
  service_descriptor::Methods,          // tp_methods
  0,                                    // tp_members
  service_descriptor::Getters,          // tp_getset
  &descriptor::PyBaseDescriptor_Type,   // tp_base
};

PyObject* PyServiceDescriptor_FromDescriptor(
    const ServiceDescriptor* service_descriptor) {
  return descriptor::NewInternedDescriptor(
      &PyServiceDescriptor_Type, service_descriptor, NULL);
}

const ServiceDescriptor* PyServiceDescriptor_AsDescriptor(PyObject* obj) {
  if (!PyObject_TypeCheck(obj, &PyServiceDescriptor_Type)) {
    PyErr_SetString(PyExc_TypeError, "Not a ServiceDescriptor");
    return NULL;
  }
  return reinterpret_cast<const ServiceDescriptor*>(
      reinterpret_cast<PyBaseDescriptor*>(obj)->descriptor);
}

namespace method_descriptor {

// Unchecked accessor to the C++ pointer.
static const MethodDescriptor* _GetDescriptor(
    PyBaseDescriptor *self) {
  return reinterpret_cast<const MethodDescriptor*>(self->descriptor);
}

static PyObject* GetName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->name());
}

static PyObject* GetFullName(PyBaseDescriptor* self, void *closure) {
  return PyString_FromCppString(_GetDescriptor(self)->full_name());
}

static PyObject* GetIndex(PyBaseDescriptor *self, void *closure) {
  return PyInt_FromLong(_GetDescriptor(self)->index());
}

static PyObject* GetContainingService(PyBaseDescriptor *self, void *closure) {
  const ServiceDescriptor* containing_service =
      _GetDescriptor(self)->service();
  return PyServiceDescriptor_FromDescriptor(containing_service);
}

static PyObject* GetInputType(PyBaseDescriptor *self, void *closure) {
  const Descriptor* input_type = _GetDescriptor(self)->input_type();
  return PyMessageDescriptor_FromDescriptor(input_type);
}

static PyObject* GetOutputType(PyBaseDescriptor *self, void *closure) {
  const Descriptor* output_type = _GetDescriptor(self)->output_type();
  return PyMessageDescriptor_FromDescriptor(output_type);
}

static PyObject* GetOptions(PyBaseDescriptor *self) {
  return GetOrBuildOptions(_GetDescriptor(self));
}

static PyObject* CopyToProto(PyBaseDescriptor *self, PyObject *target) {
  return CopyToPythonProto<MethodDescriptorProto>(_GetDescriptor(self), target);
}

static PyGetSetDef Getters[] = {
  { "name", (getter)GetName, NULL, "Name", NULL},
  { "full_name", (getter)GetFullName, NULL, "Full name", NULL},
  { "index", (getter)GetIndex, NULL, "Index", NULL},
  { "containing_service", (getter)GetContainingService, NULL,
    "Containing service", NULL},
  { "input_type", (getter)GetInputType, NULL, "Input type", NULL},
  { "output_type", (getter)GetOutputType, NULL, "Output type", NULL},
  {NULL}
};

static PyMethodDef Methods[] = {
  { "GetOptions", (PyCFunction)GetOptions, METH_NOARGS, },
  { "CopyToProto", (PyCFunction)CopyToProto, METH_O, },
  {NULL}
};

}  // namespace method_descriptor

PyTypeObject PyMethodDescriptor_Type = {
  PyVarObject_HEAD_INIT(&PyType_Type, 0)
  FULL_MODULE_NAME ".MethodDescriptor",  // tp_name
  sizeof(PyBaseDescriptor),             // tp_basicsize
  0,                                    // tp_itemsize
  0,                                    // tp_dealloc
  0,                                    // tp_print
  0,                                    // tp_getattr
  0,                                    // tp_setattr
  0,                                    // tp_compare
  0,                                    // tp_repr
  0,                                    // tp_as_number
  0,                                    // tp_as_sequence
  0,                                    // tp_as_mapping
  0,                                    // tp_hash
  0,                                    // tp_call
  0,                                    // tp_str
  0,                                    // tp_getattro
  0,                                    // tp_setattro
  0,                                    // tp_as_buffer
  Py_TPFLAGS_DEFAULT,                   // tp_flags
  "A Method Descriptor",                // tp_doc
  0,                                    // tp_traverse
  0,                                    // tp_clear
  0,                                    // tp_richcompare
  0,                                    // tp_weaklistoffset
  0,                                    // tp_iter
  0,                                    // tp_iternext
  method_descriptor::Methods,           // tp_methods
  0,                                    // tp_members
  method_descriptor::Getters,           // tp_getset
  &descriptor::PyBaseDescriptor_Type,   // tp_base
};

PyObject* PyMethodDescriptor_FromDescriptor(
    const MethodDescriptor* method_descriptor) {
  return descriptor::NewInternedDescriptor(
      &PyMethodDescriptor_Type, method_descriptor, NULL);
}

const MethodDescriptor* PyMethodDescriptor_AsDescriptor(PyObject* obj) {
  if (!PyObject_TypeCheck(obj, &PyMethodDescriptor_Type)) {
    PyErr_SetString(PyExc_TypeError, "Not a MethodDescriptor");
    return NULL;
  }
  return reinterpret_cast<const MethodDescriptor*>(
      reinterpret_cast<PyBaseDescriptor*>(obj)->descriptor);
}

// Add a enum values to a type dictionary.
static bool AddEnumValues(PyTypeObject *type,
                          const EnumDescriptor* enum_descriptor) {
  for (int i = 0; i < enum_descriptor->value_count(); ++i) {
    const EnumValueDescriptor* value = enum_descriptor->value(i);
    ScopedPyObjectPtr obj(PyInt_FromLong(value->number()));
    if (obj == NULL) {
      return false;
    }
    if (PyDict_SetItemString(type->tp_dict, value->name().c_str(), obj.get()) <
        0) {
      return false;
    }
  }
  return true;
}

static bool AddIntConstant(PyTypeObject *type, const char* name, int value) {
  ScopedPyObjectPtr obj(PyInt_FromLong(value));
  if (PyDict_SetItemString(type->tp_dict, name, obj.get()) < 0) {
    return false;
  }
  return true;
}


bool InitDescriptor() {
  if (PyType_Ready(&PyMessageDescriptor_Type) < 0)
    return false;

  if (PyType_Ready(&PyFieldDescriptor_Type) < 0)
    return false;

  if (!AddEnumValues(&PyFieldDescriptor_Type,
                     FieldDescriptorProto::Label_descriptor())) {
    return false;
  }
  if (!AddEnumValues(&PyFieldDescriptor_Type,
                     FieldDescriptorProto::Type_descriptor())) {
    return false;
  }
#define ADD_FIELDDESC_CONSTANT(NAME) AddIntConstant( \
    &PyFieldDescriptor_Type, #NAME, FieldDescriptor::NAME)
  if (!ADD_FIELDDESC_CONSTANT(CPPTYPE_INT32) ||
      !ADD_FIELDDESC_CONSTANT(CPPTYPE_INT64) ||
      !ADD_FIELDDESC_CONSTANT(CPPTYPE_UINT32) ||
      !ADD_FIELDDESC_CONSTANT(CPPTYPE_UINT64) ||
      !ADD_FIELDDESC_CONSTANT(CPPTYPE_DOUBLE) ||
      !ADD_FIELDDESC_CONSTANT(CPPTYPE_FLOAT) ||
      !ADD_FIELDDESC_CONSTANT(CPPTYPE_BOOL) ||
      !ADD_FIELDDESC_CONSTANT(CPPTYPE_ENUM) ||
      !ADD_FIELDDESC_CONSTANT(CPPTYPE_STRING) ||
      !ADD_FIELDDESC_CONSTANT(CPPTYPE_MESSAGE)) {
    return false;
  }
#undef ADD_FIELDDESC_CONSTANT

  if (PyType_Ready(&PyEnumDescriptor_Type) < 0)
    return false;

  if (PyType_Ready(&PyEnumValueDescriptor_Type) < 0)
    return false;

  if (PyType_Ready(&PyFileDescriptor_Type) < 0)
    return false;

  if (PyType_Ready(&PyOneofDescriptor_Type) < 0)
    return false;

  if (PyType_Ready(&PyServiceDescriptor_Type) < 0)
    return false;

  if (PyType_Ready(&PyMethodDescriptor_Type) < 0)
    return false;

  if (!InitDescriptorMappingTypes())
    return false;

  // Initialize globals defined in this file.
  interned_descriptors = new std::unordered_map<const void*, PyObject*>;

  return true;
}

}  // namespace python
}  // namespace protobuf
}  // namespace google
