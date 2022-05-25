# Rules for distributable C++ libraries

load("@rules_cc//cc:action_names.bzl", cc_action_names = "ACTION_NAMES")
load("@rules_cc//cc:find_cc_toolchain.bzl", "find_cc_toolchain")

################################################################################
# Archive/linking support
################################################################################

def _collect_linker_input_objects(dep_label, cc_info, objs, pic_objs):
    """Accumulate .o and .pic.o files into `objs` and `pic_objs`."""
    link_ctx = cc_info.linking_context
    if link_ctx == None:
        return

    linker_inputs = link_ctx.linker_inputs.to_list()
    for link_input in linker_inputs:
        if link_input.owner != dep_label:
            # This is a transitive dep: skip it.
            continue

        for lib in link_input.libraries:
            objs.extend(lib.objects or [])
            pic_objs.extend(lib.pic_objects or [])

# Creates an action to build the `output_file` static library (archive)
# using `object_files`.
def _create_archive_action(
        ctx,
        feature_configuration,
        cc_toolchain_info,
        output_file,
        object_files):
    # Based on Bazel's src/main/starlark/builtins_bzl/common/cc/cc_import.bzl:

    # Build the command line and add args for all of the input files:
    archiver_variables = cc_common.create_link_variables(
        feature_configuration = feature_configuration,
        cc_toolchain = cc_toolchain_info,
        output_file = output_file.path,
        is_using_linker = False,
    )
    command_line = cc_common.get_memory_inefficient_command_line(
        feature_configuration = feature_configuration,
        action_name = cc_action_names.cpp_link_static_library,
        variables = archiver_variables,
    )
    args = ctx.actions.args()
    args.add_all(command_line)
    args.add_all(object_files)
    args.use_param_file("@%s", use_always = True)

    archiver_path = cc_common.get_tool_for_action(
        feature_configuration = feature_configuration,
        action_name = cc_action_names.cpp_link_static_library,
    )

    env = cc_common.get_environment_variables(
        feature_configuration = feature_configuration,
        action_name = cc_action_names.cpp_link_static_library,
        variables = archiver_variables,
    )

    ctx.actions.run(
        executable = archiver_path,
        arguments = [args],
        env = env,
        inputs = depset(
            direct = object_files,
            transitive = [
                cc_toolchain_info.all_files,
            ],
        ),
        use_default_shell_env = False,
        outputs = [output_file],
        mnemonic = "CppArchiveDist",
    )

def _create_dso_link_action(
        ctx,
        feature_configuration,
        cc_toolchain_info,
        object_files,
        pic_object_files):
    compilation_outputs = cc_common.create_compilation_outputs(
        objects = depset(object_files),
        pic_objects = depset(pic_object_files),
    )
    link_output = cc_common.link(
        actions = ctx.actions,
        feature_configuration = feature_configuration,
        cc_toolchain = cc_toolchain_info,
        compilation_outputs = compilation_outputs,
        name = ctx.label.name,
        output_type = "dynamic_library",
        user_link_flags = ctx.attr.linkopts,
    )
    library_to_link = link_output.library_to_link

    outputs = []

    # Note: library_to_link.dynamic_library and interface_library are often
    # symlinks in the solib directory. For DefaultInfo, prefer reporting
    # the resolved artifact paths.
    if library_to_link.resolved_symlink_dynamic_library != None:
        outputs.append(library_to_link.resolved_symlink_dynamic_library)
    elif library_to_link.dynamic_library != None:
        outputs.append(library_to_link.dynamic_library)

    if library_to_link.resolved_symlink_interface_library != None:
        outputs.append(library_to_link.resolved_symlink_interface_library)
    elif library_to_link.interface_library != None:
        outputs.append(library_to_link.interface_library)

    return outputs

################################################################################
# Source file/header support
################################################################################

CcFileList = provider(
    doc = "List of files to be built into a library.",
    fields = {
        # As a rule of thumb, `hdrs` and `textual_hdrs` are the files that
        # would be installed along with a prebuilt library.
        "hdrs": "public header files, including those used by generated code",
        "textual_hdrs": "files which are included but are not self-contained",

        # The `internal_hdrs` are header files which appear in `srcs`.
        # These are only used when compiling the library.
        "internal_hdrs": "internal header files (only used to build .cc files)",
        "srcs": "source files",
    },
)

def _flatten_target_files(targets):
    return depset(transitive = [target.files for target in targets])

