// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/field_mask.proto

#include <google/protobuf/field_mask.pb.h>

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_NAMESPACE_OPEN
class FieldMaskDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<FieldMask> _instance;
} _FieldMask_default_instance_;
PROTOBUF_NAMESPACE_CLOSE
static void InitDefaultsFieldMask_google_2fprotobuf_2ffield_5fmask_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &PROTOBUF_NAMESPACE_ID::_FieldMask_default_instance_;
    new (ptr) PROTOBUF_NAMESPACE_ID::FieldMask();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  PROTOBUF_NAMESPACE_ID::FieldMask::InitAsDefaultInstance();
}

PROTOBUF_EXPORT ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_FieldMask_google_2fprotobuf_2ffield_5fmask_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsFieldMask_google_2fprotobuf_2ffield_5fmask_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_google_2fprotobuf_2ffield_5fmask_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_google_2fprotobuf_2ffield_5fmask_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_google_2fprotobuf_2ffield_5fmask_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_google_2fprotobuf_2ffield_5fmask_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(PROTOBUF_NAMESPACE_ID::FieldMask, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(PROTOBUF_NAMESPACE_ID::FieldMask, paths_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(PROTOBUF_NAMESPACE_ID::FieldMask)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&PROTOBUF_NAMESPACE_ID::_FieldMask_default_instance_),
};

static ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptorsTable assign_descriptors_table_google_2fprotobuf_2ffield_5fmask_2eproto = {
  {}, AddDescriptors_google_2fprotobuf_2ffield_5fmask_2eproto, "google/protobuf/field_mask.proto", schemas,
  file_default_instances, TableStruct_google_2fprotobuf_2ffield_5fmask_2eproto::offsets,
  file_level_metadata_google_2fprotobuf_2ffield_5fmask_2eproto, 1, file_level_enum_descriptors_google_2fprotobuf_2ffield_5fmask_2eproto, file_level_service_descriptors_google_2fprotobuf_2ffield_5fmask_2eproto,
};

const char descriptor_table_protodef_google_2fprotobuf_2ffield_5fmask_2eproto[] =
  "\n google/protobuf/field_mask.proto\022\017goog"
  "le.protobuf\"\032\n\tFieldMask\022\r\n\005paths\030\001 \003(\tB"
  "\214\001\n\023com.google.protobufB\016FieldMaskProtoP"
  "\001Z9google.golang.org/genproto/protobuf/f"
  "ield_mask;field_mask\370\001\001\242\002\003GPB\252\002\036Google.P"
  "rotobuf.WellKnownTypesb\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto = {
  false, descriptor_table_protodef_google_2fprotobuf_2ffield_5fmask_2eproto,
  "google/protobuf/field_mask.proto", &assign_descriptors_table_google_2fprotobuf_2ffield_5fmask_2eproto, 230,
};

void AddDescriptors_google_2fprotobuf_2ffield_5fmask_2eproto() {
  static constexpr ::PROTOBUF_NAMESPACE_ID::internal::InitFunc deps[1] =
  {
  };
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_FieldMask_google_2fprotobuf_2ffield_5fmask_2eproto.base);
 ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_google_2fprotobuf_2ffield_5fmask_2eproto, deps, 0);
}

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_google_2fprotobuf_2ffield_5fmask_2eproto = []() { AddDescriptors_google_2fprotobuf_2ffield_5fmask_2eproto(); return true; }();
PROTOBUF_NAMESPACE_OPEN

// ===================================================================

void FieldMask::InitAsDefaultInstance() {
}
class FieldMask::HasBitSetters {
 public:
};

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int FieldMask::kPathsFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

FieldMask::FieldMask()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:google.protobuf.FieldMask)
}
FieldMask::FieldMask(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
  _internal_metadata_(arena),
  paths_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:google.protobuf.FieldMask)
}
FieldMask::FieldMask(const FieldMask& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      paths_(from.paths_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:google.protobuf.FieldMask)
}

void FieldMask::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(
      &scc_info_FieldMask_google_2fprotobuf_2ffield_5fmask_2eproto.base);
}

FieldMask::~FieldMask() {
  // @@protoc_insertion_point(destructor:google.protobuf.FieldMask)
  SharedDtor();
}

void FieldMask::SharedDtor() {
  GOOGLE_DCHECK(GetArenaNoVirtual() == nullptr);
}

