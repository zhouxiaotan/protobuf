// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/field_mask.proto

#include "google/protobuf/field_mask.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = ::PROTOBUF_NAMESPACE_ID::internal;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_CONSTEXPR FieldMask::FieldMask(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.paths_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct FieldMaskDefaultTypeInternal {
  PROTOBUF_CONSTEXPR FieldMaskDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~FieldMaskDefaultTypeInternal() {}
  union {
    FieldMask _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 FieldMaskDefaultTypeInternal _FieldMask_default_instance_;
PROTOBUF_NAMESPACE_CLOSE
static ::_pb::Metadata file_level_metadata_google_2fprotobuf_2ffield_5fmask_2eproto[1];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_google_2fprotobuf_2ffield_5fmask_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_google_2fprotobuf_2ffield_5fmask_2eproto = nullptr;
const ::uint32_t TableStruct_google_2fprotobuf_2ffield_5fmask_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(
    protodesc_cold) = {
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::PROTOBUF_NAMESPACE_ID::FieldMask, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::PROTOBUF_NAMESPACE_ID::FieldMask, _impl_.paths_),
};

static const ::_pbi::MigrationSchema
    schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
        { 0, -1, -1, sizeof(::PROTOBUF_NAMESPACE_ID::FieldMask)},
};

static const ::_pb::Message* const file_default_instances[] = {
    &::PROTOBUF_NAMESPACE_ID::_FieldMask_default_instance_._instance,
};
const char descriptor_table_protodef_google_2fprotobuf_2ffield_5fmask_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n google/protobuf/field_mask.proto\022\017goog"
    "le.protobuf\"\032\n\tFieldMask\022\r\n\005paths\030\001 \003(\tB"
    "\205\001\n\023com.google.protobufB\016FieldMaskProtoP"
    "\001Z2google.golang.org/protobuf/types/know"
    "n/fieldmaskpb\370\001\001\242\002\003GPB\252\002\036Google.Protobuf"
    ".WellKnownTypesb\006proto3"
};
static ::absl::once_flag descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto = {
    false,
    false,
    223,
    descriptor_table_protodef_google_2fprotobuf_2ffield_5fmask_2eproto,
    "google/protobuf/field_mask.proto",
    &descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto_once,
    nullptr,
    0,
    1,
    schemas,
    file_default_instances,
    TableStruct_google_2fprotobuf_2ffield_5fmask_2eproto::offsets,
    file_level_metadata_google_2fprotobuf_2ffield_5fmask_2eproto,
    file_level_enum_descriptors_google_2fprotobuf_2ffield_5fmask_2eproto,
    file_level_service_descriptors_google_2fprotobuf_2ffield_5fmask_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto_getter() {
  return &descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto;
}
// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_google_2fprotobuf_2ffield_5fmask_2eproto(&descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto);
PROTOBUF_NAMESPACE_OPEN
// ===================================================================

class FieldMask::_Internal {
 public:
};

FieldMask::FieldMask(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:google.protobuf.FieldMask)
}
FieldMask::FieldMask(const FieldMask& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  FieldMask* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.paths_){from._impl_.paths_}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:google.protobuf.FieldMask)
}

inline void FieldMask::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.paths_){arena}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

FieldMask::~FieldMask() {
  // @@protoc_insertion_point(destructor:google.protobuf.FieldMask)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void FieldMask::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.paths_.~RepeatedPtrField();
}

void FieldMask::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void FieldMask::Clear() {
// @@protoc_insertion_point(message_clear_start:google.protobuf.FieldMask)
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.paths_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* FieldMask::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {

#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // repeated string paths = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_paths();
            ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(ptr);
            CHK_(::_pbi::VerifyUTF8(str, "google.protobuf.FieldMask.paths"));
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else {
          goto handle_unusual;
        }
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

::uint8_t* FieldMask::_InternalSerialize(
    ::uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {

  // @@protoc_insertion_point(serialize_to_array_start:google.protobuf.FieldMask)
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated string paths = 1;
  for (int i = 0, n = this->_internal_paths_size(); i < n; i++) {
    const auto& s = this->_internal_paths(i);
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      s.data(), static_cast<int>(s.length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "google.protobuf.FieldMask.paths");
    target = stream->WriteString(1, s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:google.protobuf.FieldMask)
  return target;
}

::size_t FieldMask::ByteSizeLong() const {

// @@protoc_insertion_point(message_byte_size_start:google.protobuf.FieldMask)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated string paths = 1;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(_impl_.paths_.size());
  for (int i = 0, n = _impl_.paths_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      _impl_.paths_.Get(i));
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData FieldMask::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    FieldMask::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*FieldMask::GetClassData() const { return &_class_data_; }


void FieldMask::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<FieldMask*>(&to_msg);
  auto& from = static_cast<const FieldMask&>(from_msg);

  // @@protoc_insertion_point(class_specific_merge_from_start:google.protobuf.FieldMask)
  GOOGLE_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_impl_.paths_.MergeFrom(from._impl_.paths_);
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void FieldMask::CopyFrom(const FieldMask& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:google.protobuf.FieldMask)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FieldMask::IsInitialized() const {
  return true;
}

void FieldMask::InternalSwap(FieldMask* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.paths_.InternalSwap(&other->_impl_.paths_);
}

::PROTOBUF_NAMESPACE_ID::Metadata FieldMask::GetMetadata() const {

  return ::_pbi::AssignDescriptors(
      &descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto_getter, &descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto_once,
      file_level_metadata_google_2fprotobuf_2ffield_5fmask_2eproto[0]);
}
// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_CLOSE
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::PROTOBUF_NAMESPACE_ID::FieldMask*
Arena::CreateMaybeMessage< ::PROTOBUF_NAMESPACE_ID::FieldMask >(Arena* arena) {
  return Arena::CreateMessageInternal< ::PROTOBUF_NAMESPACE_ID::FieldMask >(arena);
}
PROTOBUF_NAMESPACE_CLOSE
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"
