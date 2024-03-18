// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

// This file contains declarations needed in generated headers for messages
// that use tail-call table parsing. Everything in this file is for internal
// use only.

#ifndef GOOGLE_PROTOBUF_GENERATED_MESSAGE_TCTABLE_DECL_H__
#define GOOGLE_PROTOBUF_GENERATED_MESSAGE_TCTABLE_DECL_H__

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "absl/types/span.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/parse_context.h"

// Must come last:
#include "google/protobuf/port_def.inc"

namespace google {
namespace protobuf {
namespace internal {

// Additional information about this field:
struct TcFieldData {
  constexpr TcFieldData() : data(0) {}
  explicit constexpr TcFieldData(uint64_t data) : data(data) {}
  
  // Fast table entry constructor:
  constexpr TcFieldData(uint16_t coded_tag, uint8_t hasbit_idx, uint8_t aux_idx,
                        uint16_t offset)
      : data(uint64_t{offset} << 48 |      //
             uint64_t{aux_idx} << 24 |     //
             uint64_t{hasbit_idx} << 16 |  //
             uint64_t{coded_tag}) {}

  // Constructor to create an explicit 'uninitialized' instance.
  // This constructor can be used to pass an uninitialized `data` value to a
  // table driven parser function that does not use `data`. The purpose of this
  // is that it allows the compiler to reallocate and re-purpose the register
  // that is currently holding its value for other data. This reduces register
  // allocations inside the highly optimized varint parsing functions.
  //
  // Applications not using `data` use the `PROTOBUF_TC_PARAM_NO_DATA_DECL`
  // macro to declare the standard input arguments with no name for the `data`
  // argument. Callers then use the `PROTOBUF_TC_PARAM_NO_DATA_PASS` macro.
  //
  // Example:
  //   if (ptr == nullptr) {
  //      PROTOBUF_MUSTTAIL return Error(PROTOBUF_TC_PARAM_NO_DATA_PASS);
  //   }
  struct DefaultInit {};
  TcFieldData(DefaultInit) {}  // NOLINT(google-explicit-constructor)

  // Fields used in mini table parsing:
  //
  //     Bit:
  //     +-----------+-------------------+
  //     |63    ..     32|31     ..     0|
  //     +---------------+---------------+
  //     :   .   :   .   |===============| [32] tag() (decoded)
  //     |===============|   .   :   .   : [32] entry_offset()
  //     +-----------+-------------------+
  //     |63    ..     32|31     ..     0|
  //     +---------------+---------------+

  template <typename TagType = uint16_t>
  TagType coded_tag() const {
    return static_cast<TagType>(data);
  }
  uint8_t hasbit_idx() const { return static_cast<uint8_t>(data >> 16); }
  uint8_t aux_idx() const { return static_cast<uint8_t>(data >> 24); }
  uint16_t offset() const { return static_cast<uint16_t>(data >> 48); }

// Fields used in non-field entries
  //
  //     Bit:
  //     +-----------+-------------------+
  //     |63    ..     32|31     ..     0|
  //     +---------------+---------------+
  //     :   .   :   .   :   . 16|=======| [16] coded_tag()
  //     :   .   :   . 32|=======|   .   : [16] decoded_tag()
  //     :---.---:---.---:   .   :   .   : [32] (unused)
  //     +-----------+-------------------+
  //     |63    ..     32|31     ..     0|
  //     +---------------+---------------+

  uint16_t decoded_tag() const { return static_cast<uint16_t>(data >> 16); }

  // Fields used in mini table parsing:
  //
  //     Bit:
  //     +-----------+-------------------+
  //     |63    ..     32|31     ..     0|
  //     +---------------+---------------+
  //     :   .   :   .   |===============| [32] tag() (decoded)
  //     |===============|   .   :   .   : [32] entry_offset()
  //     +-----------+-------------------+
  //     |63    ..     32|31     ..     0|
  //     +---------------+---------------+

  uint32_t tag() const { return static_cast<uint32_t>(data); }
  uint32_t entry_offset() const { return static_cast<uint32_t>(data >> 32); }

