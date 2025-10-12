#!/usr/bin/env python

from SCons.Script import SConscript
from SCons.Script.SConscript import SConsEnvironment

import SCons, SCons.Script
import sys, os, platform, re
import lib_utils, lib_utils_external

# Fixing the encoding of the console
if platform.system() == "Windows":
    os.system("chcp 65001")

# Project config
project_name = "Debug Draw 3D"
lib_name = "dd3d"
default_output_dir = os.path.join("addons", "debug_draw_3d", "libs")
src_folder = "src"

# If necessary, add patches from the code
patches_to_apply = [
    "patches/godot_cpp_exclude_unused_classes.patch",  # Removes unused godot-cpp classes from the build process
    "patches/unity_build.patch",  # Speeds up the build by merging the source files. It can increase the size of assemblies.
]

print(
    f"If you add new source files (e.g. .cpp, .c), do not forget to specify them in '{src_folder}/default_sources.json'.\n\tOr add them to 'setup_defines_and_flags' inside 'SConstruct'."
)
print("To apply git patches, use 'scons apply_patches'.")
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

    opts.Add(BoolVariable("telemetry_enabled", "Enable the telemetry module", False))
    opts.Add(BoolVariable("tracy_enabled", "Enable tracy profiler", False))
    opts.Add(BoolVariable("force_enabled_dd3d", "Keep the rendering code in the release build", False))
    opts.Add(
        BoolVariable(
            "fix_precision_enabled",
            "Fix precision errors at greater distances, utilizing more CPU resources.\nApplies only in combination with 'precision=double'",
            True,
        )
    )
    opts.Add(
        BoolVariable(
            "shader_world_coords_enabled",
            "Use world coordinates in shaders, if applicable.\nExpandable meshes become more uniform.\nDisable it for stability at a great distance from the center of the world.",
            True,
        )
    )

    opts.Update(env)
    env.Help(opts.GenerateHelpText(env))


# Additional compilation flags
def setup_defines_and_flags(env: SConsEnvironment, src_out):
    # Add more sources to `src_out` if needed

    if "release" in env["target"] and not env["force_enabled_dd3d"]:
        env.Append(CPPDEFINES=["DISABLE_DEBUG_RENDERING"])

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

    if env["lto"] != "none":
        if env.get("is_msvc", False):
            env.AppendUnique(CCFLAGS=["/GL"])
            env.AppendUnique(ARFLAGS=["/LTCG"])
            env.AppendUnique(LINKFLAGS=["/LTCG"])
        else:
            env.AppendUnique(CCFLAGS=["-flto"])
            env.AppendUnique(LINKFLAGS=["-flto"])

    if env["tracy_enabled"]:
        env.Append(CPPDEFINES=["TRACY_ENABLE", "TRACY_ON_DEMAND", "TRACY_DELAYED_INIT", "TRACY_MANUAL_LIFETIME"])
        src_out.append("utils/TracyClientCustom.cpp")

    if env["fix_precision_enabled"]:
        env.Append(CPPDEFINES=["FIX_PRECISION_ENABLED"])

    if not env["shader_world_coords_enabled"]:
        env.Append(CPPDEFINES=["DISABLE_SHADER_WORLD_COORDS"])

    if env.get("is_msvc", False):
        env.Append(CCFLAGS=["/GF"])  # Eliminate Duplicate Strings
        # env.Append(LINKFLAGS=["/WX:NO"])

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
    if env["platform"] == "web":
        if env["dev_build"]:
            env.Append(LINKFLAGS=["-sASSERTIONS=1", "-gsource-map"])
    print()


