// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: unittest_selfreferential_options.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021, 8981
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace UnitTest.Issues.TestProtos.SelfreferentialOptions {

  /// <summary>Holder for reflection information generated from unittest_selfreferential_options.proto</summary>
  public static partial class UnittestSelfreferentialOptionsReflection {

    #region Descriptor
    /// <summary>File descriptor for unittest_selfreferential_options.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static UnittestSelfreferentialOptionsReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          "CiZ1bml0dGVzdF9zZWxmcmVmZXJlbnRpYWxfb3B0aW9ucy5wcm90bxIpcHJv" +
          "dG9idWZfdW5pdHRlc3Rfc2VsZnJlZmVyZW50aWFsX29wdGlvbnMaIGdvb2ds" +
          "ZS9wcm90b2J1Zi9kZXNjcmlwdG9yLnByb3RvIkwKCkZvb09wdGlvbnMSHgoH" +
          "aW50X29wdBgBIAEoBUINyj4KCAHAPgLKPgIIAxITCgNmb28YAiABKAVCBso+" +
          "AxDSCSoJCOgHEICAgIACOjkKC2Jhcl9vcHRpb25zEh0uZ29vZ2xlLnByb3Rv" +
          "YnVmLkZpZWxkT3B0aW9ucxjoByABKAVCBMA+0gk6agoLZm9vX29wdGlvbnMS" +
          "HS5nb29nbGUucHJvdG9idWYuRmllbGRPcHRpb25zGOkHIAEoCzI1LnByb3Rv" +
          "YnVmX3VuaXR0ZXN0X3NlbGZyZWZlcmVudGlhbF9vcHRpb25zLkZvb09wdGlv" +
          "bnM6SwoLZm9vX2ludF9vcHQSNS5wcm90b2J1Zl91bml0dGVzdF9zZWxmcmVm" +
          "ZXJlbnRpYWxfb3B0aW9ucy5Gb29PcHRpb25zGOgHIAEoBTqCAQoLZm9vX2Zv" +
          "b19vcHQSNS5wcm90b2J1Zl91bml0dGVzdF9zZWxmcmVmZXJlbnRpYWxfb3B0" +
          "aW9ucy5Gb29PcHRpb25zGOkHIAEoCzI1LnByb3RvYnVmX3VuaXR0ZXN0X3Nl" +
          "bGZyZWZlcmVudGlhbF9vcHRpb25zLkZvb09wdGlvbnNCNKoCMVVuaXRUZXN0" +
          "Lklzc3Vlcy5UZXN0UHJvdG9zLlNlbGZyZWZlcmVudGlhbE9wdGlvbnM=");
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { global::Google.Protobuf.Reflection.DescriptorReflection.Descriptor, },
          new pbr::GeneratedClrTypeInfo(null, new pb::Extension[] { UnittestSelfreferentialOptionsExtensions.BarOptions, UnittestSelfreferentialOptionsExtensions.FooOptions, UnittestSelfreferentialOptionsExtensions.FooIntOpt, UnittestSelfreferentialOptionsExtensions.FooFooOpt }, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions), global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions.Parser, new[]{ "IntOpt", "Foo" }, null, null, null, null)
          }));
    }
    #endregion

  }
  /// <summary>Holder for extension identifiers generated from the top level of unittest_selfreferential_options.proto</summary>
  public static partial class UnittestSelfreferentialOptionsExtensions {
    /// <summary>
    /// Custom field option used on the definition of that field option.
    /// </summary>
    public static readonly pb::Extension<global::Google.Protobuf.Reflection.FieldOptions, int> BarOptions =
      new pb::Extension<global::Google.Protobuf.Reflection.FieldOptions, int>(1000, pb::FieldCodec.ForInt32(8000, 0));
    public static readonly pb::Extension<global::Google.Protobuf.Reflection.FieldOptions, global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions> FooOptions =
      new pb::Extension<global::Google.Protobuf.Reflection.FieldOptions, global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions>(1001, pb::FieldCodec.ForMessage(8010, global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions.Parser));
    public static readonly pb::Extension<global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions, int> FooIntOpt =
      new pb::Extension<global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions, int>(1000, pb::FieldCodec.ForInt32(8000, 0));
    public static readonly pb::Extension<global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions, global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions> FooFooOpt =
      new pb::Extension<global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions, global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions>(1001, pb::FieldCodec.ForMessage(8010, global::UnitTest.Issues.TestProtos.SelfreferentialOptions.FooOptions.Parser));
  }

  #region Messages
  public sealed partial class FooOptions : pb::IExtendableMessage<FooOptions>
  #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
      , pb::IBufferMessage
  #endif
  {
    private static readonly pb::MessageParser<FooOptions> _parser = new pb::MessageParser<FooOptions>(() => new FooOptions());
    private pb::UnknownFieldSet _unknownFields;
    private pb::ExtensionSet<FooOptions> _extensions;
    private pb::ExtensionSet<FooOptions> _Extensions { get { return _extensions; } }
    private int _hasBits0;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public static pb::MessageParser<FooOptions> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::UnitTest.Issues.TestProtos.SelfreferentialOptions.UnittestSelfreferentialOptionsReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public FooOptions() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public FooOptions(FooOptions other) : this() {
      _hasBits0 = other._hasBits0;
      intOpt_ = other.intOpt_;
      foo_ = other.foo_;
      _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
      _extensions = pb::ExtensionSet.Clone(other._extensions);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public FooOptions Clone() {
      return new FooOptions(this);
    }

    /// <summary>Field number for the "int_opt" field.</summary>
    public const int IntOptFieldNumber = 1;
    private readonly static int IntOptDefaultValue = 0;

    private int intOpt_;
    /// <summary>
    /// Custom field option used in definition of the extension message.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public int IntOpt {
      get { if ((_hasBits0 & 1) != 0) { return intOpt_; } else { return IntOptDefaultValue; } }
      set {
        _hasBits0 |= 1;
        intOpt_ = value;
      }
    }
    /// <summary>Gets whether the "int_opt" field is set</summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public bool HasIntOpt {
      get { return (_hasBits0 & 1) != 0; }
    }
    /// <summary>Clears the value of the "int_opt" field</summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public void ClearIntOpt() {
      _hasBits0 &= ~1;
    }

    /// <summary>Field number for the "foo" field.</summary>
    public const int FooFieldNumber = 2;
    private readonly static int FooDefaultValue = 0;

    private int foo_;
    /// <summary>
    /// Custom field option used in definition of the custom option's message.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public int Foo {
      get { if ((_hasBits0 & 2) != 0) { return foo_; } else { return FooDefaultValue; } }
      set {
        _hasBits0 |= 2;
        foo_ = value;
      }
    }
    /// <summary>Gets whether the "foo" field is set</summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public bool HasFoo {
      get { return (_hasBits0 & 2) != 0; }
    }
    /// <summary>Clears the value of the "foo" field</summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public void ClearFoo() {
      _hasBits0 &= ~2;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public override bool Equals(object other) {
      return Equals(other as FooOptions);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public bool Equals(FooOptions other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (IntOpt != other.IntOpt) return false;
      if (Foo != other.Foo) return false;
      if (!Equals(_extensions, other._extensions)) {
        return false;
      }
      return Equals(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public override int GetHashCode() {
      int hash = 1;
      if (HasIntOpt) hash ^= IntOpt.GetHashCode();
      if (HasFoo) hash ^= Foo.GetHashCode();
      if (_extensions != null) {
        hash ^= _extensions.GetHashCode();
      }
      if (_unknownFields != null) {
        hash ^= _unknownFields.GetHashCode();
      }
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public void WriteTo(pb::CodedOutputStream output) {
    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
      output.WriteRawMessage(this);
    #else
      if (HasIntOpt) {
        output.WriteRawTag(8);
        output.WriteInt32(IntOpt);
      }
      if (HasFoo) {
        output.WriteRawTag(16);
        output.WriteInt32(Foo);
      }
      if (_extensions != null) {
        _extensions.WriteTo(output);
      }
      if (_unknownFields != null) {
        _unknownFields.WriteTo(output);
      }
    #endif
    }

    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    void pb::IBufferMessage.InternalWriteTo(ref pb::WriteContext output) {
      if (HasIntOpt) {
        output.WriteRawTag(8);
        output.WriteInt32(IntOpt);
      }
      if (HasFoo) {
        output.WriteRawTag(16);
        output.WriteInt32(Foo);
      }
      if (_extensions != null) {
        _extensions.WriteTo(ref output);
      }
      if (_unknownFields != null) {
        _unknownFields.WriteTo(ref output);
      }
    }
    #endif

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public int CalculateSize() {
      int size = 0;
      if (HasIntOpt) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(IntOpt);
      }
      if (HasFoo) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(Foo);
      }
      if (_extensions != null) {
        size += _extensions.CalculateSize();
      }
      if (_unknownFields != null) {
        size += _unknownFields.CalculateSize();
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public void MergeFrom(FooOptions other) {
      if (other == null) {
        return;
      }
      if (other.HasIntOpt) {
        IntOpt = other.IntOpt;
      }
      if (other.HasFoo) {
        Foo = other.Foo;
      }
      pb::ExtensionSet.MergeFrom(ref _extensions, other._extensions);
      _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public void MergeFrom(pb::CodedInputStream input) {
    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
      input.ReadRawMessage(this);
    #else
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            if (!pb::ExtensionSet.TryMergeFieldFrom(ref _extensions, input)) {
              _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, input);
            }
            break;
          case 8: {
            IntOpt = input.ReadInt32();
            break;
          }
          case 16: {
            Foo = input.ReadInt32();
            break;
          }
        }
      }
    #endif
    }

    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    void pb::IBufferMessage.InternalMergeFrom(ref pb::ParseContext input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            if (!pb::ExtensionSet.TryMergeFieldFrom(ref _extensions, ref input)) {
              _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, ref input);
            }
            break;
          case 8: {
            IntOpt = input.ReadInt32();
            break;
          }
          case 16: {
            Foo = input.ReadInt32();
            break;
          }
        }
      }
    }
    #endif

    public TValue GetExtension<TValue>(pb::Extension<FooOptions, TValue> extension) {
      return pb::ExtensionSet.Get(ref _extensions, extension);
    }
    public pbc::RepeatedField<TValue> GetExtension<TValue>(pb::RepeatedExtension<FooOptions, TValue> extension) {
      return pb::ExtensionSet.Get(ref _extensions, extension);
    }
    public pbc::RepeatedField<TValue> GetOrInitializeExtension<TValue>(pb::RepeatedExtension<FooOptions, TValue> extension) {
      return pb::ExtensionSet.GetOrInitialize(ref _extensions, extension);
    }
    public void SetExtension<TValue>(pb::Extension<FooOptions, TValue> extension, TValue value) {
      pb::ExtensionSet.Set(ref _extensions, extension, value);
    }
    public bool HasExtension<TValue>(pb::Extension<FooOptions, TValue> extension) {
      return pb::ExtensionSet.Has(ref _extensions, extension);
    }
    public void ClearExtension<TValue>(pb::Extension<FooOptions, TValue> extension) {
      pb::ExtensionSet.Clear(ref _extensions, extension);
    }
    public void ClearExtension<TValue>(pb::RepeatedExtension<FooOptions, TValue> extension) {
      pb::ExtensionSet.Clear(ref _extensions, extension);
    }

  }

  #endregion

}

#endregion Designer generated code
