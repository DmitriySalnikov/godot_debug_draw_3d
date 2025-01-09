#pragma once

// This file is generated!
//
// To create new instances of Ref<T> use std::make_shared<T>, e.g.:
// auto cfg = std::make_shared<DebugDraw3DConfig>();
//
// TODO reverse:
// Define DD3D_DISABLE_MISMATCH_CHECKS to disable signature mismatch checking and disable method existence checking.
#include "c_api_shared.hpp"

#if _MSC_VER
__pragma(warning(disable : 4244 26451 26495));
#endif
// GENERATOR_DD3D_API_INCLUDES
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#if _MSC_VER
__pragma(warning(default : 4244 26451 26495));
#endif

#ifndef ZoneScoped
#define ZoneScoped
#endif

struct _DD3D_Loader_ {
	static constexpr const char *log_prefix = "[DD3D] ";
	static constexpr const char *get_funcs_hash_name = "_get_native_functions_hash";
	static constexpr const char *get_funcs_name = "_get_native_functions";

	static godot::Object *&dd3d_cached() {
		static godot::Object *dd3d = nullptr;
		return dd3d;
	}

	static godot::Object *get_dd3d() {
		ZoneScoped;

		godot::Object *&dd3d_c = dd3d_cached();
		if (!dd3d_c && godot::Engine::get_singleton()->has_singleton("DebugDrawManager")) {
			godot::Object *dd3d = godot::Engine::get_singleton()->get_singleton("DebugDrawManager");

#ifndef DD3D_DISABLE_MISMATCH_CHECKS
			if (!dd3d->has_method(get_funcs_hash_name)) {
				godot::UtilityFunctions::printerr(log_prefix, get_funcs_hash_name, " not found!");
				return nullptr;
			}
#endif

#ifndef DD3D_DISABLE_MISMATCH_CHECKS
			if (!dd3d->has_method(get_funcs_name)) {
				godot::UtilityFunctions::printerr(log_prefix, get_funcs_name, " not found!");
				return nullptr;
			}
#endif
			dd3d_c = dd3d;
		}
		return dd3d_c;
	}

	template <typename func>
	static bool load_function(func &val, const char *name) {
		ZoneScoped;
		if (Object *dd3d = get_dd3d(); dd3d) {
			int64_t api_hash = dd3d->call(get_funcs_hash_name);

			// TODO add an actual comparison with the previous hash. It is useful in case of library reloading, but is it really useful for users?..
			if (api_hash != 0) {
				Dictionary api = dd3d->call(get_funcs_name);
				if (api.has(name)) {
					Dictionary func_dict = api[name];

#ifndef DD3D_DISABLE_MISMATCH_CHECKS
					String sign1 = func_dict["signature"];
					String sign2 = DD3DShared::get_function_signature(val);
					if (sign1 != sign2) {
						UtilityFunctions::printerr(log_prefix, "!!! FUNCTION SIGNATURE MISMATCH !!!\n\tFunc name:\t", name, "\n\tDD3D Sign:\t", sign1, "\n\tClient Sign:\t", sign2);
						return false;
					}
#endif
					val = (func)(int64_t)func_dict["ptr"];
					return true;
				} else {
					UtilityFunctions::printerr(log_prefix, "!!! FUNCTION NOT FOUND !!! function name: ", name);
					return false;
				}
			}
		}
		return false;
	}
};

#define LOAD_AND_CALL_FUNC_POINTER(_name, ...)              \
	ZoneScoped;                                             \
	if (!_name) {                                           \
		if (!_DD3D_Loader_::load_function(_name, #_name)) { \
			return;                                         \
		}                                                   \
	}                                                       \
	if (_name) {                                            \
		_name(__VA_ARGS__);                                 \
	}

#define LOAD_AND_CALL_FUNC_POINTER_SELFRET(_name, ...)      \
	ZoneScoped;                                             \
	if (!_name) {                                           \
		if (!_DD3D_Loader_::load_function(_name, #_name)) { \
			return this;                                    \
		}                                                   \
	}                                                       \
	if (_name) {                                            \
		_name(__VA_ARGS__);                                 \
	}

#define LOAD_AND_CALL_FUNC_POINTER_RET(_name, _def_ret_val, ...) \
	ZoneScoped;                                                  \
	if (!_name) {                                                \
		if (!_DD3D_Loader_::load_function(_name, #_name)) {      \
			return _def_ret_val;                                 \
		}                                                        \
	}                                                            \
	if (_name) {                                                 \
		return _name(__VA_ARGS__);                               \
	}                                                            \
	return _def_ret_val

#define LOAD_AND_CALL_FUNC_POINTER_RET_REF_TO_SHARED(_name, _cls, _def_ret_val, ...) \
	ZoneScoped;                                                                      \
	if (!_name) {                                                                    \
		if (!_DD3D_Loader_::load_function(_name, #_name)) {                          \
			return _def_ret_val;                                                     \
		}                                                                            \
	}                                                                                \
	if (_name) {                                                                     \
		return std::make_shared<_cls>(_name(__VA_ARGS__));                           \
	}                                                                                \
	return _def_ret_val

// GENERATOR_DD3D_API_FORWARD_DECLARATIONS

// Start of the generated API
// GENERATOR_DD3D_API_FUNCTIONS
// End of the generated API

#undef LOAD_FUNC_POINTER