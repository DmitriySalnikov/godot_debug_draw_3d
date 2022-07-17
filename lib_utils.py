#!/usr/bin/env python3

from shutil import copyfile
import os
import json

lib_name = 'godot_qoi'
src_folder = lib_name

# 'headers_dir' option in SConstruct must be correct: 'godot-cpp/godot-headers'
# 'binding_generator' should be commented out or replaced by 'pass'

def setup_options(env, opts, gen_help):
    from SCons.Variables import BoolVariable, EnumVariable

    #opts.Add(BoolVariable("godot_qoi_livepp", "Live++ support... Windows only", False))
    opts.Update(env)

    gen_help(env)

def setup_default_cpp_defines(env):
    pass
    #if env['godot_remote_no_default_resources']:
    #    env.Append(CPPDEFINES=['NO_GODOTREMOTE_DEFAULT_RESOURCES'])

def gdnative_get_sources(src):
    return [src_folder + "/" + file for file in src]

# this can replace everything after 'sources = []' by Default(lib_utils.gdnative_get_library_object(env))
def gdnative_get_library_object(env):
    setup_default_cpp_defines(env)
    env.Append(CPPDEFINES=['GDNATIVE_LIBRARY'])

    #######################################################
    # platform specific

    if env['platform'] in ['x11', 'linux', 'freebsd']:
        env['SHLIBSUFFIX'] = '.so'

    elif env['platform'] == 'osx':
        env['SHLIBSUFFIX'] = '.dylib'

    elif env['platform'] == 'ios':
        env['SHLIBSUFFIX'] = '.dylib'

    elif env['platform'] == 'windows':
        env['SHLIBSUFFIX'] = '.dll'

    elif env['platform'] == 'android':
        env['SHLIBSUFFIX'] = '.so'
        
        if env['target'] == 'debug':
            env.Append(CCFLAGS=['-Og'])
        elif env['target'] == 'release':
            env.Append(CCFLAGS=['-O3'])
    
    #######################################################
    
    arch_suffix = env['bits']
    if env['platform'] == 'android':
        arch_suffix = env['android_arch']
    elif env['platform'] == 'ios':
        arch_suffix = env['ios_arch']
    elif env['platform'] == 'osx':
        if env['macos_arch'] != 'universal':
            arch_suffix = env['macos_arch']
    elif env['platform'] == 'javascript':
        arch_suffix = 'wasm'

    env.Append(CPPPATH=[
        'godot-cpp/godot-headers',
        src_folder,
        'godot-cpp/include',
        'godot-cpp/include/gen',
        'godot-cpp/include/core',
    ])

    src = []
    with open(src_folder + '/default_sources.json') as f:
        src = json.load(f)

    #######################################################
    if env['target'] == 'debug':
        env.Append(CPPDEFINES=['DEBUG_ENABLED'])
    
    env.Append(LIBPATH=['#godot-cpp/bin/'])
    env.Append(LIBS=[
            'libgodot-cpp.{}.{}.{}{}'.format( # godot-cpp lib
            env['platform'],
            env['target'],
            arch_suffix,
            env['LIBSUFFIX']),
    ])

    return env.SharedLibrary(
        target='#bin/' + lib_name + '.{}.{}.{}{}'.format(
        env['platform'],
        env['target'],
        arch_suffix,
        env['SHLIBSUFFIX']
        #env['LIBSUFFIX']
        ), source=gdnative_get_sources(src)
    )
