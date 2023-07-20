#!/usr/bin/env python

import lib_utils
env = SConscript("godot-cpp/SConstruct")
new_env = lib_utils.get_library_object(env.Clone(), ARGUMENTS, lambda e, o: Help(o.GenerateHelpText(e)))
Return("new_env")
