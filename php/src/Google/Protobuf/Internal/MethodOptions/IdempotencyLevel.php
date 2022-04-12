<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: google/protobuf/descriptor.proto

namespace Google\Protobuf\Internal\MethodOptions;

use UnexpectedValueException;

/**
 * Is this method side-effect-free (or safe in HTTP parlance), or idempotent,
 * or neither? HTTP based RPC implementation may choose GET verb for safe
 * methods, and PUT verb for idempotent methods instead of the default POST.
 *
 * Protobuf type <code>google.protobuf.MethodOptions.IdempotencyLevel</code>
 */
class IdempotencyLevel
{
    /**
     * Generated from protobuf enum <code>IDEMPOTENCY_UNKNOWN = 0;</code>
     */
    const IDEMPOTENCY_UNKNOWN = 0;
    /**
     * implies idempotent
     *
     * Generated from protobuf enum <code>NO_SIDE_EFFECTS = 1;</code>
     */
    const NO_SIDE_EFFECTS = 1;
    /**
     * idempotent, but may have side effects
     *
     * Generated from protobuf enum <code>IDEMPOTENT = 2;</code>
     */
    const IDEMPOTENT = 2;

    private static $valueToName = [
        self::IDEMPOTENCY_UNKNOWN => 'IDEMPOTENCY_UNKNOWN',
        self::NO_SIDE_EFFECTS => 'NO_SIDE_EFFECTS',
        self::IDEMPOTENT => 'IDEMPOTENT',
    ];

    public static function name($value)
    {
        if (!isset(self::$valueToName[$value])) {
            throw new UnexpectedValueException(sprintf(
                    'Enum %s has no name defined for value %s', __CLASS__, $value));
        }
        return self::$valueToName[$value];
    }


    public static function value($name)
    {
        $const = __CLASS__ . '::' . strtoupper($name);
        if (!defined($const)) {
            $pbconst =  __CLASS__. '::PB' . strtoupper($name);
            if (!defined($pbconst)) {
                throw new UnexpectedValueException(sprintf(
                        'Enum %s has no value defined for name %s', __CLASS__, $name));
                }
                return constant($pbconst);
        }
        return constant($const);
    }
}

// Adding a class alias for backwards compatibility with the previous class name.
class_alias(IdempotencyLevel::class, \Google\Protobuf\Internal\MethodOptions_IdempotencyLevel::class);

