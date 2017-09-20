<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: google/protobuf/any.proto

namespace Google\Protobuf;

use Google\Protobuf\Internal\DescriptorPool;
use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\GPBUtil;
use Google\Protobuf\Internal\Message;
use Google\Protobuf\Internal\RepeatedField;

/**
 * `Any` contains an arbitrary serialized protocol buffer message along with a
 * URL that describes the type of the serialized message.
 * Protobuf library provides support to pack/unpack Any values in the form
 * of utility functions or additional generated methods of the Any type.
 * Example 1: Pack and unpack a message in C++.
 *     Foo foo = ...;
 *     Any any;
 *     any.PackFrom(foo);
 *     ...
 *     if (any.UnpackTo(&foo)) {
 *       ...
 *     }
 * Example 2: Pack and unpack a message in Java.
 *     Foo foo = ...;
 *     Any any = Any.pack(foo);
 *     ...
 *     if (any.is(Foo.class)) {
 *       foo = any.unpack(Foo.class);
 *     }
 *  Example 3: Pack and unpack a message in Python.
 *     foo = Foo(...)
 *     any = Any()
 *     any.Pack(foo)
 *     ...
 *     if any.Is(Foo.DESCRIPTOR):
 *       any.Unpack(foo)
 *       ...
 * The pack methods provided by protobuf library will by default use
 * 'type.googleapis.com/full.type.name' as the type URL and the unpack
 * methods only use the fully qualified type name after the last '/'
 * in the type URL, for example "foo.bar.com/x/y.z" will yield type
 * name "y.z".
 * JSON
 * ====
 * The JSON representation of an `Any` value uses the regular
 * representation of the deserialized, embedded message, with an
 * additional field `&#64;type` which contains the type URL. Example:
 *     package google.profile;
 *     message Person {
 *       string first_name = 1;
 *       string last_name = 2;
 *     }
 *     {
 *       "&#64;type": "type.googleapis.com/google.profile.Person",
 *       "firstName": <string>,
 *       "lastName": <string>
 *     }
 * If the embedded message type is well-known and has a custom JSON
 * representation, that representation will be embedded adding a field
 * `value` which holds the custom JSON in addition to the `&#64;type`
 * field. Example (for message [google.protobuf.Duration][]):
 *     {
 *       "&#64;type": "type.googleapis.com/google.protobuf.Duration",
 *       "value": "1.212s"
 *     }
 *
 * Generated from protobuf message <code>google.protobuf.Any</code>
 */
class Any extends \Google\Protobuf\Internal\Message
{
    /**
     * A URL/resource name whose content describes the type of the
     * serialized protocol buffer message.
     * For URLs which use the scheme `http`, `https`, or no scheme, the
     * following restrictions and interpretations apply:
     * * If no scheme is provided, `https` is assumed.
     * * The last segment of the URL's path must represent the fully
     *   qualified name of the type (as in `path/google.protobuf.Duration`).
     *   The name should be in a canonical form (e.g., leading "." is
     *   not accepted).
     * * An HTTP GET on the URL must yield a [google.protobuf.Type][]
     *   value in binary format, or produce an error.
     * * Applications are allowed to cache lookup results based on the
     *   URL, or have them precompiled into a binary to avoid any
     *   lookup. Therefore, binary compatibility needs to be preserved
     *   on changes to types. (Use versioned type names to manage
     *   breaking changes.)
     * Schemes other than `http`, `https` (or the empty scheme) might be
     * used with implementation specific semantics.
     *
     * Generated from protobuf field <code>string type_url = 1;</code>
     */
    private $type_url = '';
    /**
     * Must be a valid serialized protocol buffer of the above specified type.
     *
     * Generated from protobuf field <code>bytes value = 2;</code>
     */
    private $value = '';

    const TYPE_URL_PREFIX = 'type.googleapis.com/';

    public function __construct() {
        \GPBMetadata\Google\Protobuf\Any::initOnce();
        parent::__construct();
    }

    /**
     * A URL/resource name whose content describes the type of the
     * serialized protocol buffer message.
     * For URLs which use the scheme `http`, `https`, or no scheme, the
     * following restrictions and interpretations apply:
     * * If no scheme is provided, `https` is assumed.
     * * The last segment of the URL's path must represent the fully
     *   qualified name of the type (as in `path/google.protobuf.Duration`).
     *   The name should be in a canonical form (e.g., leading "." is
     *   not accepted).
     * * An HTTP GET on the URL must yield a [google.protobuf.Type][]
     *   value in binary format, or produce an error.
     * * Applications are allowed to cache lookup results based on the
     *   URL, or have them precompiled into a binary to avoid any
     *   lookup. Therefore, binary compatibility needs to be preserved
     *   on changes to types. (Use versioned type names to manage
     *   breaking changes.)
     * Schemes other than `http`, `https` (or the empty scheme) might be
     * used with implementation specific semantics.
     *
     * Generated from protobuf field <code>string type_url = 1;</code>
     * @return string
     */
    public function getTypeUrl()
    {
        return $this->type_url;
    }