  union {
    uint64_t data;
  };
};

struct TcParseTableBase;

// TailCallParseFunc is the function pointer type used in the tailcall table.
typedef const char* (*TailCallParseFunc)(PROTOBUF_TC_PARAM_DECL);

namespace field_layout {
struct Offset {
  uint32_t off;
};
}  // namespace field_layout

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma warning(push)
// TcParseTableBase is intentionally overaligned on 32 bit targets.
#pragma warning(disable : 4324)
#endif

struct FieldAuxDefaultMessage {};
struct FieldAuxEnumData {};

// Small type card used by mini parse to handle map entries.
// Map key/values are very limited, so we can encode the whole thing in a single
// byte.
class MapTypeCard {
 public:
  enum CppType { kBool, k32, k64, kString, kMessage };
  MapTypeCard() = default;
  constexpr MapTypeCard(WireFormatLite::WireType wiretype, CppType cpp_type,
                        bool is_zigzag_utf8, bool is_signed)
      : data_(static_cast<uint8_t>((static_cast<uint8_t>(wiretype) << 0) |
                                   (static_cast<uint8_t>(cpp_type) << 3) |
                                   (static_cast<uint8_t>(is_zigzag_utf8) << 6) |
                                   (static_cast<uint8_t>(is_signed) << 7))) {}

  WireFormatLite::WireType wiretype() const {
    return static_cast<WireFormatLite::WireType>((data_ >> 0) & 0x7);
  }

  CppType cpp_type() const { return static_cast<CppType>((data_ >> 3) & 0x7); }

  bool is_signed() const {
    ABSL_DCHECK(cpp_type() == CppType::k32 || cpp_type() == CppType::k64);
    return static_cast<bool>(data_ >> 7);
  }

  bool is_zigzag() const {
    ABSL_DCHECK(wiretype() == WireFormatLite::WIRETYPE_VARINT);
    ABSL_DCHECK(cpp_type() == CppType::k32 || cpp_type() == CppType::k64);
    return is_zigzag_utf8();
  }
  bool is_utf8() const {
    ABSL_DCHECK(wiretype() == WireFormatLite::WIRETYPE_LENGTH_DELIMITED);
    ABSL_DCHECK(cpp_type() == CppType::kString);
    return is_zigzag_utf8();
  }

