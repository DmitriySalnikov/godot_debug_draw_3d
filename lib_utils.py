#!/usr/bin/env python3

import json
from pathlib import Path

lib_name = "debug_draw_3d"
output_dir = Path("addons") / lib_name / "libs"
src_folder = "src"


def setup_options(env, arguments, gen_help):
    from SCons.Variables import Variables, BoolVariable, EnumVariable, PathVariable
    opts = Variables([], arguments)

    opts.Add(PathVariable("addon_output_dir", "Path to the output directory",
             output_dir / env["platform"], PathVariable.PathIsDirCreate))
    opts.Update(env)

    gen_help(env, opts)


def gdnative_setup_default_cpp_defines(env):
    env.Append(CPPDEFINES=["GDEXTENSION_LIBRARY"])


def gdnative_get_sources(src):
    return [src_folder + "/" + file for file in src]


def gdnative_replace_flag(arr, flag, new_flag):
    if flag in arr:
        arr.remove(flag)
    arr.append(new_flag)


def gdnative_get_library_object(env, arguments=None, gen_help=None):
    if arguments != None and gen_help:
        setup_options(env, arguments, gen_help)
    gdnative_setup_default_cpp_defines(env)

    env.Append(CPPPATH=[src_folder])

    gdnative_replace_flag(env["CXXFLAGS"], "/std:c++17" if env.get("is_msvc", False) else "-std=c++17",
                          "/std:c++20" if env.get("is_msvc", False) else "-std=c++20")

    src = []
    with open(src_folder + "/default_sources.json") as f:
        src = json.load(f)

    # store all obj's in a dedicated folder
    env["SHOBJPREFIX"] = "#obj/"

    library = env.SharedLibrary(
        (Path(env["addon_output_dir"]) / lib_name).as_posix() + ".{}.{}.{}{}".format(
            env["platform"], env["target"], env["arch"], env["SHLIBSUFFIX"]
        ),
        source=gdnative_get_sources(src),
        SHLIBSUFFIX=env["SHLIBSUFFIX"]
    )

    return library
