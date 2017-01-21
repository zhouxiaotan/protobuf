// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/any.proto

#ifndef PROTOBUF_google_2fprotobuf_2fany_2eproto__INCLUDED
#define PROTOBUF_google_2fprotobuf_2fany_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3001000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3001000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include <google/protobuf/any.h>
// @@protoc_insertion_point(includes)
namespace google {
namespace protobuf {
class Any;
class AnyDefaultTypeInternal;
LIBPROTOBUF_EXPORT extern AnyDefaultTypeInternal _Any_default_instance_;
}  // namespace protobuf
}  // namespace google

namespace google {
namespace protobuf {

// Internal implementation detail -- do not call these.
void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fany_2eproto();
void LIBPROTOBUF_EXPORT protobuf_InitDefaults_google_2fprotobuf_2fany_2eproto();
namespace protobuf_google_2fprotobuf_2fany_2eproto {
struct LIBPROTOBUF_EXPORT TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
};
}  // namespace protobuf_google_2fprotobuf_2fany_2eproto

// ===================================================================

class LIBPROTOBUF_EXPORT Any : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:google.protobuf.Any) */ {
 public:
  Any();
  virtual ~Any();

  Any(const Any& from);

  inline Any& operator=(const Any& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Any& default_instance();

  static inline const Any* internal_default_instance() {
    return reinterpret_cast<const Any*>(
               &_Any_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  // implements Any -----------------------------------------------

  void PackFrom(const ::google::protobuf::Message& message);
  void PackFrom(const ::google::protobuf::Message& message,
                const ::std::string& type_url_prefix);
  bool UnpackTo(::google::protobuf::Message* message) const;
  template<typename T> bool Is() const {
    return _any_metadata_.Is<T>();
  }

  void Swap(Any* other);

  // implements Message ----------------------------------------------

  inline Any* New() const PROTOBUF_FINAL { return New(NULL); }

  Any* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Any& from);
  void MergeFrom(const Any& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output)
      const PROTOBUF_FINAL {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Any* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string type_url = 1;
  void clear_type_url();
  static const int kTypeUrlFieldNumber = 1;
  const ::std::string& type_url() const;
  void set_type_url(const ::std::string& value);
  #if LANG_CXX11
  void set_type_url(::std::string&& value);
  #endif
  void set_type_url(const char* value);
  void set_type_url(const char* value, size_t size);
  ::std::string* mutable_type_url();
  ::std::string* release_type_url();
  void set_allocated_type_url(::std::string* type_url);

  // bytes value = 2;
  void clear_value();
  static const int kValueFieldNumber = 2;
  const ::std::string& value() const;
  void set_value(const ::std::string& value);
  #if LANG_CXX11
  void set_value(::std::string&& value);
  #endif
  void set_value(const char* value);
  void set_value(const void* value, size_t size);
  ::std::string* mutable_value();
  ::std::string* release_value();
  void set_allocated_value(::std::string* value);

  // @@protoc_insertion_point(class_scope:google.protobuf.Any)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr type_url_;
  ::google::protobuf::internal::ArenaStringPtr value_;
  mutable int _cached_size_;
  ::google::protobuf::internal::AnyMetadata _any_metadata_;
  friend void LIBPROTOBUF_EXPORT protobuf_InitDefaults_google_2fprotobuf_2fany_2eproto_impl();
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fany_2eproto_impl();
  friend struct LIBPROTOBUF_EXPORT protobuf_google_2fprotobuf_2fany_2eproto::TableStruct;
  friend const ::google::protobuf::uint32* protobuf_Offsets_google_2fprotobuf_2fany_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fany_2eproto();

};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Any

// string type_url = 1;
inline void Any::clear_type_url() {
  type_url_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Any::type_url() const {
  // @@protoc_insertion_point(field_get:google.protobuf.Any.type_url)
  return type_url_.GetNoArena();
}
inline void Any::set_type_url(const ::std::string& value) {
  
  type_url_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:google.protobuf.Any.type_url)
}
#if LANG_CXX11
inline void Any::set_type_url(::std::string&& value) {
  
  type_url_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:google.protobuf.Any.type_url)
}
#endif
inline void Any::set_type_url(const char* value) {
  
  type_url_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:google.protobuf.Any.type_url)
}
inline void Any::set_type_url(const char* value, size_t size) {
  
  type_url_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:google.protobuf.Any.type_url)
}
inline ::std::string* Any::mutable_type_url() {
  
  // @@protoc_insertion_point(field_mutable:google.protobuf.Any.type_url)
  return type_url_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Any::release_type_url() {
  // @@protoc_insertion_point(field_release:google.protobuf.Any.type_url)
  
  return type_url_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Any::set_allocated_type_url(::std::string* type_url) {
  if (type_url != NULL) {
    
  } else {
    
  }
  type_url_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), type_url);
  // @@protoc_insertion_point(field_set_allocated:google.protobuf.Any.type_url)
}

// bytes value = 2;
inline void Any::clear_value() {
  value_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Any::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.Any.value)
  return value_.GetNoArena();
}
inline void Any::set_value(const ::std::string& value) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:google.protobuf.Any.value)
}
#if LANG_CXX11
inline void Any::set_value(::std::string&& value) {
  
  value_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:google.protobuf.Any.value)
}
#endif
inline void Any::set_value(const char* value) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:google.protobuf.Any.value)
}
inline void Any::set_value(const void* value, size_t size) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:google.protobuf.Any.value)
}
inline ::std::string* Any::mutable_value() {
  
  // @@protoc_insertion_point(field_mutable:google.protobuf.Any.value)
  return value_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Any::release_value() {
  // @@protoc_insertion_point(field_release:google.protobuf.Any.value)
  
  return value_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Any::set_allocated_value(::std::string* value) {
  if (value != NULL) {
    
  } else {
    
  }
  value_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set_allocated:google.protobuf.Any.value)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_google_2fprotobuf_2fany_2eproto__INCLUDED