    /**
     * A URL/resource name whose content describes the type of the
     * serialized protocol buffer message.
     * For URLs which use the scheme `http`, `https`, or no scheme, the
     * following restrictions and interpretations apply:
     * * If no scheme is provided, `https` is assumed.
     * * The last segment of the URL's path must represent the fully
     *   qualified name of the type (as in `path/google.protobuf.Duration`).
     *   The name should be in a canonical form (e.g., leading "." is
     *   not accepted).
     * * An HTTP GET on the URL must yield a [google.protobuf.Type][]
     *   value in binary format, or produce an error.
     * * Applications are allowed to cache lookup results based on the
     *   URL, or have them precompiled into a binary to avoid any
     *   lookup. Therefore, binary compatibility needs to be preserved
     *   on changes to types. (Use versioned type names to manage
     *   breaking changes.)
     * Schemes other than `http`, `https` (or the empty scheme) might be
     * used with implementation specific semantics.
     *
     * Generated from protobuf field <code>string type_url = 1;</code>
     * @param string $var
     * @return $this
     */
    public function setTypeUrl($var)
    {
        GPBUtil::checkString($var, True);
        $this->type_url = $var;

        return $this;
    }

    /**
     * Must be a valid serialized protocol buffer of the above specified type.
     *
     * Generated from protobuf field <code>bytes value = 2;</code>
     * @return string
     */
    public function getValue()
    {
        return $this->value;
    }

    /**
     * Must be a valid serialized protocol buffer of the above specified type.
     *
     * Generated from protobuf field <code>bytes value = 2;</code>
     * @param string $var
     * @return $this
     */
    public function setValue($var)
    {
        GPBUtil::checkString($var, False);
        $this->value = $var;

        return $this;
    }

    /**
     * This method will try to resolve the type_url in Any message to get the
     * targeted message type. If failed, an error will be thrown. Otherwise,
     * the method will create a message of the targeted type and fill it with
     * the decoded value in Any.
     * @return unpacked message
     * @throws Exception Type url needs to be type.googleapis.com/fully-qulified.
     * @throws Exception Class hasn't been added to descriptor pool.
     * @throws Exception cannot decode data in value field.
     */
    public function unpack()
    {
        // Get fully qualifed name from type url.
        $type_url_len = strlen($this->type_url);
        $url_prifix_len = strlen(Any::TYPE_URL_PREFIX);
        if ($type_url_len < $url_prifix_len ||
            substr($this->type_url, 0, $url_prifix_len) !=
                Any::TYPE_URL_PREFIX) {
            throw new \Exception(
                "Type url needs to be type.googleapis.com/fully-qulified");
        }
        $fully_qualifed_name =
            substr($this->type_url, $url_prifix_len, $type_url_len);

        // Create message according to fully qualified name.
        $pool = DescriptorPool::getGeneratedPool();
        $desc = $pool->getDescriptorByProtoName( ".".$fully_qualifed_name);
        if (is_null($desc)) {
            throw new \Exception("Class ".$fully_qualifed_name
                                     ." hasn't been added to descriptor pool");
        }
        $klass = $desc->getClass();
        $msg = new $klass();

        // Merge data into message.
        $msg->mergeFromString($this->value);
        return $msg;
    }

    /**
     * The type_url will be created according to the given message’s type and
     * the value is encoded data from the given message..
     * @param message: A proto message.
     */
    public function pack($msg)
    {
        if (!$msg instanceof Message) {
            trigger_error("Given parameter is not a message instance.",
                          E_USER_ERROR);
            return;
        }

        // Set value using serialzed message.
        $this->value = $msg->serializeToString();

        // Set type url.
        $pool = DescriptorPool::getGeneratedPool();
        $desc = $pool->getDescriptorByClassName(get_class($msg));
        $fully_qualifed_name = $desc->getFullName();
        $this->type_url = Any::TYPE_URL_PREFIX.substr(
            $fully_qualifed_name, 1, strlen($fully_qualifed_name));
    }

    /**
     * This method returns whether the type_url in any_message is corresponded
     * to the given class.
     * @param klass: The fully qualified PHP class name of a proto message type.
     */
    public function is($klass)
    {
        $pool = DescriptorPool::getGeneratedPool();
        $desc = $pool->getDescriptorByClassName($klass);
        $fully_qualifed_name = $desc->getFullName();
        $type_url = Any::TYPE_URL_PREFIX.substr(
            $fully_qualifed_name, 1, strlen($fully_qualifed_name));
        return $this->type_url === $type_url;
    }
}
