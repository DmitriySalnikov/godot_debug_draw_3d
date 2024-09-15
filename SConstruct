#!/usr/bin/env python

from SCons.Script import SConscript
from SCons.Script.SConscript import SConsEnvironment
from SCons.Script import ARGLIST, ARGUMENTS, BUILD_TARGETS, COMMAND_LINE_TARGETS, DEFAULT_TARGETS

import SCons, SCons.Script
import sys, os, platform
import lib_utils, lib_utils_external, lib_utils_gen_dd3d_api

# Fixing the encoding of the console
if platform.system() == "Windows":
    os.system("chcp 65001")

SConsEnvironment.EnsureSConsVersion(4, 0)
SConsEnvironment.EnsurePythonVersion(3, 8)

# Project config
project_name = "Debug Draw 3D"
lib_name = "dd3d"
default_output_dir = os.path.join("addons", "debug_draw_3d", "libs")
src_folder = "src"

# If necessary, add patches from the code
patches_to_apply = [
    "patches/godot_cpp_exclude_unused_classes.patch",  # Removes unused godot-cpp classes from the build process
    "patches/unity_build.patch",  # Speeds up the build by merging the source files. It can increase the size of assemblies.
    "patches/web_threads.patch",  # Adds the build flag that appeared in Godot 4.3. Required for a web build compatible with Godot 4.3.
    "patches/big_int_fix.patch",  # Fixes runtime link errors
]

print(
    f"If you add new source files (e.g. .cpp, .c), do not forget to specify them in '{src_folder}/default_sources.json'."+
    f"\n\tOr add them to 'setup_defines_and_flags' inside 'SConstruct'."
)
print("To apply git patches, use 'scons apply_patches'.")
print("To generate native APIs, use 'scons gen_apis'.")
# print("To build cmake libraries, use 'scons build_cmake'.")


# Additional console arguments
def setup_options(env: SConsEnvironment, arguments):
    from SCons.Variables import Variables, BoolVariable, EnumVariable, PathVariable

    opts = Variables([], arguments)

    # It must be here for lib_utils.py
    opts.Add(
        PathVariable(
            "addon_output_dir", "Path to the output directory", default_output_dir, PathVariable.PathIsDirCreate
        )
    )

    opts.Add(BoolVariable("build_cpp_api_tests", "Build only cpp api tests", False))
    opts.Add(BoolVariable("native_api_enabled", "Enable the native API module", True))
    opts.Add(BoolVariable("telemetry_enabled", "Enable the telemetry module", False))
    opts.Add(BoolVariable("tracy_enabled", "Enable tracy profiler", False))
    opts.Add(BoolVariable("force_enabled_dd3d", "Keep the rendering code in the release build", False))
    opts.Add(BoolVariable("fix_precision_enabled", "Fix precision errors at greater distances, utilizing more CPU resources.\nApplies only in combination with 'precision=double'", True))
    opts.Add(BoolVariable("shader_world_coords_enabled", "Use world coordinates in shaders, if applicable.\nExpandable meshes become more uniform.\nDisable it for stability at a great distance from the center of the world.", True))
    opts.Add(BoolVariable("lto", "Link-time optimization", False))

    opts.Update(env)
    env.Help(opts.GenerateHelpText(env))


# Additional compilation flags
def setup_defines_and_flags(env: SConsEnvironment, src_out: list):
    # Add more sources to `src_out` if needed

    if "release" in env["target"] and not env["force_enabled_dd3d"]:
        env.Append(CPPDEFINES=["DISABLE_DEBUG_RENDERING"])

    if env["native_api_enabled"]:
        env.Append(CPPDEFINES=["NATIVE_API_ENABLED"])
        if not COMMAND_LINE_TARGETS:
            gen_apis(None, None, env, src_out)

    if env["telemetry_enabled"]:
        tele_src = "editor/dst_modules/GDExtension/usage_time_reporter.cpp"
        if os.path.exists(os.path.join(src_folder, tele_src)):
            env.Append(
                CPPDEFINES=[
                    "TELEMETRY_ENABLED",
                    "UsageTimeReporterGodotObj=_UsageTimeReporterGodotObjDD3D",
                    'TELEMETRY_DST_FILE_KEY=\\"' + os.environ.get("TELEMETRY_DST_FILE_KEY", '\\"') + '\\"',
                    'TELEMETRY_APP_ID=\\"' + os.environ.get("TELEMETRY_DD3D_APP_ID", '\\"') + '\\"',
                    'TELEMETRY_HOST=\\"' + os.environ.get("TELEMETRY_DD3D_HOST", '\\"') + '\\"',
                ]
            )
            src_out.append(tele_src)
            print("Compiling with telemetry support!")
        else:
            print("No telemetry source file found.")
            env.Exit(1)

    if env["lto"]:
        if env.get("is_msvc", False):
            env.AppendUnique(CCFLAGS=["/GL"])
            env.AppendUnique(ARFLAGS=["/LTCG"])
            env.AppendUnique(LINKFLAGS=["/LTCG"])
        else:
            env.AppendUnique(CCFLAGS=["-flto"])
            env.AppendUnique(LINKFLAGS=["-flto"])

    if env["tracy_enabled"]:
        env.Append(CPPDEFINES=["TRACY_ENABLE", "TRACY_ON_DEMAND", "TRACY_DELAYED_INIT", "TRACY_MANUAL_LIFETIME"])
        src_out.append("thirdparty/tracy/public/TracyClient.cpp")

    if env["fix_precision_enabled"]:
        env.Append(CPPDEFINES=["FIX_PRECISION_ENABLED"])

    if not env["shader_world_coords_enabled"]:
        env.Append(CPPDEFINES=["DISABLE_SHADER_WORLD_COORDS"])

    if env.get("is_msvc", False):
        env.Append(LINKFLAGS=["/WX:NO"])

    if env["platform"] in ["linux"]:  # , "android"?
        env.Append(
            LINKFLAGS=[
                "-static-libgcc",
                "-static-libstdc++",
            ]
        )
    if env["platform"] == "android":
        env.Append(
            LIBS=[
                "log",
            ]
        )
    print()