def generate_doc_links():
    from xml.dom import minidom

    lib_version = lib_utils.get_library_version()
    docs_map = {
        "DebugDraw2D": {
            "link": f"https://dd3d.dmitriysalnikov.ru/docs/{lib_version}/classDebugDraw2D.html",
            "header": "src/2d/debug_draw_2d.h",
        },
        "DebugDraw2DConfig": {
            "link": f"https://dd3d.dmitriysalnikov.ru/docs/{lib_version}/classDebugDraw2DConfig.html",
            "header": "src/2d/config_2d.h",
        },
        "DebugDraw2DStats": {
            "link": f"https://dd3d.dmitriysalnikov.ru/docs/{lib_version}/classDebugDraw2DStats.html",
            "header": "src/2d/stats_2d.h",
        },
        "DebugDraw3D": {
            "link": f"https://dd3d.dmitriysalnikov.ru/docs/{lib_version}/classDebugDraw3D.html",
            "header": "src/3d/debug_draw_3d.h",
        },
        "DebugDraw3DConfig": {
            "link": f"https://dd3d.dmitriysalnikov.ru/docs/{lib_version}/classDebugDraw3DConfig.html",
            "header": "src/3d/config_3d.h",
        },
        "DebugDraw3DScopeConfig": {
            "link": f"https://dd3d.dmitriysalnikov.ru/docs/{lib_version}/classDebugDraw3DScopeConfig.html",
            "header": "src/3d/config_scope_3d.h",
        },
        "DebugDraw3DStats": {
            "link": f"https://dd3d.dmitriysalnikov.ru/docs/{lib_version}/classDebugDraw3DStats.html",
            "header": "src/3d/stats_3d.h",
        },
        "DebugDrawManager": {
            "link": f"https://dd3d.dmitriysalnikov.ru/docs/{lib_version}/classDebugDrawManager.html",
            "header": "src/debug_draw_manager.h",
        },
    }
    docs_text = ""

    def get_parent_class(cls):
        res = re.findall(r"GDCLASS\((.*), (.*)\)", lib_utils.read_all_text(docs_map[cls]["header"]))
        for m in res:
            if m[0] == cls:
                return m[1]
        raise ModuleNotFoundError

    for k in docs_map:
        dom = minidom.Document()
        cls = dom.createElement("class")
        cls.setAttribute("name", k)
        cls.setAttribute("inherits", get_parent_class(k))
        cls.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance")
        cls.setAttribute(
            "xsi:noNamespaceSchemaLocation", "https://raw.githubusercontent.com/godotengine/godot/master/doc/class.xsd"
        )
        dom.appendChild(cls)

        brief = dom.createElement("brief_description")
        btxt = dom.createTextNode("DebugDraw3D's Class")
        brief.appendChild(btxt)
        cls.appendChild(brief)

        tutor = dom.createElement("tutorials")
        link = dom.createElement("link")
        link.setAttribute("title", "Online reference")
        dtxt = dom.createTextNode(docs_map[k]["link"])
        link.appendChild(dtxt)
        tutor.appendChild(link)
        cls.appendChild(tutor)

        docs_text += dom.toprettyxml(encoding="utf-8").decode("utf-8")

    return docs_text


def generate_sources_for_resources(env, src_out):
    if "editor" in env["target"]:
        # Array of (path, is_text)
        editor_files = [
            ("images/icon_3d_32.png", False),
        ]
        lib_utils.generate_resources_cpp_h_files(
            editor_files, "DD3DEditorResources", src_folder, "editor_resources.gen", src_out
        )

        lib_utils.generate_docs_cpp_file(generate_doc_links(), src_folder, "docs.gen.cpp", src_out)

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

args = SCons.Script.ARGUMENTS
additional_src = []
setup_options(env, args)
setup_defines_and_flags(env, additional_src)
generate_sources_for_resources(env, additional_src)

extra_tags = ""
if "release" in env["target"] and env["force_enabled_dd3d"]:
    extra_tags += ".enabled"

if env.get("precision", "single") == "double":
    extra_tags += ".double"

lib_utils.get_library_object(
    env, project_name, lib_name, extra_tags, env["addon_output_dir"], src_folder, additional_src
)

# Register console commands
env.Command("apply_patches", [], apply_patches)
# env.Command("build_cmake", [], build_cmake)
