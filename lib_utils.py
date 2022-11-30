#!/usr/bin/env python3

import json

lib_name = "debug_draw_3d"
src_folder = "src"

def setup_options(env, arguments, gen_help):
    from SCons.Variables import Variables, BoolVariable, EnumVariable
    opts = Variables([], arguments)

    #opts.Add(BoolVariable("godot_qoi_livepp", "Live++ support... Windows only", False))
    opts.Update(env)

    gen_help(env, opts)

def gdnative_setup_default_cpp_defines(env):
    env.Append(CPPDEFINES=["GDEXTENSION_LIBRARY"])

def gdnative_get_sources(src):
    return [src_folder + "/" + file for file in src]

def gdnative_get_library_object(env, arguments = None, gen_help = None):
    if arguments and gen_help: setup_options(env, arguments, gen_help)
    gdnative_setup_default_cpp_defines(env)

    env.Append(CPPPATH=[src_folder])

    src = []
    with open(src_folder + "/default_sources.json") as f:
        src = json.load(f)

    env["SHOBJPREFIX"] = "#obj/"

    library = env.SharedLibrary(
        "bin/" + lib_name + ".{}.{}.{}{}".format(
            env["platform"], env["target"], env["arch"], env["SHLIBSUFFIX"]
        ),
        source=gdnative_get_sources(src),
        SHLIBSUFFIX=env["SHLIBSUFFIX"]
    )

    return library
