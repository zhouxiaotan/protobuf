// Protocol Buffers - Google's data interchange format
// Copyright 2014 Google Inc.  All rights reserved.
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

#ifndef __GOOGLE_PROTOBUF_RUBY_PROTOBUF_H__
#define __GOOGLE_PROTOBUF_RUBY_PROTOBUF_H__

#include <ruby/ruby.h>
#include <ruby/vm.h>
#include <ruby/encoding.h>

#include "ruby-upb.h"

// Forward decls.
struct DescriptorPool;
struct Descriptor;
struct TypeInfo;
struct FileDescriptor;
struct FieldDescriptor;
struct EnumDescriptor;
struct Message;
struct MessageLayout;
struct MessageField;
struct MessageBuilderContext;
struct EnumBuilderContext;
struct FileBuilderContext;
struct Builder;

typedef struct DescriptorPool DescriptorPool;
typedef struct Descriptor Descriptor;
typedef struct TypeInfo TypeInfo;
typedef struct FileDescriptor FileDescriptor;
typedef struct FieldDescriptor FieldDescriptor;
typedef struct OneofDescriptor OneofDescriptor;
typedef struct EnumDescriptor EnumDescriptor;
typedef struct Message Message;
typedef struct MessageLayout MessageLayout;
typedef struct MessageField MessageField;
typedef struct MessageOneof MessageOneof;
typedef struct MessageBuilderContext MessageBuilderContext;
typedef struct OneofBuilderContext OneofBuilderContext;
typedef struct EnumBuilderContext EnumBuilderContext;
typedef struct FileBuilderContext FileBuilderContext;
typedef struct Builder Builder;

/*
 It can be a bit confusing how the C structs defined below and the Ruby
 objects interact and hold references to each other. First, a few principles:

 - Ruby's "TypedData" abstraction lets a Ruby VALUE hold a pointer to a C
   struct (or arbitrary memory chunk), own it, and free it when collected.
   Thus, each struct below will have a corresponding Ruby object
   wrapping/owning it.

 - To get back from an underlying upb {msg,enum}def to the Ruby object, we
   keep a global hashmap, accessed by get_def_obj/add_def_obj below.

 The in-memory structure is then something like:

   Ruby                        |      upb
                               |
   DescriptorPool  ------------|-----------> upb_symtab____________________
                               |                | (message types)          \
                               |                v                           \
   Descriptor   ---------------|-----------> upb_msgdef         (enum types)|
    |--> msgclass              |                |   ^                       |
    |    (dynamically built)   |                |   | (submsg fields)       |
    |--> MessageLayout         |                |   |                       /
    |--------------------------|> decoder method|   |                      /
    \--------------------------|> serialize     |   |                     /
                               |  handlers      v   |                    /
   FieldDescriptor  -----------|-----------> upb_fielddef               /
                               |                    |                  /
                               |                    v (enum fields)   /
   EnumDescriptor  ------------|-----------> upb_enumdef  <----------'
                               |
                               |
               ^               |               \___/
               `---------------|-----------------'    (get_def_obj map)
 */

// -----------------------------------------------------------------------------
// Ruby class structure definitions.
// -----------------------------------------------------------------------------

struct Descriptor {
  const upb_msgdef* msgdef;
  VALUE klass;
  VALUE descriptor_pool;
};

struct TypeInfo {
  upb_fieldtype_t type;
  union {
    const upb_msgdef* msgdef;  // When type == UPB_TYPE_MESSAGE
    const upb_enumdef* enumdef;    // When type == UPB_TYPE_ENUM
  } def;
};

extern VALUE cDescriptorPool;
extern VALUE cDescriptor;
extern VALUE cFileDescriptor;
extern VALUE cFieldDescriptor;
extern VALUE cEnumDescriptor;
extern VALUE cMessageBuilderContext;
extern VALUE cOneofBuilderContext;
extern VALUE cEnumBuilderContext;
extern VALUE cFileBuilderContext;
extern VALUE cBuilder;

extern VALUE cError;
extern VALUE cParseError;
extern VALUE cTypeError;

extern VALUE generated_pool;

static inline TypeInfo TypeInfo_get(const upb_fielddef *f) {
  TypeInfo ret = {upb_fielddef_type(f), {NULL}};
  switch (ret.type) {
    case UPB_TYPE_MESSAGE:
      ret.def.msgdef = upb_fielddef_msgsubdef(f);
      break;
    case UPB_TYPE_ENUM:
      ret.def.enumdef = upb_fielddef_enumsubdef(f);
      break;
    default:
      break;
  }
  return ret;
}

TypeInfo TypeInfo_FromClass(int argc, VALUE* argv, int skip_arg,
                            VALUE* type_class, VALUE* init_arg);

static inline TypeInfo TypeInfo_from_type(upb_fieldtype_t type) {
  TypeInfo ret = {type};
  assert(type != UPB_TYPE_MESSAGE && type != UPB_TYPE_ENUM);
  return ret;
}

// -----------------------------------------------------------------------------
// Native slot storage abstraction.
// -----------------------------------------------------------------------------

bool is_value_field(const upb_fielddef* f);

