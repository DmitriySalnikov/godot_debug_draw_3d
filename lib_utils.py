#!/usr/bin/env python3

import os
import json
from patches import unity_tools
from pathlib import Path

lib_name = "dd3d"
output_dir = Path("addons") / "debug_draw_3d" / "libs"
src_folder = "src"


def setup_options(env, arguments, gen_help):
    from SCons.Variables import Variables, BoolVariable, EnumVariable, PathVariable

    opts = Variables([], arguments)

    opts.Add(BoolVariable("force_enabled_dd3d", "Keep the rendering code in the release build", False))
    opts.Add(BoolVariable("lto", "Link-time optimization", False))
    opts.Add(PathVariable("addon_output_dir", "Path to the output directory", output_dir, PathVariable.PathIsDirCreate))
    opts.Update(env)

    gen_help(env, opts)


def setup_defines_and_flags(env):
    env.Append(CPPDEFINES=["GDEXTENSION_LIBRARY"])

    if "release" in env["target"] and not env["force_enabled_dd3d"]:
        env.Append(CPPDEFINES=["DISABLE_DEBUG_RENDERING"])

    if env["lto"]:
        if env.get("is_msvc", False):
            env.AppendUnique(CCFLAGS=["/GL"])
            env.AppendUnique(ARFLAGS=["/LTCG"])
            env.AppendUnique(LINKFLAGS=["/LTCG"])
        else:
            env.AppendUnique(CCFLAGS=["-flto"])
            env.AppendUnique(LINKFLAGS=["-flto"])


def is_file_locked(file_path):
    if not os.path.exists(file_path):
        return False
    try:
        with open(file_path, "a") as f:
            pass
    except IOError:
        return True
    return False


def msvc_pdb_rename(env, lib_full_name):
    new_name = (Path(env["addon_output_dir"]) / lib_full_name).as_posix()
    max_files = 256

    onlyfiles = [f for f in os.listdir(Path(env["addon_output_dir"])) if os.path.isfile(os.path.join(Path(env["addon_output_dir"]), f))]
    for of in onlyfiles:
        if of.endswith(".pdb") and of.startswith(lib_full_name):
            try:
                os.remove(Path(env["addon_output_dir"]) / of)
            except:
                pass

    pdb_name = ""
    for s in range(max_files):
        pdb_name = "{}_{}.pdb".format(new_name, s)
        if not is_file_locked(pdb_name):
            break

    env.Append(LINKFLAGS=["/PDB:" + pdb_name])


def get_sources(src):
    res = [src_folder + "/" + file for file in src]
    res = unity_tools.generate_unity_build(res, "dd3d_")

    return res


def replace_flag(arr, flag, new_flag):
    if flag in arr:
        arr.remove(flag)
    arr.append(new_flag)


def get_library_object(env, arguments=None, gen_help=None):
    if arguments != None and gen_help:
        setup_options(env, arguments, gen_help)
    setup_defines_and_flags(env)

    env.Append(CPPPATH=[src_folder])

    # replace_flag(env["CXXFLAGS"], "/std:c++17" if env.get("is_msvc", False) else "-std=c++17",
    #                      "/std:c++20" if env.get("is_msvc", False) else "-std=c++20")

    src = []
    with open(src_folder + "/default_sources.json") as f:
        src = json.load(f)

    # store all obj's in a dedicated folder
    env["SHOBJPREFIX"] = "#obj/"

    generate_sources_for_resources(env, src)

    # some additional tags
    additional_tags = ""
    if "release" in env["target"] and env["force_enabled_dd3d"]:
        additional_tags += ".enabled"

    library_full_name = "lib{}.{}.{}.{}{}".format(
        lib_name, env["platform"], env["target"], env["arch"], additional_tags
    )

    # using the library with `reloadable = true` and with the debugger block the PDB file,
    # so it needs to be renamed to something not blocked
    if env.get("is_msvc", False) and env["target"] != "template_release":
        msvc_pdb_rename(env, library_full_name)

    env.Default(
        env.SharedLibrary(
            target=env.File(Path(env["addon_output_dir"]) / (library_full_name + env["SHLIBSUFFIX"])),
            source=get_sources(src),
            SHLIBSUFFIX=env["SHLIBSUFFIX"],
        )
    )

    # Needed for easy reuse of this library in other build scripts
    # TODO: not tested at the moment. Probably need some changes in the C++ code
    env = env.Clone()
    env.Append(LIBPATH=[env.Dir(env["addon_output_dir"])])
    if env.get("is_msvc", False):
        env.Append(LIBS=[library_full_name.replace(".dll", ".lib")])
    else:
        env.Append(LIBS=[library_full_name])

    return env


def generate_sources_for_resources(env, src_out):
    # Array of (path, is_text)
    editor_files = [
        ("images/icon_3d_32.png", False),
    ]
    generate_resources_cpp_h_files(editor_files, "DD3DEditorResources", "editor_resources.gen", src_out if "editor" in env["target"] else [])

    shared_files = [
        ("src/resources/extendable_meshes.gdshader", True),
        ("src/resources/basic_unshaded.gdshader", True),
        ("src/resources/billboard_unshaded.gdshader", True),
    ]
    generate_resources_cpp_h_files(shared_files, "DD3DResources", "shared_resources.gen", src_out)


def generate_resources_cpp_h_files(input_files, namespace, output_no_ext, src_out):
    gen_dir = "gen/"
    out_dir = src_folder + "/" + gen_dir
    Path(out_dir).mkdir(parents=True, exist_ok=True)

    cpp_name = output_no_ext + ".cpp"
    h_name = output_no_ext + ".h"

    src_out.append(gen_dir + cpp_name)

    with open(out_dir + cpp_name, "w+") as cpp_file, open(out_dir + h_name, "w+") as h_file:
        h_file.write("#pragma once\n\n")
        h_file.write("#include <array>\n\n")
        h_file.write(f"namespace {namespace} {{\n")

        cpp_file.write(f"#include \"{h_name}\"\n\n")
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
                        print("Skipping file due to 'UnicodeDecodeError' exception: " +
                              (input_file_path).resolve().as_posix() + "\nException: " + str(e))
                        continue

                cpp_file.write(f"\tconst char *{file_name_escaped} = R\"({text_data})\";\n\n")

                h_file.write(f"\textern const char *{file_name_escaped};\n")
            else:
                with open(input_file_touple[0], "rb") as input_file:
                    binary_data = input_file.read()

                cpp_array = ", ".join([f"{byte}" for byte in binary_data])
                cpp_file.write(f"\tconst std::array<unsigned char, {len(binary_data)}> {file_name_escaped} = {{ {cpp_array} }};\n\n")

                h_file.write(f"\textern const std::array<unsigned char, {len(binary_data)}> {file_name_escaped};\n")

        h_file.write("}\n")
        cpp_file.write("}\n")
