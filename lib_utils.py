#!/usr/bin/env python3

import os
import json
import re
from patches import unity_tools

lib_readable_name = "Debug Draw 3D"
lib_name = "dd3d"
output_dir = os.path.join("addons", "debug_draw_3d", "libs")
src_folder = "src"


def setup_options(env, arguments, gen_help):
    from SCons.Variables import Variables, BoolVariable, EnumVariable, PathVariable

    opts = Variables([], arguments)

    opts.Add(BoolVariable("telemetry_enabled", "Enable the telemetry module", False))
    opts.Add(BoolVariable("tracy_enabled", "Enable tracy profiler", False))
    opts.Add(BoolVariable("force_enabled_dd3d", "Keep the rendering code in the release build", False))
    opts.Add(BoolVariable("lto", "Link-time optimization", False))
    opts.Add(PathVariable("addon_output_dir", "Path to the output directory", output_dir, PathVariable.PathIsDirCreate))
    opts.Update(env)

    gen_help(env, opts)


def setup_defines_and_flags(env, src_out):
    if "release" in env["target"] and not env["force_enabled_dd3d"]:
        env.Append(CPPDEFINES=["DISABLE_DEBUG_RENDERING"])

    if env["telemetry_enabled"]:
        tele_src = "src/editor/my_telemetry_modules/GDExtension/usage_time_reporter.cpp"
        if os.path.exists(tele_src):
            env.Append(CPPDEFINES=["TELEMETRY_ENABLED"])
            src_out.append(tele_src)
            print("Compiling with telemetry support!")
        else:
            print("No telemetry source file found. telemetry_enabled will be ignored!")

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
        src_out.append("src/thirdparty/tracy/public/TracyClient.cpp")

    if env["platform"] == "linux":
        env.Append(
            LINKFLAGS=[
                "-static-libgcc",
                "-static-libstdc++",
            ]
        )
    print()


def get_sources(src):
    res = [src_folder + "/" + file for file in src]
    res = unity_tools.generate_unity_build(res, "dd3d_")
    return res


def get_library_object(env, arguments=None, gen_help=None):
    if arguments != None and gen_help:
        setup_options(env, arguments, gen_help)
    additional_src = []
    setup_defines_and_flags(env, additional_src)
    env.Append(CPPPATH=src_folder)

    src = []
    with open(src_folder + "/default_sources.json") as f:
        src = json.load(f)

    scons_cache_path = os.environ.get("SCONS_CACHE")
    if scons_cache_path is None:
        # store all obj's in a dedicated folder
        env["SHOBJPREFIX"] = "#obj/"

    generate_sources_for_resources(env, src)

    # some additional tags
    additional_tags = ""
    if "release" in env["target"] and env["force_enabled_dd3d"]:
        additional_tags += ".enabled"

    library_name = "lib{}.{}.{}.{}{}".format(lib_name, env["platform"], env["target"], env["arch"], additional_tags)
    library_full_path = os.path.join(env["addon_output_dir"], library_name + env["SHLIBSUFFIX"])

    # using the library with `reloadable = true` and with the debugger block the PDB file,
    # so it needs to be renamed to something not blocked
    if env.get("is_msvc", False) and env["target"] != "template_release":
        msvc_pdb_rename(env, library_full_path)

    if env["platform"] != "macos":
        env.Default(
            env.SharedLibrary(
                target=env.File(library_full_path),
                source=additional_src + get_sources(src),
                SHLIBSUFFIX=env["SHLIBSUFFIX"],
            )
        )
    else:
        generate_framework_folder(env, library_name)
        env.Default(
            env.SharedLibrary(
                target=env.File(os.path.join(env["addon_output_dir"], library_name + ".framework", library_name)),
                source=additional_src + get_sources(src),
                SHLIBSUFFIX="",
            )
        )

    # Needed for easy reuse of this library in other build scripts
    # TODO: not tested at the moment. Probably need some changes in the C++ code
    env = env.Clone()
    env.Append(LIBPATH=[env.Dir(env["addon_output_dir"])])
    if env.get("is_msvc", False):
        env.Append(LIBS=[library_full_path.replace(".dll", ".lib")])
    else:
        env.Append(LIBS=[library_full_path])

    return env


def get_library_version():
    with open("src/version.h", "r") as f:
        header_content = f.read()

    major_match = re.search(r"#define .*_MAJOR (\d+)", header_content)
    minor_match = re.search(r"#define .*_MINOR (\d+)", header_content)
    patch_match = re.search(r"#define .*_PATCH (\d+)", header_content)

    if major_match:
        major_value = int(major_match.group(1))
    else:
        major_value = 0

    if minor_match:
        minor_value = int(minor_match.group(1))
    else:
        minor_value = 0

    if patch_match:
        patch_value = int(patch_match.group(1))
    else:
        patch_value = 0

    return f"{major_value}.{minor_value}.{patch_value}"


def generate_sources_for_resources(env, src_out):
    # Array of (path, is_text)
    editor_files = [
        ("images/icon_3d_32.png", False),
    ]
    generate_resources_cpp_h_files(
        editor_files, "DD3DEditorResources", "editor_resources.gen", src_out if "editor" in env["target"] else []
    )

    shared_files = [
        ("src/resources/extendable_meshes.gdshader", True),
        ("src/resources/wireframe_unshaded.gdshader", True),
        ("src/resources/billboard_unshaded.gdshader", True),
        ("src/resources/plane_unshaded.gdshader", True),
    ]
    generate_resources_cpp_h_files(shared_files, "DD3DResources", "shared_resources.gen", src_out)

    print("The generation of C++ sources with the contents of resources has been completed")
    print()


