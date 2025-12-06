#pragma once

// This file is generated!
//
// To create new instances of Ref<T> use std::make_shared<T>, e.g.:
// auto cfg = std::make_shared<DebugDraw3DConfig>();
//
// TODO: reverse:
// Define DD3D_DISABLE_MISMATCH_CHECKS to disable signature mismatch checking and disable method existence checking.
#include "c_api_shared.hpp"

#if _MSC_VER
__pragma(warning(disable : 4244 26451 26495));
#endif
// GENERATOR_DD3D_API_INCLUDES
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#if _MSC_VER
__pragma(warning(default : 4244 26451 26495));
#endif

#ifndef ZoneScoped
#define _NoProfiler
#define ZoneScoped
#endif

struct _DD3D_Loader_ {
	static constexpr const char *log_prefix = "[DD3D] ";
	static constexpr const char *get_funcs_is_double_name = "_get_native_functions_is_double";
	static constexpr const char *get_funcs_hash_name = "_get_native_functions_hash";
	static constexpr const char *get_funcs_name = "_get_native_functions";

	enum class LoadingResult {
		None,
		Success,
		Failure
	};

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
			if (!dd3d->has_method(get_funcs_is_double_name)) {
				godot::UtilityFunctions::printerr(log_prefix, get_funcs_is_double_name, " not found!");
				return nullptr;
			}

#ifdef REAL_T_IS_DOUBLE
			bool is_double = true;
#else
			bool is_double = false;
#endif
			if ((bool)dd3d->call(get_funcs_is_double_name) != is_double) {
				godot::UtilityFunctions::printerr(log_prefix, "The precision of Vectors and Matrices of DD3D and the current library do not match!");
				return nullptr;
			}

			if (!dd3d->has_method(get_funcs_hash_name)) {
				godot::UtilityFunctions::printerr(log_prefix, get_funcs_hash_name, " not found!");
				return nullptr;
			}

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

			// TODO: add an actual comparison with the previous hash. It is useful in case of library reloading, but is it really useful for users?..
			if (api_hash != 0) {
				godot::Dictionary api = dd3d->call(get_funcs_name);
				if (api.has(name)) {
					godot::Dictionary func_dict = api[name];

#ifndef DD3D_DISABLE_MISMATCH_CHECKS
					godot::String sign1 = func_dict["signature"];
					godot::String sign2 = DD3DShared::FunctionSignature<func>::get();
					//godot::UtilityFunctions::print(log_prefix, "FUNCTION SIGNATURE\n\tFunc name:\t", name, "\n\tDD3D Sign:\t", sign1, "\n\tClient Sign:\t", sign2);

					if (sign1 != sign2) {
						godot::UtilityFunctions::printerr(log_prefix, "!!! FUNCTION SIGNATURE MISMATCH !!!\n\tFunc name:\t", name, "\n\tDD3D Sign:\t", sign1, "\n\tClient Sign:\t", sign2);
						return false;
					}
#endif
					val = (func)(int64_t)func_dict["ptr"];
					return true;
				} else {
					godot::UtilityFunctions::printerr(log_prefix, "!!! FUNCTION NOT FOUND !!! function name: ", name);
					return false;
				}
			}
		}
		return false;
	}
};


// TODO: Add DD3DNotLoaded state and wait for it to load
#define LOADING_RESULT static _DD3D_Loader_::LoadingResult _dd3d_loading_result = _DD3D_Loader_::LoadingResult::None
#define LOAD_FUNC_AND_STORE_RESULT(_name) _dd3d_loading_result = _DD3D_Loader_::load_function(_name, #_name) ? _DD3D_Loader_::LoadingResult::Success : _DD3D_Loader_::LoadingResult::Failure
#define IS_FIRST_LOADING _dd3d_loading_result == _DD3D_Loader_::LoadingResult::None
#define IS_LOADED_SUCCESSFULLY _dd3d_loading_result == _DD3D_Loader_::LoadingResult::Success
#define IS_FAILED_TO_LOAD _dd3d_loading_result == _DD3D_Loader_::LoadingResult::Failure

