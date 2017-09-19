// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/timestamp.proto

#ifndef PROTOBUF_google_2fprotobuf_2ftimestamp_2eproto__INCLUDED
#define PROTOBUF_google_2fprotobuf_2ftimestamp_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3004000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
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
// @@protoc_insertion_point(includes)
namespace google {
namespace protobuf {
class Timestamp;
class TimestampDefaultTypeInternal;
LIBPROTOBUF_EXPORT extern TimestampDefaultTypeInternal _Timestamp_default_instance_;
}  // namespace protobuf
}  // namespace google

namespace google {
namespace protobuf {

namespace protobuf_google_2fprotobuf_2ftimestamp_2eproto {
// Internal implementation detail -- do not call these.
struct LIBPROTOBUF_EXPORT TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static void InitDefaultsImpl();
};
void LIBPROTOBUF_EXPORT AddDescriptors();
void LIBPROTOBUF_EXPORT InitDefaults();
}  // namespace protobuf_google_2fprotobuf_2ftimestamp_2eproto

// ===================================================================

class LIBPROTOBUF_EXPORT Timestamp : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:google.protobuf.Timestamp) */ {
 public:
  Timestamp();
  virtual ~Timestamp();

  Timestamp(const Timestamp& from);

  inline Timestamp& operator=(const Timestamp& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Timestamp(Timestamp&& from) noexcept
    : Timestamp() {
    *this = ::std::move(from);
  }

  inline Timestamp& operator=(Timestamp&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline ::google::protobuf::Arena* GetArena() const PROTOBUF_FINAL {
    return GetArenaNoVirtual();
  }
  inline void* GetMaybeArenaPointer() const PROTOBUF_FINAL {
    return MaybeArenaPtr();
  }
  static const ::google::protobuf::Descriptor* descriptor();
  static const Timestamp& default_instance();

  static inline const Timestamp* internal_default_instance() {
    return reinterpret_cast<const Timestamp*>(
               &_Timestamp_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void UnsafeArenaSwap(Timestamp* other);
  void Swap(Timestamp* other);
  friend void swap(Timestamp& a, Timestamp& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Timestamp* New() const PROTOBUF_FINAL { return New(NULL); }

  Timestamp* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Timestamp& from);
  void MergeFrom(const Timestamp& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Timestamp* other);
  protected:
  explicit Timestamp(::google::protobuf::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::google::protobuf::Arena* arena);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // int64 seconds = 1;
  void clear_seconds();
  static const int kSecondsFieldNumber = 1;
  ::google::protobuf::int64 seconds() const;
  void set_seconds(::google::protobuf::int64 value);

  // int32 nanos = 2;
  void clear_nanos();
  static const int kNanosFieldNumber = 2;
  ::google::protobuf::int32 nanos() const;
  void set_nanos(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:google.protobuf.Timestamp)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  template <typename T> friend class ::google::protobuf::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::google::protobuf::int64 seconds_;
  ::google::protobuf::int32 nanos_;
  mutable int _cached_size_;
  friend struct protobuf_google_2fprotobuf_2ftimestamp_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#ifdef GNUC_PRAGMA_DIAGNOSTIC
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // GNUC_PRAGMA_DIAGNOSTIC
// Timestamp

// int64 seconds = 1;
inline void Timestamp::clear_seconds() {
  seconds_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 Timestamp::seconds() const {
  // @@protoc_insertion_point(field_get:google.protobuf.Timestamp.seconds)
  return seconds_;
}
inline void Timestamp::set_seconds(::google::protobuf::int64 value) {
  
  seconds_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.Timestamp.seconds)
}

// int32 nanos = 2;
inline void Timestamp::clear_nanos() {
  nanos_ = 0;
}
inline ::google::protobuf::int32 Timestamp::nanos() const {
  // @@protoc_insertion_point(field_get:google.protobuf.Timestamp.nanos)
  return nanos_;
}
inline void Timestamp::set_nanos(::google::protobuf::int32 value) {
  
  nanos_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.Timestamp.nanos)
}

#ifdef GNUC_PRAGMA_DIAGNOSTIC
  #pragma GCC diagnostic pop
#endif  // GNUC_PRAGMA_DIAGNOSTIC
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_google_2fprotobuf_2ftimestamp_2eproto__INCLUDED
