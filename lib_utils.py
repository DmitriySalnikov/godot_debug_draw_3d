#!/usr/bin/env python3

import json

lib_name = 'debug_draw_3d'
src_folder = lib_name

def setup_options(env, opts, gen_help):
    from SCons.Variables import BoolVariable, EnumVariable

    #opts.Add(BoolVariable("use_lto", "Use link-time optimization", False))
    opts.Update(env)
    gen_help(env)

def setup_default_cpp_defines(env):
    pass
    #if env['godot_remote_no_default_resources']:
    #    env.Append(CPPDEFINES=['NO_GODOTREMOTE_DEFAULT_RESOURCES'])

def gdnative_get_sources(src):
    return [src_folder + "/" + file for file in src]

def gdnative_get_library_object(env):
    setup_default_cpp_defines(env)
    env.Append(CPPDEFINES=['GDNATIVE_LIBRARY'])

    if env['target'] == 'debug':
        env.Append(CPPDEFINES=['DEBUG_ENABLED'])
    
    env.Append(CPPPATH=[src_folder])

    #######################################################
    # library definition

    src = []
    with open(src_folder + '/default_sources.json') as f:
        src = json.load(f)

    env["SHOBJPREFIX"] = "#../obj/"

    library = env.SharedLibrary(
        "bin/" + lib_name + ".{}.{}.{}{}".format(
            env["platform"], env["target"], env["arch_suffix"], env["SHLIBSUFFIX"]
        ),
        source=gdnative_get_sources(src),
        SHLIBSUFFIX=env["SHLIBSUFFIX"]
    )

    return library
