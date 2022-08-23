#!/usr/bin/env python3

import lib_utils
env = SConscript("godot-cpp/SConstruct")
opts = Variables([], ARGUMENTS)
lib_utils.setup_options(env, opts, lambda e: Help(opts.GenerateHelpText(e)))
Default(lib_utils.gdnative_get_library_object(env))