def generate_resources_cpp_h_files(input_files, namespace, output_no_ext, src_out):
    """
    input_files
        (path, is_text)
    namespace : str
        source code namespace
    output_no_ext : str
        name of the source pair (cpp/h)
    src_out : list of str
        array which will be extended with generated sources
    """

    print(f"Generating sources '{output_no_ext}.[cpp/h]' with content from resources:")
    print("\n".join([f"{i[0]} as {'text' if i[1] else 'binary'}" for i in input_files]))
    print()

    gen_dir = "gen/"
    out_dir = src_folder + "/" + gen_dir
    os.makedirs(out_dir, exist_ok=True)

    cpp_name = output_no_ext + ".cpp"
    h_name = output_no_ext + ".h"

    src_out.append(gen_dir + cpp_name)

    with open(out_dir + cpp_name, "w+") as cpp_file, open(out_dir + h_name, "w+") as h_file:
        h_file.write("#pragma once\n\n")
        h_file.write("#include <array>\n\n")
        h_file.write(f"namespace {namespace} {{\n")

        cpp_file.write(f'#include "{h_name}"\n\n')
        cpp_file.write(f"namespace {namespace} {{\n")

        for input_file_touple in input_files:
            is_text = input_file_touple[1]
            input_file_path = input_file_touple[0]
            file_name_escaped = input_file_path.replace(".", "_").replace("/", "_").replace("\\", "_").replace(":", "_")

            if is_text:
                try:
                    with open(input_file_path, "r") as file:
                        text_data = file.read()
                except UnicodeDecodeError:
                    try:
                        with open(input_file_path, "r", encoding="utf-8") as file:
                            text_data = file.read()
                    except UnicodeDecodeError as e:
                        print(
                            "Skipping file due to 'UnicodeDecodeError' exception: "
                            + (input_file_path).resolve().as_posix()
                            + "\nException: "
                            + str(e)
                        )
                        continue

                cpp_file.write(f'\tconst char *{file_name_escaped} = R"({text_data})";\n\n')

                h_file.write(f"\textern const char *{file_name_escaped};\n")
            else:
                with open(input_file_touple[0], "rb") as input_file:
                    binary_data = input_file.read()

                cpp_array = ", ".join([f"{byte}" for byte in binary_data])
                cpp_file.write(
                    f"\tconst std::array<unsigned char, {len(binary_data)}> {file_name_escaped} = {{ {cpp_array} }};\n\n"
                )

                h_file.write(f"\textern const std::array<unsigned char, {len(binary_data)}> {file_name_escaped};\n")

        h_file.write("}\n")
        cpp_file.write("}\n")


def generate_framework_folder(env, library_name):
    min_version = env.get("macos_deployment_target", "10.14")
    lib_version = get_library_version()

    info_plist = f"""<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleDevelopmentRegion</key>
	<string>en</string>
	<key>CFBundleExecutable</key>
	<string>{library_name}</string>
	<key>CFBundleName</key>
	<string>{lib_readable_name}</string>
	<key>CFBundleDisplayName</key>
	<string>{lib_readable_name}</string>
	<key>CFBundleIdentifier</key>
	<string>ru.dmitriysalnikov.{lib_name}</string>
	<key>NSHumanReadableCopyright</key>
	<string>Copyright (c) Dmitriy Salnikov.</string>
	<key>CFBundleVersion</key>
	<string>{lib_version}</string>
	<key>CFBundleShortVersionString</key>
	<string>{lib_version}</string>
	<key>CFBundlePackageType</key>
	<string>FMWK</string>
	<key>CSResourcesFileMapped</key>
	<true/>
	<key>DTPlatformName</key>
	<string>macosx</string>
	<key>LSMinimumSystemVersion</key>
	<string>{min_version}</string>
</dict>
</plist>
    """

    res_folder = os.path.join(env["addon_output_dir"], library_name + ".framework", "Resources")
    os.makedirs(res_folder, exist_ok=True)

    with open(os.path.join(res_folder, "Info.plist"), "w") as info_plist_file:
        info_plist_file.write(info_plist)


def replace_flag(arr, flag, new_flag):
    if flag in arr:
        arr.remove(flag)
    arr.append(new_flag)


# A utility function for getting the name of an unblocked file
def get_unblocked_file_name(original_file_path, new_file_ext, max_files=256, keep_newest_one=True):
    lib_dir = os.path.normpath(os.path.dirname(original_file_path))
    lib_name = os.path.splitext(os.path.basename(original_file_path))[0]

    # Collect all matching files
    found_files = [
        f
        for f in os.listdir(lib_dir)
        if os.path.isfile(os.path.join(lib_dir, f)) and f.startswith(lib_name) and f.endswith("." + new_file_ext)
    ]
    found_files = sorted(found_files, key=lambda x: os.path.getmtime(os.path.join(lib_dir, x)))

    # Clean up the old files if possible, except for the newest one
    if found_files:
        if keep_newest_one:
            found_files.pop()
        for f in found_files:
            try:
                os.remove(os.path.join(lib_dir, f))
            except:
                pass

    # Search for a unblocked file name
    file_name = ""
    for s in range(max_files):
        file_name = "{}_{}.{}".format(os.path.join(lib_dir, lib_name), s, new_file_ext)
        if not os.path.exists(file_name):
            break
        try:
            with open(file_name, "a") as f:
                pass
        except IOError:
            continue
        break

    return file_name


# This is necessary to support debugging and Hot-Reload at the same time when building using MSVC
def msvc_pdb_rename(env, full_lib_path):
    pdb_name = get_unblocked_file_name(full_lib_path, "pdb")
    print("New path to the PDB: " + pdb_name)
    # explicit assignment of the PDB name
    env.Append(LINKFLAGS=["/PDB:" + pdb_name])
