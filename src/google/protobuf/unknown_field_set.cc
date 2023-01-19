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

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include "google/protobuf/unknown_field_set.h"

#include <optional>
#include <utility>

#include "google/protobuf/stubs/logging.h"
#include "absl/strings/cord.h"
#include "absl/strings/internal/resize_uninitialized.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_tctable_impl.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/parse_context.h"
#include "google/protobuf/wire_format.h"
#include "google/protobuf/wire_format_lite.h"

// Must be included last.
#include "google/protobuf/port_def.inc"

namespace google {
namespace protobuf {

const UnknownFieldSet& UnknownFieldSet::default_instance() {
  static auto instance = internal::OnShutdownDelete(new UnknownFieldSet());
  return *instance;
}

void UnknownFieldSet::ClearFallback() {
  GOOGLE_ABSL_DCHECK(!fields_.empty());
  int n = fields_.size();
  do {
    (fields_)[--n].Delete();
  } while (n > 0);
  fields_.clear();
}

void UnknownFieldSet::InternalMergeFrom(const UnknownFieldSet& other) {
  int other_field_count = other.field_count();
  if (other_field_count > 0) {
    fields_.reserve(fields_.size() + other_field_count);
    for (int i = 0; i < other_field_count; i++) {
      fields_.push_back((other.fields_)[i].DeepCopy());
    }
  }
}

void UnknownFieldSet::MergeFrom(const UnknownFieldSet& other) {
  int other_field_count = other.field_count();
  if (other_field_count > 0) {
    fields_.reserve(fields_.size() + other_field_count);
    for (int i = 0; i < other_field_count; i++) {
      fields_.push_back((other.fields_)[i].DeepCopy());
    }
  }
}

// A specialized MergeFrom for performance when we are merging from an UFS that
// is temporary and can be destroyed in the process.
void UnknownFieldSet::MergeFromAndDestroy(UnknownFieldSet* other) {
  if (fields_.empty()) {
    fields_ = std::move(other->fields_);
  } else {
    fields_.insert(fields_.end(),
                   std::make_move_iterator(other->fields_.begin()),
                   std::make_move_iterator(other->fields_.end()));
  }
  other->fields_.clear();
}

void UnknownFieldSet::MergeToInternalMetadata(
    const UnknownFieldSet& other, internal::InternalMetadata* metadata) {
  metadata->mutable_unknown_fields<UnknownFieldSet>()->MergeFrom(other);
}

size_t UnknownField::SpaceUsedExcludingSelfLong() const {
  switch (type()) {
    case TYPE_LENGTH_DELIMITED:
      if (auto cord = GetLengthDelimitedCord()) {
        return sizeof(*cord) + cord->EstimatedMemoryUsage();
      }
      GOOGLE_ABSL_DCHECK_EQ(GetLengthDelimitedDataType(), DATATYPE_STRING);
      return sizeof(*data_.string_value_) +
             internal::StringSpaceUsedExcludingSelfLong(*data_.string_value_);
    case UnknownField::TYPE_GROUP:
      return data_.group_->SpaceUsedLong();
    default:
      return 0;
  }
}

size_t UnknownFieldSet::SpaceUsedExcludingSelfLong() const {
  if (fields_.empty()) return 0;

  size_t total_size = sizeof(UnknownField) * fields_.capacity();
  for (const UnknownField& field : fields_) {
    total_size += field.SpaceUsedExcludingSelfLong();
  }
  return total_size;
}

size_t UnknownFieldSet::SpaceUsedLong() const {
  return sizeof(*this) + SpaceUsedExcludingSelf();
}

void UnknownFieldSet::AddVarint(int number, uint64_t value) {
  fields_.emplace_back();
  auto& field = fields_.back();
  field.number_ = number;
  field.SetType(UnknownField::TYPE_VARINT);
  field.data_.varint_ = value;
}

void UnknownFieldSet::AddFixed32(int number, uint32_t value) {
  fields_.emplace_back();
  auto& field = fields_.back();
  field.number_ = number;
  field.SetType(UnknownField::TYPE_FIXED32);
  field.data_.fixed32_ = value;
}

void UnknownFieldSet::AddFixed64(int number, uint64_t value) {
  fields_.emplace_back();
  auto& field = fields_.back();
  field.number_ = number;
  field.SetType(UnknownField::TYPE_FIXED64);
  field.data_.fixed64_ = value;
}

std::string* UnknownFieldSet::AddLengthDelimited(int number) {
  fields_.emplace_back();
  auto& field = fields_.back();
  field.number_ = number;
  field.SetLengthDelimitedType(UnknownField::DATATYPE_STRING);
  field.data_.string_value_ = new std::string;
  return field.data_.string_value_;
}

absl::Cord* UnknownFieldSet::AddLengthDelimitedCord(int number) {
  fields_.emplace_back();
  auto& field = fields_.back();
  field.number_ = number;
  field.SetLengthDelimitedType(UnknownField::DATATYPE_CORD);
  field.data_.cord_value_ = new absl::Cord();
  return field.data_.cord_value_;
}

UnknownFieldSet* UnknownFieldSet::AddGroup(int number) {
  fields_.emplace_back();
  auto& field = fields_.back();
  field.number_ = number;
  field.SetType(UnknownField::TYPE_GROUP);
  field.data_.group_ = new UnknownFieldSet;
  return field.data_.group_;
}

void UnknownFieldSet::AddField(const UnknownField& field) {
  fields_.push_back(field.DeepCopy());
}

void UnknownFieldSet::DeleteSubrange(int start, int num) {
  // Delete the specified fields.
  for (int i = 0; i < num; ++i) {
    (fields_)[i + start].Delete();
  }
  // Slide down the remaining fields.
  for (size_t i = start + num; i < fields_.size(); ++i) {
    (fields_)[i - num] = (fields_)[i];
  }
  // Pop off the # of deleted fields.
  for (int i = 0; i < num; ++i) {
    fields_.pop_back();
  }
}

void UnknownFieldSet::DeleteByNumber(int number) {
  size_t left = 0;  // The number of fields left after deletion.
  for (size_t i = 0; i < fields_.size(); ++i) {
    UnknownField* field = &(fields_)[i];
    if (field->number() == number) {
      field->Delete();
    } else {
      if (i != left) {
        (fields_)[left] = (fields_)[i];
      }
      ++left;
    }
  }
  fields_.resize(left);
}

bool UnknownFieldSet::MergeFromCodedStream(io::CodedInputStream* input) {
  UnknownFieldSet other;
  if (internal::WireFormat::SkipMessage(input, &other) &&
      input->ConsumedEntireMessage()) {
    MergeFromAndDestroy(&other);
    return true;
  } else {
    return false;
  }
}

bool UnknownFieldSet::ParseFromCodedStream(io::CodedInputStream* input) {
  Clear();
  return MergeFromCodedStream(input);
}

bool UnknownFieldSet::ParseFromZeroCopyStream(io::ZeroCopyInputStream* input) {
  io::CodedInputStream coded_input(input);
  return (ParseFromCodedStream(&coded_input) &&
          coded_input.ConsumedEntireMessage());
}

bool UnknownFieldSet::ParseFromArray(const void* data, int size) {
  io::ArrayInputStream input(data, size);
  return ParseFromZeroCopyStream(&input);
}

bool UnknownFieldSet::SerializeToString(std::string* output) const {
  const size_t size =
      google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(*this);
  absl::strings_internal::STLStringResizeUninitializedAmortized(output, size);
  google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
      *this, reinterpret_cast<uint8_t*>(const_cast<char*>(output->data())));
  return true;
}

bool UnknownFieldSet::SerializeToCodedStream(
    io::CodedOutputStream* output) const {
  google::protobuf::internal::WireFormat::SerializeUnknownFields(*this, output);
  return !output->HadError();
}

bool UnknownFieldSet::SerializeToCord(absl::Cord* output) const {
  const size_t size =
      google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(*this);
  io::CordOutputStream cord_output_stream(size);
  {
    io::CodedOutputStream coded_output_stream(&cord_output_stream);
    if (!SerializeToCodedStream(&coded_output_stream)) return false;
  }
  *output = cord_output_stream.Consume();
  return true;
}

void UnknownField::Delete() {
  switch (type()) {
    case UnknownField::TYPE_LENGTH_DELIMITED:
      if (GetLengthDelimitedDataType() == DATATYPE_STRING) {
        delete data_.string_value_;
      } else {
        GOOGLE_ABSL_DCHECK_EQ(GetLengthDelimitedDataType(), DATATYPE_CORD);
        delete data_.cord_value_;
      }
      break;
    case UnknownField::TYPE_GROUP:
      delete data_.group_;
      break;
    default:
      break;
  }
}

UnknownField UnknownField::DeepCopy() const {
  UnknownField field = *this;
  switch (type()) {
    case TYPE_LENGTH_DELIMITED:
      if (auto cord = GetLengthDelimitedCord()) {
        field.datatype_.store(DATATYPE_CORD, std::memory_order_relaxed);
        field.data_.cord_value_ = new absl::Cord(std::move(*cord));
      } else {
        GOOGLE_ABSL_DCHECK_EQ(GetLengthDelimitedDataType(), DATATYPE_STRING);
        field.datatype_.store(DATATYPE_STRING, std::memory_order_relaxed);
        field.data_.string_value_ = new std::string(*data_.string_value_);
      }
      break;

    case UnknownField::TYPE_GROUP:
      field.data_.group_ = new UnknownFieldSet();
      field.data_.group_->InternalMergeFrom(*data_.group_);
      break;

    default:
      break;
  }
  return field;
}

void UnknownField::EnsureConvertedToStringSlow() const {
  absl::MutexLock lock(&datatype_mutex_);
  if (GetLengthDelimitedDataType() == DATATYPE_CORD) {
    std::string* string_value = new std::string();
    data_.cord_value_->AppendTo(string_value);
    delete data_.cord_value_;
    data_.string_value_ = string_value;
    datatype_.store(DATATYPE_STRING, std::memory_order_release);
  }
  GOOGLE_ABSL_DCHECK_EQ(GetLengthDelimitedDataType(), DATATYPE_STRING);
}

uint8_t* UnknownField::InternalSerializeLengthDelimitedNoTag(
    uint8_t* target, io::EpsCopyOutputStream* stream) const {
  GOOGLE_ABSL_DCHECK_EQ(TYPE_LENGTH_DELIMITED, type());
  if (auto cord = GetLengthDelimitedCord()) {
    target = io::CodedOutputStream::WriteVarint32ToArray(cord->size(), target);
    target = stream->WriteCord(*cord, target);
    return target;
  }
  const std::string& data = *data_.string_value_;
  target = io::CodedOutputStream::WriteVarint32ToArray(data.size(), target);
  target = stream->WriteRaw(data.data(), data.size(), target);
  return target;
}

std::optional<absl::Cord> UnknownField::GetLengthDelimitedCord() const {
  if (GetLengthDelimitedDataType() == DATATYPE_CORD) {
    absl::MutexLock lock(&datatype_mutex_);
    if (GetLengthDelimitedDataType() == DATATYPE_CORD) {
      return *data_.cord_value_;
    }
  }
  return std::nullopt;
}

namespace internal {

class UnknownFieldParserHelper {
 public:
  explicit UnknownFieldParserHelper(UnknownFieldSet* unknown)
      : unknown_(unknown) {}

