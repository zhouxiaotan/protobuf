// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/duration.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2fduration_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2fduration_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fduration_2eproto PROTOBUF_EXPORT
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct PROTOBUF_EXPORT TableStruct_google_2fprotobuf_2fduration_2eproto {
  static const uint32_t offsets[];
};
PROTOBUF_EXPORT extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fprotobuf_2fduration_2eproto;
PROTOBUF_NAMESPACE_OPEN
class Duration;
struct DurationDefaultTypeInternal;
PROTOBUF_EXPORT extern DurationDefaultTypeInternal _Duration_default_instance_;
PROTOBUF_NAMESPACE_CLOSE
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_EXPORT ::PROTOBUF_NAMESPACE_ID::Duration* Arena::CreateMaybeMessage<::PROTOBUF_NAMESPACE_ID::Duration>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
PROTOBUF_NAMESPACE_OPEN

// ===================================================================

class PROTOBUF_EXPORT Duration final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:google.protobuf.Duration) */ {
 public:
  inline Duration() : Duration(nullptr) {}
  ~Duration() override;
  explicit constexpr Duration(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Duration(const Duration& from);
  Duration(Duration&& from) noexcept
    : Duration() {
    *this = ::std::move(from);
  }

  inline Duration& operator=(const Duration& from) {
    CopyFrom(from);
    return *this;
  }
  inline Duration& operator=(Duration&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Duration& default_instance() {
    return *internal_default_instance();
  }
  static inline const Duration* internal_default_instance() {
    return reinterpret_cast<const Duration*>(
               &_Duration_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Duration& a, Duration& b) {
    a.Swap(&b);
  }
  inline void Swap(Duration* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Duration* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Duration* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Duration>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Duration& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Duration& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Duration* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "google.protobuf.Duration";
  }
  protected:
  explicit Duration(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kSecondsFieldNumber = 1,
    kNanosFieldNumber = 2,
  };
  // int64 seconds = 1;
  void clear_seconds();
  int64_t seconds() const;
  void set_seconds(int64_t value);
  private:
  int64_t _internal_seconds() const;
  void _internal_set_seconds(int64_t value);
  public:

  // int32 nanos = 2;
  void clear_nanos();
  int32_t nanos() const;
  void set_nanos(int32_t value);
  private:
  int32_t _internal_nanos() const;
  void _internal_set_nanos(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:google.protobuf.Duration)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int64_t seconds_;
  int32_t nanos_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_google_2fprotobuf_2fduration_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Duration

// int64 seconds = 1;
inline void Duration::clear_seconds() {
  seconds_ = int64_t{0};
}
inline int64_t Duration::_internal_seconds() const {
  return seconds_;
}
inline int64_t Duration::seconds() const {
  // @@protoc_insertion_point(field_get:google.protobuf.Duration.seconds)
  return _internal_seconds();
}
inline void Duration::_internal_set_seconds(int64_t value) {
  
  seconds_ = value;
}
inline void Duration::set_seconds(int64_t value) {
  _internal_set_seconds(value);
  // @@protoc_insertion_point(field_set:google.protobuf.Duration.seconds)
}

// int32 nanos = 2;
inline void Duration::clear_nanos() {
  nanos_ = 0;
}
inline int32_t Duration::_internal_nanos() const {
  return nanos_;
}
inline int32_t Duration::nanos() const {
  // @@protoc_insertion_point(field_get:google.protobuf.Duration.nanos)
  return _internal_nanos();
}
inline void Duration::_internal_set_nanos(int32_t value) {
  
  nanos_ = value;
}
inline void Duration::set_nanos(int32_t value) {
  _internal_set_nanos(value);
  // @@protoc_insertion_point(field_set:google.protobuf.Duration.nanos)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2fduration_2eproto