void FieldMask::ArenaDtor(void* object) {
  FieldMask* _this = reinterpret_cast< FieldMask* >(object);
  (void)_this;
}
void FieldMask::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void FieldMask::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const FieldMask& FieldMask::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_FieldMask_google_2fprotobuf_2ffield_5fmask_2eproto.base);
  return *internal_default_instance();
}


void FieldMask::Clear() {
// @@protoc_insertion_point(message_clear_start:google.protobuf.FieldMask)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  paths_.Clear();
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* FieldMask::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArenaNoVirtual(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated string paths = 1;
      case 1: {
        if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) != 10) goto handle_unusual;
        while (true) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(add_paths(), ptr, ctx, "google.protobuf.FieldMask.paths");
          CHK_(ptr);
          if (!ctx->DataAvailable(ptr)) break;
          if (::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<::PROTOBUF_NAMESPACE_ID::uint8>(ptr) != 10) break;
          ptr += 1;
        }
        break;
      }
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        break;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  return nullptr;
#undef CHK_
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool FieldMask::MergePartialFromCodedStream(
    ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::PROTOBUF_NAMESPACE_ID::uint32 tag;
  // @@protoc_insertion_point(parse_start:google.protobuf.FieldMask)
  for (;;) {
    ::std::pair<::PROTOBUF_NAMESPACE_ID::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated string paths = 1;
      case 1: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (10 & 0xFF)) {
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadString(
                input, this->add_paths()));
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
            this->paths(this->paths_size() - 1).data(),
            static_cast<int>(this->paths(this->paths_size() - 1).length()),
            ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE,
            "google.protobuf.FieldMask.paths"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:google.protobuf.FieldMask)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:google.protobuf.FieldMask)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

void FieldMask::SerializeWithCachedSizes(
    ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:google.protobuf.FieldMask)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated string paths = 1;
  for (int i = 0, n = this->paths_size(); i < n; i++) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->paths(i).data(), static_cast<int>(this->paths(i).length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "google.protobuf.FieldMask.paths");
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteString(
      1, this->paths(i), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:google.protobuf.FieldMask)
}

::PROTOBUF_NAMESPACE_ID::uint8* FieldMask::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:google.protobuf.FieldMask)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated string paths = 1;
  for (int i = 0, n = this->paths_size(); i < n; i++) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->paths(i).data(), static_cast<int>(this->paths(i).length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "google.protobuf.FieldMask.paths");
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      WriteStringToArray(1, this->paths(i), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:google.protobuf.FieldMask)
  return target;
}

size_t FieldMask::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:google.protobuf.FieldMask)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated string paths = 1;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(this->paths_size());
  for (int i = 0, n = this->paths_size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      this->paths(i));
  }

  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void FieldMask::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:google.protobuf.FieldMask)
  GOOGLE_DCHECK_NE(&from, this);
  const FieldMask* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<FieldMask>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:google.protobuf.FieldMask)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:google.protobuf.FieldMask)
    MergeFrom(*source);
  }
}

void FieldMask::MergeFrom(const FieldMask& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:google.protobuf.FieldMask)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  paths_.MergeFrom(from.paths_);
}

void FieldMask::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:google.protobuf.FieldMask)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
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

void FieldMask::Swap(FieldMask* other) {
  if (other == this) return;
  if (GetArenaNoVirtual() == other->GetArenaNoVirtual()) {
    InternalSwap(other);
  } else {
    FieldMask* temp = New(GetArenaNoVirtual());
    temp->MergeFrom(*other);
    other->CopyFrom(*this);
    InternalSwap(temp);
    if (GetArenaNoVirtual() == nullptr) {
      delete temp;
    }
  }
}
void FieldMask::UnsafeArenaSwap(FieldMask* other) {
  if (other == this) return;
  GOOGLE_DCHECK(GetArenaNoVirtual() == other->GetArenaNoVirtual());
  InternalSwap(other);
}
void FieldMask::InternalSwap(FieldMask* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  paths_.InternalSwap(CastToBase(&other->paths_));
}

::PROTOBUF_NAMESPACE_ID::Metadata FieldMask::GetMetadata() const {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::assign_descriptors_table_google_2fprotobuf_2ffield_5fmask_2eproto);
  return ::file_level_metadata_google_2fprotobuf_2ffield_5fmask_2eproto[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_CLOSE
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE PROTOBUF_NAMESPACE_ID::FieldMask* Arena::CreateMaybeMessage< PROTOBUF_NAMESPACE_ID::FieldMask >(Arena* arena) {
  return Arena::CreateMessageInternal< PROTOBUF_NAMESPACE_ID::FieldMask >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