 private:
  bool is_zigzag_utf8() const { return static_cast<bool>((data_ >> 6) & 0x1); }
  uint8_t data_;
};
static_assert(sizeof(MapTypeCard) == sizeof(uint8_t), "");

// Make the map entry type card for a specified field type.
constexpr MapTypeCard MakeMapTypeCard(WireFormatLite::FieldType type) {
  switch (type) {
    case WireFormatLite::TYPE_FLOAT:
      return {WireFormatLite::WIRETYPE_FIXED32, MapTypeCard::k32, false, true};
    case WireFormatLite::TYPE_FIXED32:
      return {WireFormatLite::WIRETYPE_FIXED32, MapTypeCard::k32, false, false};
    case WireFormatLite::TYPE_SFIXED32:
      return {WireFormatLite::WIRETYPE_FIXED32, MapTypeCard::k32, false, true};

    case WireFormatLite::TYPE_DOUBLE:
      return {WireFormatLite::WIRETYPE_FIXED64, MapTypeCard::k64, false, true};
    case WireFormatLite::TYPE_FIXED64:
      return {WireFormatLite::WIRETYPE_FIXED64, MapTypeCard::k64, false, false};
    case WireFormatLite::TYPE_SFIXED64:
      return {WireFormatLite::WIRETYPE_FIXED64, MapTypeCard::k64, false, true};

    case WireFormatLite::TYPE_BOOL:
      return {WireFormatLite::WIRETYPE_VARINT, MapTypeCard::kBool, false,
              false};

    case WireFormatLite::TYPE_ENUM:
      // Enum validation is handled via `value_is_validated_enum` below.
      return {WireFormatLite::WIRETYPE_VARINT, MapTypeCard::k32, false, true};
    case WireFormatLite::TYPE_INT32:
      return {WireFormatLite::WIRETYPE_VARINT, MapTypeCard::k32, false, true};
    case WireFormatLite::TYPE_UINT32:
      return {WireFormatLite::WIRETYPE_VARINT, MapTypeCard::k32, false, false};

    case WireFormatLite::TYPE_INT64:
      return {WireFormatLite::WIRETYPE_VARINT, MapTypeCard::k64, false, true};
    case WireFormatLite::TYPE_UINT64:
      return {WireFormatLite::WIRETYPE_VARINT, MapTypeCard::k64, false, false};

    case WireFormatLite::TYPE_SINT32:
      return {WireFormatLite::WIRETYPE_VARINT, MapTypeCard::k32, true, true};
    case WireFormatLite::TYPE_SINT64:
      return {WireFormatLite::WIRETYPE_VARINT, MapTypeCard::k64, true, true};

    case WireFormatLite::TYPE_STRING:
      return {WireFormatLite::WIRETYPE_LENGTH_DELIMITED, MapTypeCard::kString,
              true, false};
    case WireFormatLite::TYPE_BYTES:
      return {WireFormatLite::WIRETYPE_LENGTH_DELIMITED, MapTypeCard::kString,
              false, false};

    case WireFormatLite::TYPE_MESSAGE:
      return {WireFormatLite::WIRETYPE_LENGTH_DELIMITED, MapTypeCard::kMessage,
              false, false};

    case WireFormatLite::TYPE_GROUP:
    default:
      Unreachable();
  }
}

enum class MapNodeSizeInfoT : uint32_t;

// Aux entry for map fields.
struct MapAuxInfo {
  MapTypeCard key_type_card;
  MapTypeCard value_type_card;
  // When off, we fall back to table->fallback to handle the parse. An example
  // of this is for DynamicMessage.
  uint8_t is_supported : 1;
  // Determines if we are using LITE or the full runtime. When using the full
  // runtime we have to synchronize with reflection before accessing the map.
  uint8_t use_lite : 1;
  // If true UTF8 errors cause the parsing to fail.
  uint8_t fail_on_utf8_failure : 1;
  // If true UTF8 errors are logged, but they are accepted.
  uint8_t log_debug_utf8_failure : 1;
  // If true the next aux contains the enum validator.
  uint8_t value_is_validated_enum : 1;
  // Size information derived from the actual node type.
  MapNodeSizeInfoT node_size_info;
};
static_assert(sizeof(MapAuxInfo) <= 8, "");

// Base class for message-level table with info for the tail-call parser.
struct alignas(uint64_t) TcParseTableBase {
  // Common attributes for message layout:
  uint16_t has_bits_offset;
  uint16_t extension_offset;
  uint32_t max_field_number;
  uint32_t num_fast_fields;
  uint32_t lookup_table_offset;
  uint32_t skipmap32;
  uint32_t field_entries_offset;
  uint16_t num_field_entries;

  uint16_t num_aux_entries;
  uint32_t aux_offset;

  const MessageLite* default_instance;
  using PostLoopHandler = const char* (*)(MessageLite* msg, const char* ptr,
                                          ParseContext* ctx);
  PostLoopHandler post_loop_handler;

  // Handler for fields which are not handled by table dispatch.
  TailCallParseFunc fallback;

  // A sub message's table to be prefetched.
#ifdef PROTOBUF_PREFETCH_PARSE_TABLE
  const TcParseTableBase* to_prefetch;
#endif  // PROTOBUF_PREFETCH_PARSE_TABLE