extern rb_encoding* kRubyStringUtf8Encoding;
extern rb_encoding* kRubyStringASCIIEncoding;
extern rb_encoding* kRubyString8bitEncoding;

VALUE field_type_class(const MessageLayout* layout, const upb_fielddef* field);

#define MAP_KEY_FIELD 1
#define MAP_VALUE_FIELD 2

// Oneof case slot value to indicate that no oneof case is set. The value `0` is
// safe because field numbers are used as case identifiers, and no field can
// have a number of 0.
#define ONEOF_CASE_NONE 0

// These operate on a map field (i.e., a repeated field of submessages whose
// submessage type is a map-entry msgdef).
bool is_map_field(const upb_fielddef* field);
const upb_fielddef* map_field_key(const upb_fielddef* field);
const upb_fielddef* map_field_value(const upb_fielddef* field);

// These operate on a map-entry msgdef.
const upb_fielddef* map_entry_key(const upb_msgdef* msgdef);
const upb_fielddef* map_entry_value(const upb_msgdef* msgdef);

// -----------------------------------------------------------------------------
// Repeated field container type.
// -----------------------------------------------------------------------------

bool is_wrapper_type_field(const upb_fielddef* field);
VALUE ruby_wrapper_type(VALUE type_class, VALUE value);

// -----------------------------------------------------------------------------
// Message class creation.
// -----------------------------------------------------------------------------

struct Message {
  VALUE arena;
  upb_msg* msg;
  const upb_msgdef* msgdef;      // kept alive by self.class.descriptor reference.
};

VALUE Arena_new();
upb_arena *Arena_get(VALUE arena);

extern rb_data_type_t Message_type;

VALUE build_class_from_descriptor(VALUE descriptor);

VALUE Google_Protobuf_discard_unknown(VALUE self, VALUE msg_rb);
VALUE Google_Protobuf_deep_copy(VALUE self, VALUE obj);

VALUE build_module_from_enumdesc(VALUE _enumdesc);
VALUE enum_lookup(VALUE self, VALUE number);
VALUE enum_resolve(VALUE self, VALUE sym);
VALUE enum_descriptor(VALUE self);

// Maximum depth allowed during encoding, to avoid stack overflows due to
// cycles.
#define ENCODE_MAX_NESTING 63

// -----------------------------------------------------------------------------
// A cache of frozen string objects to use as field defaults.
// -----------------------------------------------------------------------------
VALUE get_frozen_string(const char* data, size_t size, bool binary);

// -----------------------------------------------------------------------------
// Global map from upb {msg,enum}defs to wrapper Descriptor/EnumDescriptor
// instances.
// -----------------------------------------------------------------------------
VALUE get_msgdef_obj(VALUE descriptor_pool, const upb_msgdef* def);
VALUE get_enumdef_obj(VALUE descriptor_pool, const upb_enumdef* def);
VALUE get_fielddef_obj(VALUE descriptor_pool, const upb_fielddef* def);
VALUE get_filedef_obj(VALUE descriptor_pool, const upb_filedef* def);
VALUE get_oneofdef_obj(VALUE descriptor_pool, const upb_oneofdef* def);

// -----------------------------------------------------------------------------
// Global object cache from upb array/map/message/symtab to wrapper object.
// -----------------------------------------------------------------------------

// This is a conceptually "weak" cache, in that it does not prevent "val" from
// being collected.
//
// To prevent dangling references, the finalizer for "val" *must* call
// ObjectCache_Remove(). Only objects with such a finalizer are suitable to be
// stored in the cache.
void ObjectCache_Add(const void* key, VALUE val);
void ObjectCache_Remove(const void* key);

// Returns the cached object for this key, if any. Otherwise returns Qnil.
VALUE ObjectCache_Get(const void* key);

// -----------------------------------------------------------------------------
// StringBuilder, for inspect
// -----------------------------------------------------------------------------

struct StringBuilder;
typedef struct StringBuilder StringBuilder;

StringBuilder* StringBuilder_New();
void StringBuilder_Free(StringBuilder* b);
void StringBuilder_Printf(StringBuilder* b, const char *fmt, ...);
VALUE StringBuilder_ToRubyString(StringBuilder* b);

void StringBuilder_PrintMsgval(StringBuilder* b, upb_msgval val, TypeInfo info);

// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------

void check_upb_status(const upb_status* status, const char* msg);

#define CHECK_UPB(code, msg) do {                                             \
    upb_status status = UPB_STATUS_INIT;                                      \
    code;                                                                     \
    check_upb_status(&status, msg);                                           \
} while (0)

extern ID descriptor_instancevar_interned;

// A distinct object that is not accessible from Ruby.  We use this as a
// constructor argument to enforce that certain objects cannot be created from
// Ruby.
extern VALUE c_only_cookie;

#ifdef NDEBUG
#define PBRUBY_ASSERT(expr) do {} while (false && (expr))
#else
#define PBRUBY_ASSERT(expr) assert(expr)
#endif

#define UPB_UNUSED(var) (void)var

#endif  // __GOOGLE_PROTOBUF_RUBY_PROTOBUF_H__
