<?php
# Generated by the protocol buffer compiler: libprotoc 21.12.  DO NOT EDIT!
# source: google/protobuf/type.proto

namespace Google\Protobuf;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 * A protocol buffer option, which can be attached to a message, field,
 * enumeration, etc.
 *
 * Generated from protobuf message <code>google.protobuf.Option</code>
 */
class Option extends \Google\Protobuf\Internal\Message
{
    /**
     * The option's name. For protobuf built-in options (options defined in
     * descriptor.proto), this is the short name. For example, `"map_entry"`.
     * For custom options, it should be the fully-qualified name. For example,
     * `"google.api.http"`.
     *
     * Generated from protobuf field <code>string name = 1;</code>
     */
    protected $name = '';
    /**
     * The option's value packed in an Any message. If the value is a primitive,
     * the corresponding wrapper type defined in google/protobuf/wrappers.proto
     * should be used. If the value is an enum, it should be stored as an int32
     * value using the google.protobuf.Int32Value type.
     *
     * Generated from protobuf field <code>.google.protobuf.Any value = 2;</code>
     */
    protected $value = null;

    /**
     * Constructor.
     *
     * @param array $data {
     *     Optional. Data for populating the Message object.
     *
     *     @type string $name
     *           The option's name. For protobuf built-in options (options defined in
     *           descriptor.proto), this is the short name. For example, `"map_entry"`.
     *           For custom options, it should be the fully-qualified name. For example,
     *           `"google.api.http"`.
     *     @type \Google\Protobuf\Any $value
     *           The option's value packed in an Any message. If the value is a primitive,
     *           the corresponding wrapper type defined in google/protobuf/wrappers.proto
     *           should be used. If the value is an enum, it should be stored as an int32
     *           value using the google.protobuf.Int32Value type.
     * }
     */
    public function __construct($data = NULL) {
        \GPBMetadata\Google\Protobuf\Type::initOnce();
        parent::__construct($data);
    }

    /**
     * The option's name. For protobuf built-in options (options defined in
     * descriptor.proto), this is the short name. For example, `"map_entry"`.
     * For custom options, it should be the fully-qualified name. For example,
     * `"google.api.http"`.
     *
     * Generated from protobuf field <code>string name = 1;</code>
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * The option's name. For protobuf built-in options (options defined in
     * descriptor.proto), this is the short name. For example, `"map_entry"`.
     * For custom options, it should be the fully-qualified name. For example,
     * `"google.api.http"`.
     *
     * Generated from protobuf field <code>string name = 1;</code>
     * @param string $var
     * @return $this
     */
    public function setName($var)
    {
        GPBUtil::checkString($var, True);
        $this->name = $var;

        return $this;
    }

    /**
     * The option's value packed in an Any message. If the value is a primitive,
     * the corresponding wrapper type defined in google/protobuf/wrappers.proto
     * should be used. If the value is an enum, it should be stored as an int32
     * value using the google.protobuf.Int32Value type.
     *
     * Generated from protobuf field <code>.google.protobuf.Any value = 2;</code>
     * @return \Google\Protobuf\Any|null
     */
    public function getValue()
    {
        return $this->value;
    }

    public function hasValue()
    {
        return isset($this->value);
    }

    public function clearValue()
    {
        unset($this->value);
    }

    /**
     * The option's value packed in an Any message. If the value is a primitive,
     * the corresponding wrapper type defined in google/protobuf/wrappers.proto
     * should be used. If the value is an enum, it should be stored as an int32
     * value using the google.protobuf.Int32Value type.
     *
     * Generated from protobuf field <code>.google.protobuf.Any value = 2;</code>
     * @param \Google\Protobuf\Any $var
     * @return $this
     */
    public function setValue($var)
    {
        GPBUtil::checkMessage($var, \Google\Protobuf\Any::class);
        $this->value = $var;

        return $this;
    }

}