  // This constructor exactly follows the field layout, so it's technically
  // not necessary.  However, it makes it much much easier to add or re-arrange
  // fields, because it can be overloaded with an additional constructor,
  // temporarily allowing both old and new protocol buffer headers to be
  // compiled.
  constexpr TcParseTableBase(
      uint16_t has_bits_offset, uint16_t extension_offset,
      uint32_t max_field_number, uint32_t num_fast_fields,
      uint32_t lookup_table_offset, uint32_t skipmap32,
      uint32_t field_entries_offset, uint16_t num_field_entries,
      uint16_t num_aux_entries, uint32_t aux_offset,
      const MessageLite* default_instance, PostLoopHandler post_loop_handler, TailCallParseFunc fallback
#ifdef PROTOBUF_PREFETCH_PARSE_TABLE
                            ,
                            const TcParseTableBase* to_prefetch
#endif  // PROTOBUF_PREFETCH_PARSE_TABLE
      )
      : has_bits_offset(has_bits_offset),
        extension_offset(extension_offset),
        max_field_number(max_field_number),
        num_fast_fields(num_fast_fields),
        lookup_table_offset(lookup_table_offset),
        skipmap32(skipmap32),
        field_entries_offset(field_entries_offset),
        num_field_entries(num_field_entries),
        num_aux_entries(num_aux_entries),
        aux_offset(aux_offset),
        default_instance(default_instance),
        post_loop_handler(post_loop_handler),
        fallback(fallback)
#ifdef PROTOBUF_PREFETCH_PARSE_TABLE
        ,
        to_prefetch(to_prefetch)
#endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  {
  }

  // Table entry for fast-path tailcall dispatch handling.
  struct FastFieldEntry {
    enum {
      // Cardinality
      kCardShift = 3,  // 3 bits wiretype
      kCardMask = 0x18,
      kSingular = 0,  // no hasbit
      kOptional = 0x8,
      kRepeated = 0x10,
      kFallback = 0x18,  // oneof or non-standard reps

      // Representation
      kRepShift = kCardShift + 2,
      kRepMask = 0x60,
      // Varint / fixed wiretype
      kRepBool = 0,
      kRep32Bit = 0x20,
      kRep64Bit = 0x40,
      // length-delimited wiretype
      kRepBytes = 0,
      kRepMessage  = 0x20,  // MessageLite*

      // Transforms
      kTransformShift = kRepShift + 2,
      kTransformMask = 0x180,
      // Varint
      kNoZigZag = 0,
      kZigZag = 0x80,
      // TODO (fast enum validation?)
      // Bytes/Strings
      kBytes = 0,
      kUtf8Debug = 0x80,
      kUtf8 = 0x100,

      // Hasbit
      kHasBitBits = 16,
      kHasBitMask = (1 << kHasBitBits) - 1,
      kHasBitShift = kTransformShift + 2,

      kOffsetShift = kHasBitShift + kHasBitBits,

      // Fallback stores in remaining bits
      kFallbackShift = kCardShift + 2,
    };

    uint64_t bits = 0;

    // Default initializes this instance with undefined values.
    constexpr FastFieldEntry() = default;

    constexpr FastFieldEntry(TailCallParseFunc func, TcFieldData bits) {}


    // Constant initializes this instance
    constexpr FastFieldEntry(uint64_t bits)
        : bits(bits) {}
    constexpr FastFieldEntry(uint64_t bits, uint64_t hasbit_idx, uint64_t offset)
        : bits(bits | (hasbit_idx << kHasBitShift) | (offset << kOffsetShift)) {}
  };

  // There is always at least one table entry.
  const FastFieldEntry* fast_entry(size_t idx) const {
    return reinterpret_cast<const FastFieldEntry*>(this + 1) + idx;
  }
  FastFieldEntry* fast_entry(size_t idx) {
    return reinterpret_cast<FastFieldEntry*>(this + 1) + idx;
  }

  // Returns a begin iterator (pointer) to the start of the field lookup table.
  const uint16_t* field_lookup_begin() const {
    return reinterpret_cast<const uint16_t*>(reinterpret_cast<uintptr_t>(this) +
                                             lookup_table_offset);
  }
  uint16_t* field_lookup_begin() {
    return reinterpret_cast<uint16_t*>(reinterpret_cast<uintptr_t>(this) +
                                       lookup_table_offset);
  }

