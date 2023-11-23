#pragma once

#include "compiler.h"

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

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/builtin_types.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
GODOT_WARNING_RESTORE()

#pragma region PRINTING

#if DEV_ENABLED
#define DEV_PRINT(text, ...) godot::UtilityFunctions::print(FMT_STR(godot::Variant(text), ##__VA_ARGS__))
// TODO rework. rename to "Fast logs". use without DEV?
#define DEV_PRINT_STD(format, ...) Utils::logv(format, false, false, ##__VA_ARGS__)
// Forced
#define DEV_PRINT_STD_F(format, ...) Utils::logv(format, false, true, ##__VA_ARGS__)
#define DEV_PRINT_STD_ERR(format, ...) Utils::logv(format, true, false, ##__VA_ARGS__)
// Forced
#define DEV_PRINT_STD_ERR_F(format, ...) Utils::logv(format, true, true, ##__VA_ARGS__)
#else
#define DEV_PRINT(text, ...)
#define DEV_PRINT_STD(format, ...)
#define DEV_PRINT_STD_F(format, ...)
#define DEV_PRINT_STD_ERR(format, ...)
#define DEV_PRINT_STD_ERR_F(format, ...)
#endif

#define FMT_STR(str, ...) String(str).format(Array::make(__VA_ARGS__))
#define PRINT(text, ...) godot::UtilityFunctions::print(FMT_STR(godot::Variant(text), ##__VA_ARGS__))
#define PRINT_ERROR(text, ...) godot::_err_print_error(__FUNCTION__, godot::get_file_name_in_repository(__FILE__).utf8().get_data(), __LINE__, FMT_STR(godot::Variant(text).stringify(), ##__VA_ARGS__))
#define PRINT_WARNING(text, ...) godot::_err_print_error(__FUNCTION__, godot::get_file_name_in_repository(__FILE__).utf8().get_data(), __LINE__, FMT_STR(godot::Variant(text).stringify(), ##__VA_ARGS__), false, true)

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

#pragma region BINDING REGISTRATION

// For this to work, you need to define REG_CLASS_NAME before using it.
// e.g. #define REG_CLASS_NAME DebugDraw3D
// REG_METHOD("draw_box")

#define REG_METHOD(name, ...) ClassDB::bind_method(D_METHOD(#name, ##__VA_ARGS__), &REG_CLASS_NAME::name)

