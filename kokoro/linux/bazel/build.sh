#!/bin/bash
#
# Build file to set up and run tests
set -eu

# Install Bazel 4.0.0.
use_bazel.sh 4.0.0
bazel version

# Change to repo root
cd $(dirname $0)/../../..
source kokoro/common/pyenv.sh
source kokoro/common/bazel_wrapper.sh
trap cleanup_invocation_ids ERR

#  Disabled for now, re-enable if appropriate.
#  //:build_files_updated_unittest \

bazel_args=(
  test
  --keep_going
  --copt=-Werror
  --host_copt=-Werror
  --test_output=errors
  --
  //...
  -//objectivec/...  # only works on macOS
  @com_google_protobuf_examples//...
)

bazel "${bazel_args[@]}"

# Verify that we can build successfully from generated tar files.
(
  pyenv versions
  pyenv shell 2.7.9  # python2 required for old googletest autotools support
  ./autogen.sh && ./configure && make -j$(nproc) dist
)
DIST=`ls *.tar.gz`
tar -xf $DIST
cd ${DIST//.tar.gz}
bazel build //:protobuf //:protobuf_java

cleanup_invocation_ids