  // Field entry for all fields.
  struct FieldEntry {
    constexpr FieldEntry() : type_card(0), has_idx(0), aux_idx(0), offset(0) {}
    constexpr FieldEntry(uint32_t o, int32_t h, uint16_t a, uint16_t t) : type_card(t), has_idx(h), aux_idx(a), offset(o) {
      // Prevent silent overflows
      if (o >= (1 << 18)) NotConstexpr();
      if (h >= (1 << 16)) NotConstexpr();
      if (h < -1) NotConstexpr();
      if (a >= (1 << 14)) NotConstexpr();
    }
    uint64_t type_card : 16;  // `FieldType` and `Cardinality` (see _impl.h)
    uint64_t has_idx : 16;     // has-bit index, relative to the message object
    uint64_t aux_idx : 14;    // index for `field_aux`.
    uint64_t offset : 18;     // offset in the message object

    static constexpr uint16_t kNoAuxIdx = 0xFFFF;
    static void NotConstexpr() {}
  };

  // Returns a begin iterator (pointer) to the start of the field entries array.
  const FieldEntry* field_entries_begin() const {
    return reinterpret_cast<const FieldEntry*>(
        reinterpret_cast<uintptr_t>(this) + field_entries_offset);
  }
  absl::Span<const FieldEntry> field_entries() const {
    return {field_entries_begin(), num_field_entries};
  }
  FieldEntry* field_entries_begin() {
    return reinterpret_cast<FieldEntry*>(reinterpret_cast<uintptr_t>(this) +
                                         field_entries_offset);
  }

  // Auxiliary entries for field types that need extra information.
  union FieldAux {
    constexpr FieldAux() : message_default_p(nullptr) {}
    constexpr FieldAux(FieldAuxEnumData, const uint32_t* enum_data)
        : enum_data(enum_data) {}
    constexpr FieldAux(field_layout::Offset off) : offset(off.off) {}
    constexpr FieldAux(int16_t range_start, uint16_t range_length)
        : enum_range{range_start, range_length} {}
    constexpr FieldAux(const MessageLite* msg) : message_default_p(msg) {}
    constexpr FieldAux(FieldAuxDefaultMessage, const void* msg)
        : message_default_p(msg) {}
    constexpr FieldAux(const TcParseTableBase* table) : table(table) {}
    constexpr FieldAux(MapAuxInfo map_info) : map_info(map_info) {}
    constexpr FieldAux(void (*create_in_arena)(Arena*, void*))
        : create_in_arena(create_in_arena) {}
    constexpr FieldAux(LazyEagerVerifyFnType verify_func)
        : verify_func(verify_func) {}
    struct {
      int16_t start;    // minimum enum number (if it fits)
      uint16_t length;  // length of range (i.e., max = start + length - 1)
    } enum_range;
    uint32_t offset;
    const void* message_default_p;
    const uint32_t* enum_data;
    const TcParseTableBase* table;
    MapAuxInfo map_info;
    void (*create_in_arena)(Arena*, void*);
    LazyEagerVerifyFnType verify_func;

    const MessageLite* message_default() const {
      return static_cast<const MessageLite*>(message_default_p);
    }
    const MessageLite* message_default_weak() const {
      return *static_cast<const MessageLite* const*>(message_default_p);
    }
  };
  const FieldAux* field_aux(uint32_t idx) const {
    return reinterpret_cast<const FieldAux*>(reinterpret_cast<uintptr_t>(this) +
                                             aux_offset) +
           idx;
  }
  FieldAux* field_aux(uint32_t idx) {
    return reinterpret_cast<FieldAux*>(reinterpret_cast<uintptr_t>(this) +
                                       aux_offset) +
           idx;
  }
  const FieldAux* field_aux(const FieldEntry* entry) const {
    return field_aux(entry->aux_idx);
  }

  // Field name data
  const char* name_data() const {
    return reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(this) +
                                         aux_offset +
                                         num_aux_entries * sizeof(FieldAux));
  }
  char* name_data() {
    return reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(this) +
                                   aux_offset +
                                   num_aux_entries * sizeof(FieldAux));
  }
};

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma warning(pop)
#endif

static_assert(sizeof(TcParseTableBase::FastFieldEntry) <= 16,
              "Fast field entry is too big.");
static_assert(sizeof(TcParseTableBase::FieldEntry) <= 16,
              "Field entry is too big.");