#define LOAD_AND_CALL_FUNC_POINTER(_name, ...) \
	ZoneScoped;                                \
	LOADING_RESULT;                            \
	if (IS_FIRST_LOADING) {                    \
		LOAD_FUNC_AND_STORE_RESULT(_name);     \
		if (IS_FAILED_TO_LOAD) {               \
			return;                            \
		}                                      \
	}                                          \
	if (IS_LOADED_SUCCESSFULLY) {              \
		_name(__VA_ARGS__);                    \
	}

#define LOAD_AND_CALL_FUNC_POINTER_SELFRET(_name, ...) \
	ZoneScoped;                                        \
	LOADING_RESULT;                                    \
	if (IS_FIRST_LOADING) {                            \
		LOAD_FUNC_AND_STORE_RESULT(_name);             \
		if (IS_FAILED_TO_LOAD) {                       \
			return shared_from_this();                 \
		}                                              \
	}                                                  \
	if (IS_LOADED_SUCCESSFULLY) {                      \
		_name(__VA_ARGS__);                            \
	}

#define LOAD_AND_CALL_FUNC_POINTER_RET(_name, _def_ret_val, ...) \
	ZoneScoped;                                                  \
	LOADING_RESULT;                                              \
	if (IS_FIRST_LOADING) {                                      \
		LOAD_FUNC_AND_STORE_RESULT(_name);                       \
		if (IS_FAILED_TO_LOAD) {                                 \
			return _def_ret_val;                                 \
		}                                                        \
	}                                                            \
	if (IS_LOADED_SUCCESSFULLY) {                                \
		return _name(__VA_ARGS__);                               \
	}                                                            \
	return _def_ret_val

#define LOAD_AND_CALL_FUNC_POINTER_RET_GODOT_REF(_name, godot_ref_type, _def_ret_val, ...)                                            \
	ZoneScoped;                                                                                                                       \
	LOADING_RESULT;                                                                                                                   \
	if (IS_FIRST_LOADING) {                                                                                                           \
		LOAD_FUNC_AND_STORE_RESULT(_name);                                                                                            \
		if (IS_FAILED_TO_LOAD) {                                                                                                      \
			return _def_ret_val;                                                                                                      \
		}                                                                                                                             \
	}                                                                                                                                 \
	if (IS_LOADED_SUCCESSFULLY) {                                                                                                     \
		return godot::Ref<godot_ref_type>(godot::Object::cast_to<godot_ref_type>(godot::ObjectDB::get_instance(_name(__VA_ARGS__)))); \
	}                                                                                                                                 \
	return _def_ret_val

#define LOAD_AND_CALL_FUNC_POINTER_RET_REF_TO_SHARED(_name, _cls, _def_ret_val, ...) \
	ZoneScoped;                                                                      \
	LOADING_RESULT;                                                                  \
	if (IS_FIRST_LOADING && !_name) {                                                \
		LOAD_FUNC_AND_STORE_RESULT(_name);                                           \
		if (IS_FAILED_TO_LOAD) {                                                     \
			return _def_ret_val;                                                     \
		}                                                                            \
	}                                                                                \
	if (IS_LOADED_SUCCESSFULLY) {                                                    \
		return std::make_shared<_cls>(_name(__VA_ARGS__));                           \
	}                                                                                \
	return _def_ret_val

// GENERATOR_DD3D_API_FORWARD_DECLARATIONS

// Start of the generated API
// GENERATOR_DD3D_API_FUNCTIONS
// End of the generated API

#undef LOADING_RESULT
#undef LOAD_FUNC_AND_STORE_RESULT
#undef IS_FIRST_LOADING
#undef IS_LOADED_SUCCESSFULLY
#undef IS_FAILED_TO_LOAD

#undef LOAD_AND_CALL_FUNC_POINTER
#undef LOAD_AND_CALL_FUNC_POINTER_SELFRET
#undef LOAD_AND_CALL_FUNC_POINTER_RET
#undef LOAD_AND_CALL_FUNC_POINTER_RET_GODOT_REF
#undef LOAD_AND_CALL_FUNC_POINTER_RET_REF_TO_SHARED

#ifdef _NoProfiler
#undef ZoneScoped
#endif
#undef _NoProfiler