def _extract_cc_file_list_from_cc_rule(rule_attr):
    # CcInfo is a proxy for what we expect this rule to look like.
    # However, some deps may expose `CcInfo` without having `srcs`,
    # `hdrs`, etc., so we use `getattr` to handle that gracefully.

    internal_hdrs = []
    srcs = []

    # Filter `srcs` so it only contains source files. Headers will go
    # into `internal_headers`.
    for src in _flatten_target_files(getattr(rule_attr, "srcs", [])).to_list():
        if src.extension.lower() in ["c", "cc", "cpp", "cxx"]:
            srcs.append(src)
        else:
            internal_hdrs.append(src)

    return CcFileList(
        hdrs = _flatten_target_files(getattr(rule_attr, "hdrs", [])),
        textual_hdrs = _flatten_target_files(getattr(
            rule_attr,
            "textual_hdrs",
            [],
        )),
        internal_hdrs = depset(internal_hdrs),
        srcs = depset(srcs),
    )

def _collect_cc_file_lists(cc_file_lists):
    """Combine several CcFileLists into a single one."""

    # The returned CcFileList will contain depsets of the deps' file lists.
    # These lists hold `depset()`s from each of `deps`.
    srcs = []
    hdrs = []
    internal_hdrs = []
    textual_hdrs = []

    for cfl in cc_file_lists:
        srcs.append(cfl.srcs)
        hdrs.append(cfl.hdrs)
        internal_hdrs.append(cfl.internal_hdrs)
        textual_hdrs.append(cfl.textual_hdrs)

    return CcFileList(
        srcs = depset(transitive = srcs),
        hdrs = depset(transitive = hdrs),
        internal_hdrs = depset(transitive = internal_hdrs),
        textual_hdrs = depset(transitive = textual_hdrs),
    )

def _extract_cc_file_list_from_test_suite(target, rule_attr):
    transitive = []

    for dep in getattr(rule_attr, "tests", []):
        transitive.append(dep[CcFileList])

    # Test suites can list tests explicitly, or they can implicitly
    # expand to "all tests in this package." In the latter case, Bazel
    # exposes the implicit tests via the internal attribute
    # `_implicit_tests`.
    #
    # Unfortunately, these attrs are currently (as of Bazel 5) the only
    # way to get the lists of tests, but this might change in the
    # future (c.f.: https://github.com/bazelbuild/bazel/issues/14993).
    for dep in getattr(rule_attr, "_implicit_tests", []):
        transitive.append(dep[CcFileList])

    if not transitive:
        # We are reaching into Bazel-internal attributes, which have no
        # guarantee that they won't change. If we end up with no tests,
        # print a warning.
        print(("WARNING: no tests were found in test_suite %s. If there " +
               "are no tests in package %s, consider removing the " +
               "test_suite. (Otherwise, this could be due to a bug " +
               "in gen_file_lists.)") %
              (target.label, target.label.package))

    return _collect_cc_file_lists(transitive)

def _cc_file_list_aspect_impl(target, ctx):
    # Special case: if this is a test_suite, collect the CcFileLists from
    # its tests.
    if ctx.rule.kind == "test_suite":
        return [_extract_cc_file_list_from_test_suite(target, ctx.rule.attr)]

    # Extract sources from a `cc_library` (or similar):
    if CcInfo in target:
        return [_extract_cc_file_list_from_cc_rule(ctx.rule.attr)]

    return []

cc_file_list_aspect = aspect(
    doc = """
Aspect to provide the list of sources and headers from a rule.

Output is CcFileList. Example:

  cc_library(
      name = "foo",
      srcs = [
          "foo.cc",
          "foo_internal.h",
      ],
      hdrs = ["foo.h"],
      textual_hdrs = ["foo_inl.inc"],
  )
  # produces:
  # CcFileList(
  #     hdrs = depset([File("foo.h")]),
  #     textual_hdrs = depset([File("foo_inl.inc")]),
  #     internal_hdrs = depset([File("foo_internal.h")]),
  #     srcs = depset([File("foo.cc")]),
  # )
""",
    implementation = _cc_file_list_aspect_impl,
    # Allow propagation for test_suite rules:
    attr_aspects = ["tests", "_implicit_tests"],
)

################################################################################
# Rule impl
################################################################################

def _collect_inputs(deps):
    """Collects files from a list of immediate deps.

    This rule collects source files and linker inputs for C++ deps. Only
    these immediate deps are considered, not transitive deps.

    The return value is a struct with object files (linker inputs),
    partitioned by PIC and non-pic, and the rules' source and header files:

        struct(
            objects = ...,       # non-PIC object files
            pic_objects = ...,   # PIC objects
            cc_file_list = ...,  # a CcFileList
        )

    Args:
      deps: Iterable of immediate deps. These will be treated as the "inputs,"
          but not the transitive deps.

    Returns:
      A struct with linker inputs, source files, and header files.
    """

    objs = []
    pic_objs = []
    cc_file_lists = []

    for dep in deps:
        if CcInfo in dep:
            _collect_linker_input_objects(
                dep.label,
                dep[CcInfo],
                objs,
                pic_objs,
            )

        if CcFileList in dep:
            cc_file_lists.append(dep[CcFileList])

    return struct(
        objects = objs,
        pic_objects = pic_objs,
        cc_file_list = _collect_cc_file_lists(cc_file_lists),
    )

