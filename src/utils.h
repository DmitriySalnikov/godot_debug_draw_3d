#pragma once

#include "circular_buffer.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/builtin_types.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#define TEXT(s) #s

#if DEBUG_ENABLED && _MSC_VER
#ifndef _CRT_STRINGIZE
#define _CRT_STRINGIZE_(x) #x
#define _CRT_STRINGIZE(x) _CRT_STRINGIZE_(x)
#endif

// Expands to macro:
#define EXPAND_MACRO(x) __pragma(message(__FILE__ _CRT_STRINGIZE((__LINE__) \
																 : \nmacro\t) #x " expands to:\n" _CRT_STRINGIZE(x)))
#else
#define EXPAND_MACRO(x)
#endif

#if REAL_T_IS_DOUBLE
typedef godot::PackedFloat64Array PackedRealArray;
#else
typedef godot::PackedFloat32Array PackedRealArray;
#endif

#pragma region PRINTING

#if DEBUG_ENABLED
#define DEBUG_PRINT(text) godot::UtilityFunctions::print(godot::Variant(text))
#define DEBUG_PRINT_STD(format, ...) Utils::logv(format, false, false, ##__VA_ARGS__)
#define DEBUG_PRINT_STD_ERR(format, ...) Utils::logv(format, true, false, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(text)
#define DEBUG_PRINT_STD(format, ...)
#define DEBUG_PRINT_STD_ERR(format, ...)
#endif

#define PRINT(text) godot::UtilityFunctions::print(godot::Variant(text))
#define PRINT_ERROR(text) godot::_err_print_error(__FUNCTION__, godot::get_file_name_in_repository(__FILE__).utf8().get_data(), __LINE__, godot::Variant(text).stringify())
#define PRINT_WARNING(text) godot::_err_print_error(__FUNCTION__, godot::get_file_name_in_repository(__FILE__).utf8().get_data(), __LINE__, godot::Variant(text).stringify(), true)

namespace godot {
static String get_file_name_in_repository(const String &name) {
	if (name != "") {
		int64_t idx = name.find("src");
		if (idx != -1)
			return name.substr(idx, name.length());
	}
	return name;
}
} // namespace godot

#pragma endregion !PRINTING

// TODO temp constants. I didn't find them in gdnative api

const godot::Vector2 Vector2_ZERO = godot::Vector2(0, 0);
const godot::Vector2 Vector2_ONE = godot::Vector2(1, 1);
const godot::Vector2 Vector2_UP = godot::Vector2(0, -1);
const godot::Vector2 Vector2_DOWN = godot::Vector2(0, 1);
const godot::Vector2 Vector2_RIGHT = godot::Vector2(1, 0);
const godot::Vector2 Vector2_LEFT = godot::Vector2(-1, 0);

const godot::Vector3 Vector3_ZERO = godot::Vector3(0, 0, 0);
const godot::Vector3 Vector3_ONE = godot::Vector3(1, 1, 1);
const godot::Vector3 Vector3_UP = godot::Vector3(0, 1, 0);
const godot::Vector3 Vector3_DOWN = godot::Vector3(0, -1, 0);
const godot::Vector3 Vector3_LEFT = godot::Vector3(-1, 0, 0);
const godot::Vector3 Vector3_RIGHT = godot::Vector3(1, 0, 0);
const godot::Vector3 Vector3_BACK = godot::Vector3(0, 0, 1);
const godot::Vector3 Vector3_FORWARD = godot::Vector3(0, 0, -1);

const godot::Quaternion Quaternion_IDENTITY = godot::Quaternion();

#pragma region BINDING REGISTRATION

// For this to work, you need to define REG_CLASS_NAME before using it.
// e.g. #define REG_CLASS_NAME DebugDraw3D
// REG_METHOD("draw_box")

#define REG_METHOD(name) ClassDB::bind_method(D_METHOD(#name), &REG_CLASS_NAME::name)
#define REG_METHOD_ARGS(name, ...) ClassDB::bind_method(D_METHOD(#name, __VA_ARGS__), &REG_CLASS_NAME::name)

