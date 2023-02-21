// Generated by the protocol buffer compiler: libprotoc 21.12.  DO NOT EDIT!
// clang-format off
// source: google/protobuf/field_mask.proto

#import "GPBProtocolBuffers_RuntimeSupport.h"
#import "GPBFieldMask.pbobjc.h"

#if GOOGLE_PROTOBUF_OBJC_VERSION < 30007
#error This file was generated by a newer version of protoc which is incompatible with your Protocol Buffer library sources.
#endif
#if 30007 < GOOGLE_PROTOBUF_OBJC_MIN_SUPPORTED_VERSION
#error This file was generated by an older version of protoc which is incompatible with your Protocol Buffer library sources.
#endif

// @@protoc_insertion_point(imports)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"

#pragma mark - Objective-C Class declarations
// Forward declarations of Objective-C classes that we can use as
// static values in struct initializers.
// We don't use [Foo class] because it is not a static value.
GPBObjCClassDeclaration(GPBFieldMask);

#pragma mark - GPBFieldMaskRoot

@implementation GPBFieldMaskRoot

// No extensions in the file and no imports, so no need to generate
// +extensionRegistry.

@end

static GPBFileDescription GPBFieldMaskRoot_FileDescription = {
  .package = "google.protobuf",
  .prefix = "GPB",
  .syntax = GPBFileSyntaxProto3
};

#pragma mark - GPBFieldMask

@implementation GPBFieldMask

@dynamic pathsArray, pathsArray_Count;

typedef struct GPBFieldMask__storage_ {
  uint32_t _has_storage_[1];
  NSMutableArray *pathsArray;
} GPBFieldMask__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    GPB_DEBUG_CHECK_RUNTIME_VERSIONS();
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "pathsArray",
        .dataTypeSpecific.clazz = Nil,
        .number = GPBFieldMask_FieldNumber_PathsArray,
        .hasIndex = GPBNoHasBit,
        .offset = (uint32_t)offsetof(GPBFieldMask__storage_, pathsArray),
        .flags = GPBFieldRepeated,
        .dataType = GPBDataTypeString,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:GPBObjCClass(GPBFieldMask)
                                   messageName:@"FieldMask"
                               fileDescription:&GPBFieldMaskRoot_FileDescription
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(GPBFieldMask__storage_)
                                         flags:(GPBDescriptorInitializationFlags)(GPBDescriptorInitializationFlag_UsesClassRefs | GPBDescriptorInitializationFlag_Proto3OptionalKnown | GPBDescriptorInitializationFlag_ClosedEnumSupportKnown)];
    #if defined(DEBUG) && DEBUG
      NSAssert(descriptor == nil, @"Startup recursed!");
    #endif  // DEBUG
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end


#pragma clang diagnostic pop

// @@protoc_insertion_point(global_scope)

// clang-format on