template <size_t kNumFastTable, size_t kNumFieldEntries = 0,
          size_t kNumFieldAux = 0, size_t kNameTableSize = 0,
          size_t kFieldLookupSize = 2>
struct TcParseTable {
  TcParseTableBase header;

  // Entries for each field.
  //
  // Fields are indexed by the lowest bits of their field number. The field
  // number is masked to fit inside the table. Note that the parsing logic
  // generally calls `TailCallParseTableBase::fast_entry()` instead of accessing
  // this field directly.
  std::array<TcParseTableBase::FastFieldEntry, kNumFastTable> fast_entries;

  // Just big enough to find all the field entries.
  std::array<uint16_t, kFieldLookupSize> field_lookup_table;
  // Entries for all fields:
  std::array<TcParseTableBase::FieldEntry, kNumFieldEntries> field_entries;
  std::array<TcParseTableBase::FieldAux, kNumFieldAux> aux_entries;
  std::array<char, kNameTableSize == 0 ? 1 : kNameTableSize> field_names;
};

// Partial specialization: if there are no aux entries, there will be no array.
// In C++, arrays cannot have length 0, but (C++11) std::array<T, 0> is valid.
// However, different implementations have different sizeof(std::array<T, 0>).
// Skipping the member makes offset computations portable.
template <size_t kNumFastTable, size_t kNumFieldEntries,
          size_t kNameTableSize, size_t kFieldLookupSize>
struct TcParseTable<kNumFastTable, kNumFieldEntries, 0, kNameTableSize,
                    kFieldLookupSize> {
  TcParseTableBase header;
  std::array<TcParseTableBase::FastFieldEntry, kNumFastTable>
      fast_entries;
  std::array<uint16_t, kFieldLookupSize> field_lookup_table;
  std::array<TcParseTableBase::FieldEntry, kNumFieldEntries> field_entries;
  std::array<char, kNameTableSize == 0 ? 1 : kNameTableSize> field_names;
};

// Partial specialization: if there are no fields at all, then we can save space
// by skipping the field numbers and entries.
template <size_t kNameTableSize, size_t kFieldLookupSize>
struct TcParseTable<0, 0, 0, kNameTableSize, kFieldLookupSize> {
  TcParseTableBase header;
  std::array<TcParseTableBase::FastFieldEntry, 0> fast_entries;
  std::array<uint16_t, kFieldLookupSize> field_lookup_table;
  std::array<char, kNameTableSize == 0 ? 1 : kNameTableSize> field_names;
};

static_assert(std::is_standard_layout<TcParseTable<1>>::value,
              "TcParseTable must be standard layout.");

static_assert(offsetof(TcParseTable<1>, fast_entries) ==
                  sizeof(TcParseTableBase),
              "Table entries must be laid out after TcParseTableBase.");

template <typename T, const char* (*func)(T*, const char*, ParseContext*)>
const char* StubParseImpl(PROTOBUF_TC_PARAM_DECL) {
  return func(static_cast<T*>(msg), ptr, ctx);
}

template <typename T, const char* (*func)(T*, const char*, ParseContext*)>
constexpr TcParseTable<0> CreateStubTcParseTable(
    const MessageLite* default_instance,
    TcParseTableBase::PostLoopHandler post_loop_handler = nullptr) {
  return {
      {
          0,                  // has_bits_offset
          0,                  // extension_offset
          0,                  // max_field_number
          0,                  // fast_idx_mask
          0,                  // lookup_table_offset
          0,                  // skipmap32
          0,                  // field_entries_offset
          0,                  // num_field_entries
          0,                  // num_aux_entries
          0,                  // aux_offset
          default_instance,   //
          post_loop_handler,  //
          nullptr,            // fallback
#ifdef PROTOBUF_PREFETCH_PARSE_TABLE
          nullptr,  // to_prefetch
#endif              // PROTOBUF_PREFETCH_PARSE_TABLE
      },
      {{{StubParseImpl<T, func>, {}}}},
  };
}

}  // namespace internal
}  // namespace protobuf
}  // namespace google

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_GENERATED_MESSAGE_TCTABLE_DECL_H__
