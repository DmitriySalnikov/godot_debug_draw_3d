#!/usr/bin/env python

import lib_utils
env = SConscript("godot-cpp/SConstruct")
Default(lib_utils.gdnative_get_library_object(env, ARGUMENTS, lambda e, o: Help(o.GenerateHelpText(e))))