#define REG_PROP_BASE(name, getter, prop_info)                                                \
	ClassDB::bind_method(D_METHOD(NAMEOF(set_##name), "value"), &REG_CLASS_NAME::set_##name); \
	ClassDB::bind_method(D_METHOD(NAMEOF(getter##name)), &REG_CLASS_NAME::getter##name);      \
	ADD_PROPERTY(prop_info, NAMEOF(set_##name), NAMEOF(getter##name));
/// name, type, hint, hint_string, usage
#define REG_PROP(name, type, ...) REG_PROP_BASE(name, get_, PropertyInfo(type, #name, ##__VA_ARGS__))
/// name, type, hint, hint_string, usage
#define REG_PROP_BOOL(name, ...) REG_PROP_BASE(name, is_, PropertyInfo(Variant::BOOL, #name, ##__VA_ARGS__))

#pragma endregion !BINDING REGISTRATION

#define INSTANCE_DATA_FLOAT_COUNT ((sizeof(Transform3D) + sizeof(Color)) / sizeof(real_t))

#define IS_EDITOR_HINT() Engine::get_singleton()->is_editor_hint()
#define SCENE_TREE() Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop())
#define SCENE_ROOT() (SCENE_TREE()->get_root())
#define RS() RenderingServer::get_singleton()
#define TIME() Time::get_singleton()
#define ENGINE() Engine::get_singleton()

#define LOCK_GUARD(_mutex) std::lock_guard<std::recursive_mutex> __guard(_mutex)

#define PS() ProjectSettings::get_singleton()
#define DEFINE_SETTING_AND_GET(var, path, def, type) \
	{                                                \
		if (!PS()->has_setting(path)) {              \
			PS()->set_setting(path, def);            \
		}                                            \
		Dictionary info;                             \
		info["name"] = path;                         \
		info["type"] = type;                         \
		PS()->add_property_info(info);               \
		PS()->set_initial_value(path, def);          \
	}                                                \
	var = PS()->get_setting(path)
#define DEFINE_SETTING_READ_ONLY(path, def, type)  \
	{                                              \
		PS()->set_setting(path, def);              \
		Dictionary info;                           \
		info["name"] = path;                       \
		info["type"] = type;                       \
		/* Does not work in the ProjectSettings */ \
		info["usage"] = PROPERTY_USAGE_READ_ONLY;  \
		PS()->add_property_info(info);             \
		PS()->set_initial_value(path, def);        \
	}

#define ASSIGN_SINGLETON(class_name)                                         \
	if (!singleton) {                                                        \
		singleton = this;                                                    \
	} else {                                                                 \
		PRINT_ERROR("Only 1 instance of " NAMEOF(class_name) " is allowed"); \
	}

#define UNASSIGN_SINGLETON(class_name)                                              \
	if (singleton) {                                                                \
		if (singleton == this) {                                                    \
			singleton = nullptr;                                                    \
		} else {                                                                    \
			PRINT_ERROR("More than 1 " NAMEOF(class_name) " instance was created"); \
		}                                                                           \
	}

// TODO: temp constants. I didn't find them in gdnative api

const godot::Vector3 Vector3_ZERO = godot::Vector3(0, 0, 0);
const godot::Vector3 Vector3_ONE = godot::Vector3(1, 1, 1);
const godot::Vector3 Vector3_UP = godot::Vector3(0, 1, 0);
const godot::Vector3 Vector3_DOWN = godot::Vector3(0, -1, 0);
const godot::Vector3 Vector3_LEFT = godot::Vector3(-1, 0, 0);
const godot::Vector3 Vector3_RIGHT = godot::Vector3(1, 0, 0);
const godot::Vector3 Vector3_BACK = godot::Vector3(0, 0, 1);
const godot::Vector3 Vector3_FORWARD = godot::Vector3(0, 0, -1);

const godot::Quaternion Quaternion_IDENTITY = godot::Quaternion();

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
	static const char *root_settings_section;

	static void logv(const char *p_format, bool p_err, bool p_force_print, ...);
	static void print_logs();

	static godot::Node *find_node_by_class(godot::Node *start_node, const godot::String &class_name);
	static godot::String get_scene_tree_as_string(godot::Node *start);

	template <class T>
	static bool connect_safe(T p_inst, const godot::StringName &p_signal, const godot::Callable &p_callable, const uint32_t &p_flags = 0, godot::Error *p_out_error = nullptr) {
		if (godot::UtilityFunctions::is_instance_valid(p_inst) && !p_inst->is_connected(p_signal, p_callable)) {
			godot::Error err = p_inst->connect(p_signal, p_callable, p_flags);
			if (p_out_error)
				*p_out_error = err;
			return true;
		}
		return false;
	}

	template <class T>
	static bool disconnect_safe(T p_inst, const godot::StringName &p_signal, const godot::Callable &p_callable) {
		if (godot::UtilityFunctions::is_instance_valid(p_inst) && p_inst->is_connected(p_signal, p_callable)) {
			p_inst->disconnect(p_signal, p_callable);
			return true;
		}
		return false;
	}

	template <class T>
	static bool swap_connection(T p_inst_new, T p_inst_old, const godot::StringName &p_signal, const godot::Callable &p_callable, const uint32_t &p_flags = 0, godot::Error *p_out_error = nullptr) {
		disconnect_safe(p_inst_old, p_signal, p_callable);
		return connect_safe(p_inst_new, p_signal, p_callable, p_flags, p_out_error);
	}

	template <class TPool, class TContainer>
	static TPool convert_to_packed_array(TContainer &arr) {
		TPool p;
		if (arr.size() > 0) {
			p.resize(arr.size());
			memcpy(p.ptrw(), arr.data(), sizeof(arr[0]) * arr.size());
		}
		return p;
	}

	template <class TPool, class TContainer>
	static TPool convert_to_packed_array_diffrent_types(TContainer &arr) {
		TPool p;
		p.resize(1);
		long s = sizeof(p[0]);
		p.resize(0);

		if (arr.size() > 0) {
			p.resize(arr.size() * sizeof(arr[0]) / s);
			memcpy(p.ptrw(), arr.data(), sizeof(arr[0]) * arr.size());
		}
		return p;
	}

	template <class TPool, class TContainer>
	static TPool convert_packed_array_to_diffrent_types(TContainer &arr) {
		TPool p;
		p.resize(1);
		long s = sizeof(p[0]);
		p.resize(0);

		if (arr.size() > 0) {
			p.resize(arr.size() * sizeof(arr[0]) / s);
			memcpy(p.ptrw(), arr.ptr(), sizeof(arr[0]) * arr.size());
		}
		return p;
	}

	template <class T>
	static typename std::vector<T>::const_iterator find(const std::vector<T> &arr, const std::function<bool(T)> &search) {
		for (auto it = arr.begin(); it != arr.end(); it++) {
			if (search(*it))
				return it;
		}
		return arr.end();
	}

	template <class TVal, class TFun>
	static void remove_where(std::unordered_set<TVal> &set, const TFun &checker_bool_val, const std::function<void(TVal)> &deleter = nullptr) {
		std::unordered_set<TVal> to_remove;
		for (const TVal &t : set) {
			if (checker_bool_val(t)) {
				to_remove.insert(t);
			}
		}
		for (const TVal &t : to_remove) {
			if (deleter) {
				deleter(t);
			}
			set.erase(t);
		}
	}

	template <class TVal, class TFun>
	static int sum(const std::unordered_set<TVal> &set, const TFun &getter_int_val) {
		int res = 0;
		for (const TVal &t : set) {
			res += getter_int_val(t);
		}
		return res;
	}

	template <class TVal, class TFun>
	static std::vector<TVal> order_by(const std::unordered_set<TVal> &set, const TFun &comparator_bool_tval_tval) {
		std::vector<TVal> ordered(set.begin(), set.end());

		std::sort(ordered.begin(), ordered.end(), comparator_bool_tval_tval);
		return ordered;
	}

	// TODO: need to use make from API when it becomes possible
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
