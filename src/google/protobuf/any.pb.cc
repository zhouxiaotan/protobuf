// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/any.proto

#include <google/protobuf/any.pb.h>

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

#if defined(__llvm__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wuninitialized"
#endif  // __llvm__
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_CONSTEXPR Any::Any(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.type_url_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.value_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_._any_metadata_)*/{&_impl_.type_url_, &_impl_.value_}} {}
struct AnyDefaultTypeInternal {
  PROTOBUF_CONSTEXPR AnyDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~AnyDefaultTypeInternal() {}
  union {
    Any _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 AnyDefaultTypeInternal _Any_default_instance_;
PROTOBUF_NAMESPACE_CLOSE
static ::_pb::Metadata file_level_metadata_google_2fprotobuf_2fany_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_google_2fprotobuf_2fany_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_google_2fprotobuf_2fany_2eproto = nullptr;

const uint32_t TableStruct_google_2fprotobuf_2fany_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::PROTOBUF_NAMESPACE_ID::Any, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  ~0u,  // no _split_
  ~0u,  // no sizeof(Split)
  PROTOBUF_FIELD_OFFSET(::PROTOBUF_NAMESPACE_ID::Any, _impl_.type_url_),
  PROTOBUF_FIELD_OFFSET(::PROTOBUF_NAMESPACE_ID::Any, _impl_.value_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::PROTOBUF_NAMESPACE_ID::Any)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::PROTOBUF_NAMESPACE_ID::_Any_default_instance_._instance,
};

const char descriptor_table_protodef_google_2fprotobuf_2fany_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\031google/protobuf/any.proto\022\017google.prot"
  "obuf\"&\n\003Any\022\020\n\010type_url\030\001 \001(\t\022\r\n\005value\030\002"
  " \001(\014Bv\n\023com.google.protobufB\010AnyProtoP\001Z"
  ",google.golang.org/protobuf/types/known/"
  "anypb\242\002\003GPB\252\002\036Google.Protobuf.WellKnownT"
  "ypesb\006proto3"
  ;
static ::absl::once_flag descriptor_table_google_2fprotobuf_2fany_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_google_2fprotobuf_2fany_2eproto = {
    false, false, 212, descriptor_table_protodef_google_2fprotobuf_2fany_2eproto,
    "google/protobuf/any.proto",
    &descriptor_table_google_2fprotobuf_2fany_2eproto_once, nullptr, 0, 1,
    schemas, file_default_instances, TableStruct_google_2fprotobuf_2fany_2eproto::offsets,
    file_level_metadata_google_2fprotobuf_2fany_2eproto, file_level_enum_descriptors_google_2fprotobuf_2fany_2eproto,
    file_level_service_descriptors_google_2fprotobuf_2fany_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_google_2fprotobuf_2fany_2eproto_getter() {
  return &descriptor_table_google_2fprotobuf_2fany_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_google_2fprotobuf_2fany_2eproto(&descriptor_table_google_2fprotobuf_2fany_2eproto);
PROTOBUF_NAMESPACE_OPEN

// ===================================================================

bool Any::GetAnyFieldDescriptors(
    const ::PROTOBUF_NAMESPACE_ID::Message& message,
    const ::PROTOBUF_NAMESPACE_ID::FieldDescriptor** type_url_field,
    const ::PROTOBUF_NAMESPACE_ID::FieldDescriptor** value_field) {
  return ::_pbi::GetAnyFieldDescriptors(
      message, type_url_field, value_field);
}
bool Any::ParseAnyTypeUrl(
    ::absl::string_view type_url,
    std::string* full_type_name) {
  return ::_pbi::ParseAnyTypeUrl(type_url, full_type_name);
}

class Any::_Internal {
 public:
};

Any::Any(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:google.protobuf.Any)
}
Any::Any(const Any& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Any* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.type_url_){}
    , decltype(_impl_.value_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , /*decltype(_impl_._any_metadata_)*/{&_impl_.type_url_, &_impl_.value_}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.type_url_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.type_url_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_type_url().empty()) {
    _this->_impl_.type_url_.Set(from._internal_type_url(), 
      _this->GetArenaForAllocation());
  }
  _impl_.value_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.value_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_value().empty()) {
    _this->_impl_.value_.Set(from._internal_value(), 
      _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:google.protobuf.Any)
}

inline void Any::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.type_url_){}
    , decltype(_impl_.value_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , /*decltype(_impl_._any_metadata_)*/{&_impl_.type_url_, &_impl_.value_}
  };
  _impl_.type_url_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.type_url_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.value_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.value_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

Any::~Any() {
  // @@protoc_insertion_point(destructor:google.protobuf.Any)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Any::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.type_url_.Destroy();
  _impl_.value_.Destroy();
  _impl_._any_metadata_.~AnyMetadata();
}

void Any::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Any::Clear() {
// @@protoc_insertion_point(message_clear_start:google.protobuf.Any)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.type_url_.ClearToEmpty();
  _impl_.value_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Any::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string type_url = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_type_url();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "google.protobuf.Any.type_url"));
        } else
          goto handle_unusual;
        continue;
      // bytes value = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_value();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Any::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:google.protobuf.Any)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string type_url = 1;
  if (!this->_internal_type_url().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_type_url().data(), static_cast<int>(this->_internal_type_url().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "google.protobuf.Any.type_url");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_type_url(), target);
  }

  // bytes value = 2;
  if (!this->_internal_value().empty()) {
    target = stream->WriteBytesMaybeAliased(
        2, this->_internal_value(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:google.protobuf.Any)
  return target;
}

size_t Any::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:google.protobuf.Any)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string type_url = 1;
  if (!this->_internal_type_url().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_type_url());
  }

  // bytes value = 2;
  if (!this->_internal_value().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_value());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Any::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Any::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Any::GetClassData() const { return &_class_data_; }


void Any::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Any*>(&to_msg);
  auto& from = static_cast<const Any&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:google.protobuf.Any)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_type_url().empty()) {
    _this->_internal_set_type_url(from._internal_type_url());
  }
  if (!from._internal_value().empty()) {
    _this->_internal_set_value(from._internal_value());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Any::CopyFrom(const Any& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:google.protobuf.Any)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Any::IsInitialized() const {
  return true;
}

void Any::InternalSwap(Any* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.type_url_, lhs_arena,
      &other->_impl_.type_url_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.value_, lhs_arena,
      &other->_impl_.value_, rhs_arena
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata Any::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_google_2fprotobuf_2fany_2eproto_getter, &descriptor_table_google_2fprotobuf_2fany_2eproto_once,
      file_level_metadata_google_2fprotobuf_2fany_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_CLOSE
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::PROTOBUF_NAMESPACE_ID::Any*
Arena::CreateMaybeMessage< ::PROTOBUF_NAMESPACE_ID::Any >(Arena* arena) {
  return Arena::CreateMessageInternal< ::PROTOBUF_NAMESPACE_ID::Any >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#if defined(__llvm__)
  #pragma clang diagnostic pop
#endif  // __llvm__
#include <google/protobuf/port_undef.inc>