# Implementation for cc_dist_library rule.
def _cc_dist_library_impl(ctx):
    cc_toolchain_info = find_cc_toolchain(ctx)

    feature_configuration = cc_common.configure_features(
        ctx = ctx,
        cc_toolchain = cc_toolchain_info,
    )

    inputs = _collect_inputs(ctx.attr.deps)

    # For static libraries, build separately with and without pic.

    stemname = "lib" + ctx.label.name
    outputs = []

    if len(inputs.objects) > 0:
        archive_out = ctx.actions.declare_file(stemname + ".a")
        _create_archive_action(
            ctx,
            feature_configuration,
            cc_toolchain_info,
            archive_out,
            inputs.objects,
        )
        outputs.append(archive_out)

    if len(inputs.pic_objects) > 0:
        pic_archive_out = ctx.actions.declare_file(stemname + ".pic.a")
        _create_archive_action(
            ctx,
            feature_configuration,
            cc_toolchain_info,
            pic_archive_out,
            inputs.pic_objects,
        )
        outputs.append(pic_archive_out)

    # For dynamic libraries, use the `cc_common.link` command to ensure
    # everything gets built correctly according to toolchain definitions.
    outputs.extend(_create_dso_link_action(
        ctx,
        feature_configuration,
        cc_toolchain_info,
        inputs.objects,
        inputs.pic_objects,
    ))

    # We could expose the libraries for use from cc rules:
    #
    # linking_context = cc_common.create_linking_context(
    #     linker_inputs = depset([
    #         cc_common.create_linker_input(
    #             owner = ctx.label,
    #             libraries = depset([library_to_link]),
    #         ),
    #     ]),
    # )
    # cc_info = CcInfo(linking_context = linking_context)  # and return this
    #
    # However, if the goal is to force a protobuf dependency to use the
    # DSO, then `cc_import` is a better-supported way to do so.
    #
    # If we wanted to expose CcInfo from this rule (and make it usable as a
    # C++ dependency), then we would probably want to include the static
    # archive and headers as well. exposing headers would probably require
    # an additional aspect to extract CcInfos with just the deps' headers.

    return [
        DefaultInfo(files = depset(outputs)),
        inputs.cc_file_list,
    ]

cc_dist_library = rule(
    implementation = _cc_dist_library_impl,
    doc = """
Create libraries suitable for distribution.

This rule creates static and dynamic libraries from the libraries listed in
'deps'. The resulting libraries are suitable for distributing all of 'deps'
in a single logical library, for example, in an installable binary package.
Only the targets listed in 'deps' are included in the result (i.e., the
output does not include transitive dependencies), allowing precise control
over the library boundary.

The outputs of this rule are a dynamic library and a static library. (If
the build produces both PIC and non-PIC object files, then there is also a
second static library.) The example below illustrates additional details.

This rule is different from Bazel's experimental `shared_cc_library` in
several ways. First, this rule ignores transitive dependencies, which means
that dynamic library dependencies generally need to be specified via
'linkopts'. Second, this rule produces a static archive library in addition
to the dynamic shared library. Third, this rule is not directly usable as a
C++ dependency (although the outputs could be used, e.g., by `cc_import`).

Example:

    cc_library(name = "a", srcs = ["a.cc"], hdrs = ["a.h"])
    cc_library(name = "b", srcs = ["b.cc"], hdrs = ["b.h"], deps = [":a"])
    cc_library(name = "c", srcs = ["c.cc"], hdrs = ["c.h"], deps = [":b"])

    # Creates libdist.so and (typically) libdist.pic.a:
    # (This may also produce libdist.a if the build produces non-PIC objects.)
    cc_dist_library(
        name = "dist",
        linkopts = ["-la"],   # libdist.so dynamically links against liba.so.
        deps = [":b", ":c"],  # Output contains b.o and c.o, but not a.o.
    )
""",
    attrs = {
        "deps": attr.label_list(
            doc = ("The list of libraries to be included in the outputs. " +
                   "Only these targets' compilation outputs will be " +
                   "included (i.e., the transitive dependencies are not " +
                   "included in the output)."),
            aspects = [cc_file_list_aspect],
        ),
        "linkopts": attr.string_list(
            doc = ("Add these flags to the C++ linker command when creating " +
                   "the dynamic library."),
        ),
        # C++ toolchain before https://github.com/bazelbuild/bazel/issues/7260:
        "_cc_toolchain": attr.label(
            default = Label("@rules_cc//cc:current_cc_toolchain"),
        ),
    },
    toolchains = [
        # C++ toolchain after https://github.com/bazelbuild/bazel/issues/7260:
        "@bazel_tools//tools/cpp:toolchain_type",
    ],
    fragments = ["cpp"],
)
