// Protocol Buffers - Google's data interchange format
// Copyright 2023 Google LLC.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef PROTOBUF_COMPILER_HBP_GEN_REPEATED_FIELDS_H_
#define PROTOBUF_COMPILER_HBP_GEN_REPEATED_FIELDS_H_

#include "absl/strings/string_view.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/compiler/hpb/output.h"

namespace google::protobuf::hpb_generator {

namespace protobuf = ::google::protobuf;

void WriteRepeatedFieldUsingAccessors(const protobuf::FieldDescriptor* field,
                                      absl::string_view class_name,
                                      absl::string_view resolved_field_name,
                                      Output& output, bool read_only);

void WriteRepeatedFieldsInMessageHeader(const protobuf::Descriptor* desc,
                                        const protobuf::FieldDescriptor* field,
                                        absl::string_view resolved_field_name,
                                        absl::string_view resolved_upbc_name,
                                        Output& output);

void WriteRepeatedMessageAccessor(const protobuf::Descriptor* message,
                                  const protobuf::FieldDescriptor* field,
                                  absl::string_view resolved_field_name,
                                  absl::string_view class_name, Output& output);

void WriteRepeatedStringAccessor(const protobuf::Descriptor* message,
                                 const protobuf::FieldDescriptor* field,
                                 absl::string_view resolved_field_name,
                                 absl::string_view class_name, Output& output);

void WriteRepeatedScalarAccessor(const protobuf::Descriptor* message,
                                 const protobuf::FieldDescriptor* field,
                                 absl::string_view resolved_field_name,
                                 absl::string_view class_name, Output& output);

}  // namespace google::protobuf::hpb_generator

#endif  // PROTOBUF_COMPILER_HBP_GEN_REPEATED_FIELDS_H_
