<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: google/protobuf/compiler/plugin.proto

namespace GPBMetadata\Google\Protobuf\Internal\Compiler;

class Plugin
{
    public static $is_initialized = false;

    public static function initOnce() {
        $pool = \Google\Protobuf\Internal\DescriptorPool::getGeneratedPool();

        if (static::$is_initialized == true) {
          return;
        }
        \GPBMetadata\Google\Protobuf\Internal\Descriptor::initOnce();
        $pool->addMessage('google.protobuf.internal.compiler.Version', \Google\Protobuf\Internal\Version::class)
            ->optional('major', \Google\Protobuf\Internal\GPBType::INT32, 1)
            ->optional('minor', \Google\Protobuf\Internal\GPBType::INT32, 2)
            ->optional('patch', \Google\Protobuf\Internal\GPBType::INT32, 3)
            ->optional('suffix', \Google\Protobuf\Internal\GPBType::STRING, 4)
            ->finalizeToPool();

        $pool->addMessage('google.protobuf.internal.compiler.CodeGeneratorRequest', \Google\Protobuf\Internal\CodeGeneratorRequest::class)
            ->repeated('file_to_generate', \Google\Protobuf\Internal\GPBType::STRING, 1)
            ->optional('parameter', \Google\Protobuf\Internal\GPBType::STRING, 2)
            ->repeated('proto_file', \Google\Protobuf\Internal\GPBType::MESSAGE, 15, 'google.protobuf.internal.FileDescriptorProto')
            ->optional('compiler_version', \Google\Protobuf\Internal\GPBType::MESSAGE, 3, 'google.protobuf.internal.compiler.Version')
            ->finalizeToPool();

        $pool->addMessage('google.protobuf.internal.compiler.CodeGeneratorResponse', \Google\Protobuf\Internal\CodeGeneratorResponse::class)
            ->optional('error', \Google\Protobuf\Internal\GPBType::STRING, 1)
            ->repeated('file', \Google\Protobuf\Internal\GPBType::MESSAGE, 15, 'google.protobuf.internal.compiler.CodeGeneratorResponse.File')
            ->finalizeToPool();

        $pool->addMessage('google.protobuf.internal.compiler.CodeGeneratorResponse.File', \Google\Protobuf\Internal\CodeGeneratorResponse_File::class)
            ->optional('name', \Google\Protobuf\Internal\GPBType::STRING, 1)
            ->optional('insertion_point', \Google\Protobuf\Internal\GPBType::STRING, 2)
            ->optional('content', \Google\Protobuf\Internal\GPBType::STRING, 15)
            ->finalizeToPool();

        $pool->finish();
        static::$is_initialized = true;
    }
}

