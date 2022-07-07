# Protocol Buffers - Google's data interchange format
# Copyright 2022 Google Inc.  All rights reserved.
# https://developers.google.com/protocol-buffers/
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

##
# Implementation details below are subject to breaking changes without
# warning and are intended for use only within the gem.
module Google
  module Protobuf
    module Internal
      module Convert

        # Arena should be the
        # @param value [Object] Value to convert
        # @param arena [Arena] Arena that owns the Message where the MessageValue
        #   will be set
        # @return [Google::Protobuf::FFI::MessageValue]
        def convert_ruby_to_upb(value, arena, c_type, msg_or_enum_def)
          raise ArgumentError.new "Expected Descriptor or EnumDescriptor, instead got #{msg_or_enum_def.class}" unless [NilClass, Descriptor, EnumDescriptor].include? msg_or_enum_def.class
          return_value = Google::Protobuf::FFI::MessageValue.new
          case c_type
          when :float
            raise TypeError.new "Expected number type for float field '#{name}' (given #{value.class})." unless value.respond_to? :to_f
            return_value[:float_val] = value.to_f
          when :double
            raise TypeError.new "Expected number type for double field '#{name}' (given #{value.class})." unless value.respond_to? :to_f
            return_value[:double_val] = value.to_f
          when :bool
            raise TypeError.new "Invalid argument for boolean field '#{name}' (given #{value.class})." unless [TrueClass, FalseClass].include? value.class
            return_value[:bool_val] = value
          when :string
            raise TypeError.new "Invalid argument for string field '#{name}' (given #{value.class})." unless [Symbol, String].include? value.class
            begin
              string_value = value.to_s.encode("UTF-8")
            rescue Encoding::UndefinedConversionError
              # TODO(jatl) - why not include the field name here?
              raise Encoding::UndefinedConversionError.new "String is invalid UTF-8"
            end
            return_value[:str_val][:size] = string_value.bytesize
            return_value[:str_val][:data] = Google::Protobuf::FFI.arena_malloc(arena, string_value.bytesize)
            # TODO(jatl) - how important is it to still use arena malloc, versus the following?
            # buffer = ::FFI::MemoryPointer.new(:char, string_value.bytesize)
            # buffer.put_bytes(0, string_value)
            # return_value[:str_val][:data] = buffer
            return_value[:str_val][:data].write_string(string_value)
          when :bytes
            raise TypeError.new "Invalid argument for bytes field '#{name}' (given #{value.class})." unless value.is_a? String
            string_value = value.encode("ASCII-8BIT")
            return_value[:str_val][:size] = string_value.bytesize
            return_value[:str_val][:data] = Google::Protobuf::FFI.arena_malloc(arena, string_value.bytesize)
            return_value[:str_val][:data].write_string_length(string_value, string_value.bytesize)
          when :message
            raise TypeError.new "nil message not allowed here." if value.nil?
            if value.is_a? Hash
              raise RuntimeError.new "Attempted to initialize message from Hash for field #{name} but have no definition" if msg_or_enum_def.nil?
              new_message = msg_or_enum_def.msgclass.
                send(:private_constructor, arena, initial_value: value)
              return_value[:msg_val] = new_message.send(:msg)
              return return_value
            end

            descriptor = value.class.respond_to?(:descriptor) ? value.class.descriptor : nil
            if descriptor != msg_or_enum_def
              wkt = Google::Protobuf::FFI.get_well_known_type(msg_or_enum_def)
              case wkt
              when :Timestamp
                raise TypeError.new "Invalid type #{value.class} to assign to submessage field '#{name}'." unless value.kind_of? Time
                new_message = Google::Protobuf::FFI.new_message_from_def msg_or_enum_def, arena
                sec = Google::Protobuf::FFI::MessageValue.new
                sec[:int64_val] = value.tv_sec
                sec_field_def = Google::Protobuf::FFI.get_field_by_number msg_or_enum_def, 1
                raise "Should be impossible" unless Google::Protobuf::FFI.set_message_field new_message, sec_field_def, sec, arena
                nsec_field_def = Google::Protobuf::FFI.get_field_by_number msg_or_enum_def, 2
                nsec = Google::Protobuf::FFI::MessageValue.new
                nsec[:int32_val] = value.tv_nsec
                raise "Should be impossible" unless Google::Protobuf::FFI.set_message_field new_message, nsec_field_def, nsec, arena
                return_value[:msg_val] = new_message
              when :Duration
                raise TypeError.new "Invalid type #{value.class} to assign to submessage field '#{name}'." unless value.kind_of? Numeric
                new_message = Google::Protobuf::FFI.new_message_from_def msg_or_enum_def, arena
                sec = Google::Protobuf::FFI::MessageValue.new
                sec[:int64_val] = value
                sec_field_def = Google::Protobuf::FFI.get_field_by_number msg_or_enum_def, 1
                raise "Should be impossible" unless Google::Protobuf::FFI.set_message_field new_message, sec_field_def, sec, arena
                nsec_field_def = Google::Protobuf::FFI.get_field_by_number msg_or_enum_def, 2
                nsec = Google::Protobuf::FFI::MessageValue.new
                nsec[:int32_val] = ((value.to_f - value.to_i) * 1000000000).round
                raise "Should be impossible" unless Google::Protobuf::FFI.set_message_field new_message, nsec_field_def, nsec, arena
                return_value[:msg_val] = new_message
              else
                raise TypeError.new "Invalid type #{value.class} to assign to submessage field '#{name}'."
              end
            else
              arena.fuse(value.send :arena)
              return_value[:msg_val] = value.send :msg
            end
          when :enum
            return_value[:int32_val] = case value
              when Numeric
                value.to_i
              when String, Symbol
                enum_number = EnumDescriptor.send(:lookup_name, msg_or_enum_def, value.to_s)
                #TODO(jatl) add the bad value to the error message after tests pass
                raise RangeError.new "Unknown symbol value for enum field '#{name}'." if enum_number.nil?
                enum_number
              else
                raise TypeError.new "Expected number or symbol type for enum field '#{name}'."
              end
          #TODO(jatl) After all tests pass, improve error message across integer type by including actual offending value
          when :int32
            raise TypeError.new "Expected number type for integral field '#{name}' (given #{value.class})." unless value.is_a? Numeric
            raise RangeError.new "Non-integral floating point value assigned to integer field '#{name}' (given #{value.class})." if value.floor != value
            raise RangeError.new "Value assigned to int32 field '#{name}' (given #{value.class}) with more than 32-bits." unless value.to_i.bit_length < 32
            return_value[:int32_val] = value.to_i
          when :uint32
            raise TypeError.new "Expected number type for integral field '#{name}' (given #{value.class})." unless value.is_a? Numeric
            raise RangeError.new "Non-integral floating point value assigned to integer field '#{name}' (given #{value.class})." if value.floor != value
            raise RangeError.new "Assigning negative value to unsigned integer field '#{name}' (given #{value.class})." if value < 0
            raise RangeError.new "Value assigned to uint32 field '#{name}' (given #{value.class}) with more than 32-bits." unless value.to_i.bit_length < 33
            return_value[:uint32_val] = value.to_i
          when :int64
            raise TypeError.new "Expected number type for integral field '#{name}' (given #{value.class})." unless value.is_a? Numeric
            raise RangeError.new "Non-integral floating point value assigned to integer field '#{name}' (given #{value.class})." if value.floor != value
            raise RangeError.new "Value assigned to int64 field '#{name}' (given #{value.class}) with more than 64-bits." unless value.to_i.bit_length < 64
            return_value[:int64_val] = value.to_i
          when :uint64
            raise TypeError.new "Expected number type for integral field '#{name}' (given #{value.class})." unless value.is_a? Numeric
            raise RangeError.new "Non-integral floating point value assigned to integer field '#{name}' (given #{value.class})." if value.floor != value
            raise RangeError.new "Assigning negative value to unsigned integer field '#{name}' (given #{value.class})." if value < 0
            raise RangeError.new "Value assigned to uint64 field '#{name}' (given #{value.class}) with more than 64-bits." unless value.to_i.bit_length < 65
            return_value[:uint64_val] = value.to_i
          else
            raise RuntimeError.new "Unsupported type #{c_type}"
          end
          return_value
        end

        ##
        # Safe to call without an arena if the caller has checked that c_type
        # is not :message.
        # @param message_value [Google::Protobuf::FFI::MessageValue] Value to be converted.
        # @param c_type [Google::Protobuf::FFI::CType] Enum representing the type of message_value
        # @param msg_or_enum_def [::FFI::Pointer] Pointer to the MsgDef or EnumDef definition
        # @param arena [Google::Protobuf::Internal::Arena] Arena to create Message instances, if needed
        def convert_upb_to_ruby(message_value, c_type, msg_or_enum_def = nil, arena = nil)
          throw TypeError.new "Expected MessageValue but got #{message_value.class}" unless message_value.is_a? Google::Protobuf::FFI::MessageValue

          case c_type
          when :bool
            message_value[:bool_val]
          when :int32
            message_value[:int32_val]
          when :uint32
            message_value[:uint32_val]
          when :double
            message_value[:double_val]
          when :int64
            message_value[:int64_val]
          when :uint64
            message_value[:uint64_val]
          when :string
            if message_value[:str_val][:size].zero?
              ""
            else
              message_value[:str_val][:data].read_string_length(message_value[:str_val][:size]).force_encoding("UTF-8").freeze
            end
          when :bytes
            if message_value[:str_val][:size].zero?
              ""
            else
              message_value[:str_val][:data].read_string_length(message_value[:str_val][:size]).force_encoding("ASCII-8BIT").freeze
            end
          when :float
            message_value[:float_val]
          when :enum
            EnumDescriptor.send(:lookup_value, msg_or_enum_def, message_value[:int32_val]) || message_value[:int32_val]
          when :message
            raise "Null Arena for message" if arena.nil?
            Descriptor.send(:get_message, message_value[:msg_val], msg_or_enum_def, arena)
          else
            raise RuntimeError.new "Unexpected type #{c_type}"
          end
        end

        # @param message_descriptor [Descriptor] Message Descriptor
        # @return [::FFI::Pointer] PoolDef pointer
        def pool_def_from_message_definition(message_descriptor)
          raise RuntimeError.new "Descriptor is nil" if message_descriptor.nil?
          file_def = Google::Protobuf::FFI.get_message_file_def message_descriptor
          raise RuntimeError.new "FileDef is nil" if file_def.nil?
          raise RuntimeError.new "FileDef is null" if file_def.null?
          pool_def = Google::Protobuf::FFI.file_def_pool file_def
          raise RuntimeError.new "PoolDef is nil" if pool_def.nil?
          raise RuntimeError.new "PoolDef is null" if pool_def.null?
          pool_def
        end

        def to_h_internal msg, message_descriptor
          return nil if msg.nil? or msg.null?
          hash = {}
          is_proto2 = Google::Protobuf::FFI.message_def_syntax(message_descriptor) == :Proto2
          message_descriptor.each do |field_descriptor|
            # TODO: Legacy behavior, remove when we fix the is_proto2 differences.
            if !is_proto2 and
              field_descriptor.sub_message? and
              !field_descriptor.repeated? and
              !Google::Protobuf::FFI.get_message_has(msg, field_descriptor)
              hash[field_descriptor.name.to_sym] = nil
              next
            end

            # Do not include fields that are not present (oneof or optional fields).
            if is_proto2 and field_descriptor.has_presence? and !Google::Protobuf::FFI.get_message_has(msg, field_descriptor)
              next
            end

            message_value = Google::Protobuf::FFI.get_message_value msg, field_descriptor

            # Proto2 omits empty map/repeated fields also.
            if field_descriptor.map?
              hash_entry = map_create_hash(message_value[:map_val], field_descriptor)
            elsif field_descriptor.repeated?
              array = message_value[:array_val]
              if is_proto2 and (array.null? || Google::Protobuf::FFI.array_size(array).zero?)
                next
              end
              hash_entry = repeated_field_create_array(array, field_descriptor, field_descriptor.type)
            else
              hash_entry = scalar_create_hash(message_value, field_descriptor.type, field_descriptor: field_descriptor)
            end

            hash[field_descriptor.name.to_sym] = hash_entry

          end

          hash
        end

        def map_create_hash(map_ptr, field_descriptor)
          return {} if map_ptr.nil? or map_ptr.null?
          return_value = {}

          message_descriptor = field_descriptor.send(:subtype)
          key_field_def = Google::Protobuf::FFI.get_field_by_number(message_descriptor, 1)
          key_field_type = Google::Protobuf::FFI.get_type(key_field_def)

          value_field_def = Google::Protobuf::FFI.get_field_by_number(message_descriptor, 2)
          value_field_type = Google::Protobuf::FFI.get_type(value_field_def)

          iter = ::FFI::MemoryPointer.new(:size_t, 1)
          iter.write(:size_t, Google::Protobuf::FFI::Upb_Map_Begin)
          while Google::Protobuf::FFI.map_next(map_ptr, iter) do
            iter_size_t = iter.read(:size_t)
            key_message_value = Google::Protobuf::FFI.map_key(map_ptr, iter_size_t)
            value_message_value = Google::Protobuf::FFI.map_value(map_ptr, iter_size_t)
            hash_key = convert_upb_to_ruby(key_message_value, key_field_type)
            hash_value = scalar_create_hash(value_message_value, value_field_type, msg_or_enum_descriptor: value_field_def.subtype)
            return_value[hash_key] = hash_value
          end
          return_value
        end

        def repeated_field_create_array(array, field_descriptor, type)
          return_value = []
          # puts "JATL - about to call FFI method array_size(#{array}); stacktrace:\n#{caller.join("\n")}"
          n = (array.nil? || array.null?) ? 0 : Google::Protobuf::FFI.array_size(array)
          0.upto(n - 1) do |i|
            message_value = Google::Protobuf::FFI.get_msgval_at(array, i)
            return_value << scalar_create_hash(message_value, type, field_descriptor: field_descriptor)
          end
          return_value
        end

        # @param field_descriptor [FieldDescriptor] Descriptor of the field to convert to a hash.
        def scalar_create_hash(message_value, type, field_descriptor: nil, msg_or_enum_descriptor: nil)
          if [:message, :enum].include? type
            if field_descriptor.nil?
              if msg_or_enum_descriptor.nil?
                raise "scalar_create_hash requires either a FieldDescriptor, MessageDescriptor, or EnumDescriptor as an argument, but received only nil"
              end
            else
              msg_or_enum_descriptor = field_descriptor.subtype
            end
            if type == :message
              to_h_internal(message_value[:msg_val], msg_or_enum_descriptor)
            elsif type == :enum
              convert_upb_to_ruby message_value, type, msg_or_enum_descriptor
            end
          else
            convert_upb_to_ruby message_value, type
          end
        end

        def message_value_deep_copy(message_value, type, descriptor, arena)
          raise unless message_value.is_a? Google::Protobuf::FFI::MessageValue
          new_message_value = Google::Protobuf::FFI::MessageValue.new
          case type
          when :string, :bytes
            # TODO(jatl) - how important is it to still use arena malloc, versus using FFI MemoryPointers?
            new_message_value[:str_val][:size] = message_value[:str_val][:size]
            new_message_value[:str_val][:data] = Google::Protobuf::FFI.arena_malloc(arena, message_value[:str_val][:size])
            Google::Protobuf::FFI.memcpy(new_message_value[:str_val][:data], message_value[:str_val][:data], message_value[:str_val][:size])
          when :message
            new_message_value[:msg_val] = descriptor.msgclass.send(:deep_copy, message_value[:msg_val], arena).send(:msg)
          else
            Google::Protobuf::FFI.memcpy(new_message_value.to_ptr, message_value.to_ptr, Google::Protobuf::FFI::MessageValue.size)
          end
          new_message_value
        end

        # @param message_value [Google::Protobuf::FFI::MessageValue] Value to inspect
        # @param field_descriptor [FieldDescriptor] Pointer to the FieldDef
        def inspect_message_value(message_value, type: nil, field_descriptor: nil, msg_or_enum_descriptor: nil)
          raise ArgumentError.new "Expected message_value to be a MessageValue, but was #{message_value.class}" unless message_value.is_a? Google::Protobuf::FFI::MessageValue
          c_type = type || Google::Protobuf::FFI.get_c_type(field_descriptor)
          case c_type
          when :bool
            message_value[:bool_val].inspect
          when :int32
            message_value[:int32_val].inspect
          when :uint32
            message_value[:uint32_val].inspect
          when :double
            message_value[:double_val].inspect
          when :int64
            message_value[:int64_val].inspect
          when :uint64
            message_value[:uint64_val].inspect
          when :string
            if message_value[:str_val][:size].zero?
              ""
            else
              message_value[:str_val][:data].read_string_length(message_value[:str_val][:size]).force_encoding("UTF-8")
            end.inspect
          when :bytes
            if message_value[:str_val][:size].zero?
              ""
            else
              message_value[:str_val][:data].read_string_length(message_value[:str_val][:size]).force_encoding("ASCII-8BIT")
            end.inspect
          when :float
            message_value[:float_val].inspect
          when :enum
            enum_descriptor = msg_or_enum_descriptor || Google::Protobuf::FFI.get_subtype_as_enum(field_descriptor)
            enum_value = EnumDescriptor.lookup_value enum_descriptor, message_value[:int32_val]
            (enum_value || message_value[:int32_val]).inspect
          when :message
            sub_msg_descriptor = msg_or_enum_descriptor || Google::Protobuf::FFI.get_subtype_as_message(field_descriptor)
            sub_msg_descriptor.msgclass.send(:inspect_internal, message_value[:msg_val])
          else
            raise RuntimeError.new "Unexpected type #{c_type}"
          end
        end
      end
    end
  end
end