def generate_sources_for_resources(env: SConsEnvironment, src_out: list):
    # Array of (path, is_text)
    editor_files = [
        ("images/icon_3d_32.png", False),
    ]
    lib_utils.generate_resources_cpp_h_files(
        editor_files,
        "DD3DEditorResources",
        src_folder,
        "editor_resources.gen",
        src_out if "editor" in env["target"] else [],
    )

    shared_files = [
        ("src/resources/extendable_meshes.gdshader", True),
        ("src/resources/wireframe_unshaded.gdshader", True),
        ("src/resources/billboard_unshaded.gdshader", True),
        ("src/resources/plane_unshaded.gdshader", True),
    ]
    lib_utils.generate_resources_cpp_h_files(shared_files, "DD3DResources", src_folder, "shared_resources.gen", src_out)

    print("The generation of C++ sources with the contents of resources has been completed")
    print()


def apply_patches(target, source, env: SConsEnvironment):
    return lib_utils_external.apply_git_patches(env, patches_to_apply, "godot-cpp")


def gen_apis(target, source, env: SConsEnvironment, src_out: list = []):
    return lib_utils_gen_dd3d_api.gen_apis(env, "src/native_api/templates/c_api.cpp", os.path.join(os.path.dirname(env["addon_output_dir"]), "native_api"), src_out)


def get_android_toolchain() -> str:
    sys.path.insert(0, "godot-cpp/tools")
    import android

    sys.path.pop(0)
    return os.path.join(android.get_android_ndk_root(env), "build/cmake/android.toolchain.cmake")


# Additional build of the projects via CMake
# def build_cmake(target, source, env: SConsEnvironment):
#    extra_flags = []
#    if env["platform"] in ["macos", "ios"]:
#        extra_flags += ["-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64", "-DCMAKE_OSX_DEPLOYMENT_TARGET=10.14"]
#    if env["platform"] in ["android"]:
#        extra_flags += [f"-DCMAKE_TOOLCHAIN_FILE={get_android_toolchain()}"]
#    return lib_utils_external.cmake_build_project(env, "project", extra_flags)

env: SConsEnvironment = SConscript("godot-cpp/SConstruct")
env = env.Clone()

args = ARGUMENTS
additional_src = []
setup_options(env, args)
setup_defines_and_flags(env, additional_src)
generate_sources_for_resources(env, additional_src)

scons_cache_path = os.environ.get("SCONS_CACHE")
if scons_cache_path is None:
    # store all obj's in a dedicated folder
    env["SHOBJPREFIX"] = "#obj/"
else:
    env.CacheDir(scons_cache_path)
    env.Decider("MD5")

extra_tags = ""
if "release" in env["target"] and env["force_enabled_dd3d"]:
    extra_tags += ".enabled"

if not env["build_cpp_api_tests"]:
    lib_utils.get_library_object(
        env, project_name, lib_name, extra_tags, env["addon_output_dir"], src_folder, additional_src
    )

if env.get("precision", "single") == "double":
    extra_tags += ".double"

# Register console commands
env.Command("apply_patches", [], apply_patches)
env.Command("gen_apis", [], gen_apis)
# env.Command("build_cmake", [], build_cmake)


## CPP API TESTS

if env["build_cpp_api_tests"]:
    tests_src_folder = "tests_native_api/cpp"
    env.Append(CPPPATH=src_folder)
    env["SHOBJPREFIX"] = "#obj/"
    additional_tags = ""
    if env["platform"] == "web" and env.get("threads", True):
        additional_tags = ".threads"

    lib_filename = "lib{}.{}.{}.{}{}".format(
        "dd3d_cpp_api", env["platform"], env["target"], env["arch"], additional_tags
    )
    lib_filename = os.path.join(tests_src_folder, "addon_cpp_api_test", "libs", lib_filename + env["SHLIBSUFFIX"])

    shbin = env.Default(
        env.SharedLibrary(
            target=env.File(lib_filename),
            source=lib_utils.get_sources(env.Glob(os.path.join(tests_src_folder, "*.cpp")), "", "test_c_api"),
        )
    )