  void AddVarint(uint32_t num, uint64_t value) {
    unknown_->AddVarint(num, value);
  }
  void AddFixed64(uint32_t num, uint64_t value) {
    unknown_->AddFixed64(num, value);
  }
  const char* ParseLengthDelimited(uint32_t num, const char* ptr,
                                   ParseContext* ctx) {
    std::string* s = unknown_->AddLengthDelimited(num);
    int size = ReadSize(&ptr);
    GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
    return ctx->ReadString(ptr, size, s);
  }
  const char* ParseGroup(uint32_t num, const char* ptr, ParseContext* ctx) {
    UnknownFieldParserHelper child(unknown_->AddGroup(num));
    return ctx->ParseGroup(&child, ptr, num * 8 + 3);
  }
  void AddFixed32(uint32_t num, uint32_t value) {
    unknown_->AddFixed32(num, value);
  }

  const char* _InternalParse(const char* ptr, ParseContext* ctx) {
    return WireFormatParser(*this, ptr, ctx);
  }

 private:
  UnknownFieldSet* unknown_;
};

const char* UnknownGroupParse(UnknownFieldSet* unknown, const char* ptr,
                              ParseContext* ctx) {
  UnknownFieldParserHelper field_parser(unknown);
  return WireFormatParser(field_parser, ptr, ctx);
}

const char* UnknownFieldParse(uint64_t tag, UnknownFieldSet* unknown,
                              const char* ptr, ParseContext* ctx) {
  UnknownFieldParserHelper field_parser(unknown);
  return FieldParser(tag, field_parser, ptr, ctx);
}

}  // namespace internal
}  // namespace protobuf
}  // namespace google

#include "google/protobuf/port_undef.inc"
