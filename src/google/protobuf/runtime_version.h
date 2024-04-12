#ifndef GOOGLE_PROTOBUF_RUNTIME_VERSION_H__
#define GOOGLE_PROTOBUF_RUNTIME_VERSION_H__

#ifdef PROTOBUF_VERSION
#error PROTOBUF_VERSION was previously defined
#endif  // PROTOBUF_VERSION

#ifdef PROTOBUF_VERSION_SUFFIX
#error PROTOBUF_VERSION_SUFFIX was previously defined
#endif  // PROTOBUF_VERSION_SUFFIX

#define PROTOBUF_VERSION 5027000
#define PROTOBUF_VERSION_SUFFIX "-dev"

#endif  // GOOGLE_PROTOBUF_RUNTIME_VERSION_H__
