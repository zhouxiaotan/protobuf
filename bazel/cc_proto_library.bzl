"""cc_proto_library rule"""

load("@proto_bazel_features//:features.bzl", "bazel_features")
load("//bazel/private:bazel_cc_proto_library.bzl", _cc_proto_library = "cc_proto_library")  # buildifier: disable=bzl-visibility

def cc_proto_library(**kwattrs):
    # This condition causes Starlark rules to be used only on Bazel >=7.0.0
    if bazel_features.proto.starlark_proto_info:
        _cc_proto_library(**kwattrs)
    else:
        # On older Bazel versions keep using native rules, so that mismatch in ProtoInfo doesn't happen
        native.cc_proto_library(**kwattrs)  # buildifier: disable=native-cc-proto