#define REG_PROP_BASE(name, type, getter)                                                   \
	ClassDB::bind_method(D_METHOD(TEXT(set_##name), "value"), &REG_CLASS_NAME::set_##name); \
	ClassDB::bind_method(D_METHOD(TEXT(getter##name)), &REG_CLASS_NAME::getter##name);      \
	ADD_PROPERTY(PropertyInfo(type, #name), TEXT(set_##name), TEXT(getter##name));
#define REG_PROP(name, type) REG_PROP_BASE(name, type, get_)
#define REG_PROP_BOOL(name) REG_PROP_BASE(name, Variant::BOOL, is_)

#pragma endregion !BINDING REGISTRATION

#define IS_EDITOR_HINT() Engine::get_singleton()->is_editor_hint()
#define SCENE_TREE() cast_to<SceneTree>(Engine::get_singleton()->get_main_loop())
#define SCENE_ROOT() (SCENE_TREE()->get_root())
#define RS() RenderingServer::get_singleton()
#define ENGINE() Engine::get_singleton()

#define LOCK_GUARD(_mutex) std::lock_guard<std::recursive_mutex> __guard(_mutex)

class Utils {
#if DEBUG_ENABLED
	struct LogData {
		size_t hash;
		std::string text;
		bool is_error;
		int repeat = 1;
	};

	static std::queue<LogData> log_buffer;
#endif

public:
	static void logv(const char *p_format, bool p_err, bool p_force_print, ...);
	static void print_logs();
	static godot::Node *find_node_by_class(godot::Node *start_node, godot::String *class_name);
	static godot::String get_scene_tree_as_string(godot::Node *start);

	template <class TPool, class TContainer>
	static TPool convert_to_pool_array(TContainer &arr) {
		TPool p;
		if (arr.size() > 0) {
			p.resize((int)arr.size());
			memcpy(p.ptrw(), arr.data(), sizeof(arr[0]) * arr.size());
		}
		return p;
	}

	template <class TPool, class TVal>
	static TPool convert_to_pool_array(const TVal *arr, const size_t &size) {
		TPool p;
		if (size > 0) {
			p.resize((int)size);
			memcpy(p.ptrw(), arr, sizeof(TVal) * size);
		}
		return p;
	}

	template <class T>
	static typename std::vector<T>::const_iterator find(const std::vector<T> &arr, std::function<bool(T)> search) {
		for (auto it = arr.begin(); it != arr.end(); it++) {
			if (search(*it))
				return it;
		}
		return arr.end();
	}

	template <class TVal, class TFun>
	static void remove_where(std::unordered_set<TVal> *set, const TFun &checker_bool_val, const std::function<void(TVal)> &deleter = nullptr) {
		std::unordered_set<TVal> to_remove;
		for (const TVal &t : *set) {
			if (checker_bool_val(t)) {
				to_remove.insert(t);
			}
		}
		for (const TVal &t : to_remove) {
			if (deleter) {
				deleter(t);
			}
			(*set).erase(t);
		}
	}

	template <class TVal, class TFun>
	static int sum(const std::unordered_set<TVal> *set, TFun getter_int_val) {
		int res = 0;
		for (const TVal &t : *set) {
			res += getter_int_val(t);
		}
		return res;
	}

	template <class TVal, class TFun>
	inline static std::vector<TVal> order_by(const std::unordered_set<TVal> *set, const TFun &comparator_bool_tval_tval) {
		std::vector<TVal> ordered((*set).begin(), (*set).end());

		std::sort(ordered.begin(), ordered.end(), comparator_bool_tval_tval);
		return ordered;
	}

	// TODO need to use make from API when it becomes possible
#pragma region HACK_FOR_DICTIONARIES
	template <class... Args>
	static godot::Dictionary make_dict(Args... args) {
		return dict_append_all(godot::Dictionary(), args...);
	}

	template <typename T, typename Key, typename Value>
	static T dict_append_all(T appendable, Key key, Value val) {
		appendable[key] = val;
		return appendable;
	}

	template <typename T, typename Key, typename Value, typename... Args>
	static T dict_append_all(T appendable, Key key, Value val, Args... args) {
		appendable[key] = val;
		return dict_append_all(appendable, args...);
	}

	template <typename T>
	static T dict_append_all(T appendable) {
		return appendable;
	}
#pragma endregion
};
