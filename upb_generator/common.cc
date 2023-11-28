// Protocol Buffers - Google's data interchange format
// Copyright 2023 Google LLC.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "upb_generator/common.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include <string>

#include "absl/strings/ascii.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/string_view.h"
#include "absl/strings/substitute.h"
#include "upb/mini_table/field.h"
#include "upb/mini_table/internal/field.h"
#include "upb/reflection/def.hpp"
#include "upb_generator/mangle.h"

// Must be last
#include "upb/port/def.inc"

namespace upb {
namespace generator {

std::string StripExtension(absl::string_view fname) {
  size_t lastdot = fname.find_last_of('.');
  if (lastdot == std::string::npos) {
    return std::string(fname);
  }
  return std::string(fname.substr(0, lastdot));
}

std::string ToCIdent(absl::string_view str) {
  return absl::StrReplaceAll(str, {{".", "_"}, {"/", "_"}, {"-", "_"}});
}

std::string ToPreproc(absl::string_view str) {
  return absl::AsciiStrToUpper(ToCIdent(str));
}

void EmitFileWarning(absl::string_view name, Output& output) {
  output(
      "/* This file was generated by upb_generator from the input file:\n"
      " *\n"
      " *     $0\n"
      " *\n"
      " * Do not edit -- your changes will be discarded when the file is\n"
      " * regenerated. */\n\n",
      name);
}

std::string MessageInitName(upb::MessageDefPtr descriptor) {
  return MessageInit(descriptor.full_name());
}

void EmitDllExportMacros(absl::string_view dllexport_decl, Output& output) {
  if (dllexport_decl.length()) {
    output(
        "#if !defined $0\n"
        "#if defined $0_EXPORT\n"
        "#define $0 __declspec(dll_export)\n"
        "#elif defined $0_IMPORT\n"
        "#define $0 __declspec(dll_import)\n"
        "#else\n"
        "#define $0\n"
        "#endif\n"
        "#endif\n\n",
        dllexport_decl);    
  }
}

std::string PadSuffix(absl::string_view tag) {
  return tag.length() ? absl::StrCat(tag, " ") : std::string(tag);
}

std::string MessageName(upb::MessageDefPtr descriptor) {
  return ToCIdent(descriptor.full_name());
}

std::string FileLayoutName(upb::FileDefPtr file) {
  return ToCIdent(file.name()) + "_upb_file_layout";
}

std::string CApiHeaderFilename(upb::FileDefPtr file) {
  return StripExtension(file.name()) + ".upb.h";
}

std::string MiniTableHeaderFilename(upb::FileDefPtr file) {
  return StripExtension(file.name()) + ".upb_minitable.h";
}

std::string EnumInit(upb::EnumDefPtr descriptor) {
  return ToCIdent(descriptor.full_name()) + "_enum_init";
}

std::string FieldInitializer(upb::FieldDefPtr field,
                             const upb_MiniTableField* field64,
                             const upb_MiniTableField* field32) {
  return absl::Substitute(
      "{$0, $1, $2, $3, $4, $5}", upb_MiniTableField_Number(field64),
      ArchDependentSize(field32->offset, field64->offset),
      ArchDependentSize(field32->presence, field64->presence),
      field64->UPB_PRIVATE(submsg_index) == kUpb_NoSub
          ? "kUpb_NoSub"
          : absl::StrCat(field64->UPB_PRIVATE(submsg_index)).c_str(),
      field64->UPB_PRIVATE(descriptortype), GetModeInit(field32, field64));
}

std::string ArchDependentSize(int64_t size32, int64_t size64) {
  if (size32 == size64) return absl::StrCat(size32);
  return absl::Substitute("UPB_SIZE($0, $1)", size32, size64);
}

// Returns the field mode as a string initializer.
//
// We could just emit this as a number (and we may yet go in that direction) but
// for now emitting symbolic constants gives this better readability and
// debuggability.
std::string GetModeInit(const upb_MiniTableField* field32,
                        const upb_MiniTableField* field64) {
  std::string ret;
  uint8_t mode32 = field32->UPB_PRIVATE(mode);
  switch (mode32 & kUpb_FieldMode_Mask) {
    case kUpb_FieldMode_Map:
      ret = "(int)kUpb_FieldMode_Map";
      break;
    case kUpb_FieldMode_Array:
      ret = "(int)kUpb_FieldMode_Array";
      break;
    case kUpb_FieldMode_Scalar:
      ret = "(int)kUpb_FieldMode_Scalar";
      break;
    default:
      break;
  }

  if (mode32 & kUpb_LabelFlags_IsPacked) {
    absl::StrAppend(&ret, " | (int)kUpb_LabelFlags_IsPacked");
  }

  if (mode32 & kUpb_LabelFlags_IsExtension) {
    absl::StrAppend(&ret, " | (int)kUpb_LabelFlags_IsExtension");
  }

  if (mode32 & kUpb_LabelFlags_IsAlternate) {
    absl::StrAppend(&ret, " | (int)kUpb_LabelFlags_IsAlternate");
  }

  absl::StrAppend(&ret, " | ((int)", GetFieldRep(field32, field64),
                  " << kUpb_FieldRep_Shift)");
  return ret;
}

std::string GetFieldRep(const upb_MiniTableField* field32,
                        const upb_MiniTableField* field64) {
  const auto rep32 = UPB_PRIVATE(_upb_MiniTableField_GetRep)(field32);
  const auto rep64 = UPB_PRIVATE(_upb_MiniTableField_GetRep)(field64);

  switch (rep32) {
    case kUpb_FieldRep_1Byte:
      return "kUpb_FieldRep_1Byte";
      break;
    case kUpb_FieldRep_4Byte: {
      if (rep64 == kUpb_FieldRep_4Byte) {
        return "kUpb_FieldRep_4Byte";
      } else {
        assert(rep64 == kUpb_FieldRep_8Byte);
        return "UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte)";
      }
      break;
    }
    case kUpb_FieldRep_StringView:
      return "kUpb_FieldRep_StringView";
      break;
    case kUpb_FieldRep_8Byte:
      return "kUpb_FieldRep_8Byte";
      break;
  }
  UPB_UNREACHABLE();
}

}  // namespace generator
}  // namespace upb
