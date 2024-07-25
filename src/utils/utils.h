#pragma once

#include "compiler.h"
#include "profiler.h"

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
#include <godot_cpp/classes/project_settings.hpp>
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
#define DEV_PRINT_STD(format, ...) Utils::_logv(false, false, format, ##__VA_ARGS__)
// Forced
#define DEV_PRINT_STD_F(format, ...) Utils::_logv(false, true, format, ##__VA_ARGS__)
#define DEV_PRINT_STD_ERR(format, ...) Utils::_logv(true, false, format, ##__VA_ARGS__)
// Forced
#define DEV_PRINT_STD_ERR_F(format, ...) Utils::_logv(true, true, format, ##__VA_ARGS__)
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

constexpr size_t INSTANCE_DATA_FLOAT_COUNT = ((sizeof(godot::Transform3D) + sizeof(godot::Color) /*Instance Color*/ + sizeof(godot::Color) /*Custom Data*/) / sizeof(real_t));

#define IS_EDITOR_HINT() Engine::get_singleton()->is_editor_hint()
#define SCENE_TREE() Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop())
#define SCENE_ROOT() (SCENE_TREE()->get_root())

#ifndef TRACY_ENABLE
#define LOCK_GUARD(_mutex) std::lock_guard<std::recursive_mutex> __guard(_mutex)
#else
#define LOCK_GUARD(_mutex) std::lock_guard<LockableBase(std::recursive_mutex)> __guard(_mutex)
#endif

#define PS() ProjectSettings::get_singleton()
#define DEFINE_SETTING(path, def, type)     \
	{                                       \
		if (!PS()->has_setting(path)) {     \
			PS()->set_setting(path, def);   \
		}                                   \
		Dictionary info;                    \
		info["name"] = path;                \
		info["type"] = type;                \
		PS()->add_property_info(info);      \
		PS()->set_initial_value(path, def); \
	}
#define DEFINE_SETTING_HINT(path, def, type, hint, hint_string) \
	{                                                           \
		if (!PS()->has_setting(path)) {                         \
			PS()->set_setting(path, def);                       \
		}                                                       \
		Dictionary info;                                        \
		info["name"] = path;                                    \
		info["type"] = type;                                    \
		info["hint"] = hint;                                    \
		info["hint_string"] = hint_string;                      \
		PS()->add_property_info(info);                          \
		PS()->set_initial_value(path, def);                     \
	}
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
#define DEFINE_SETTING_AND_GET_HINT(var, path, def, type, hint, hint_string) \
	{                                                                        \
		if (!PS()->has_setting(path)) {                                      \
			PS()->set_setting(path, def);                                    \
		}                                                                    \
		Dictionary info;                                                     \
		info["name"] = path;                                                 \
		info["type"] = type;                                                 \
		info["hint"] = hint;                                                 \
		info["hint_string"] = hint_string;                                   \
		PS()->add_property_info(info);                                       \
		PS()->set_initial_value(path, def);                                  \
	}                                                                        \
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

// HACK temp constants.

#define VEC3_ONE(comp) Vector3(comp, comp, comp)
const extern godot::Vector3 Vector3_ZERO;
const extern godot::Vector3 Vector3_INF;
const extern godot::Vector3 Vector3_ONE;
const extern godot::Vector3 Vector3_UP_OF_S;
const extern godot::Vector3 Vector3_UP_OF;
const extern godot::Vector3 Vector3_UP;
const extern godot::Vector3 Vector3_DOWN;
const extern godot::Vector3 Vector3_LEFT;
const extern godot::Vector3 Vector3_RIGHT;
const extern godot::Vector3 Vector3_BACK;
const extern godot::Vector3 Vector3_FORWARD;

const extern godot::Quaternion Quaternion_IDENTITY;

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

	static void _logv(bool p_err, bool p_force_print, const char *p_format, ...);
	static void print_logs();

#ifndef DISABLE_DEBUG_RENDERING
	static godot::Node *find_node_by_class(godot::Node *start_node, const godot::String &class_name);
	static godot::String get_scene_tree_as_string(godot::Node *start);
#endif

	static void get_godot_version(int *major, int *minor, int *patch, int *version_sum);
	/// Is the current version of godot within this range? [Inclusive]
	static bool is_current_godot_version_in_range(uint8_t min_major = 0, uint8_t min_minor = 0, uint8_t min_patch = 0, uint8_t max_major = 0, uint8_t max_minor = 0, uint8_t max_patch = 0);

	// p_custom_callable_to_connect is used here to avoid constantly creating Callable().bindv()
	template <class T>
	static bool connect_safe(T p_inst, const godot::StringName &p_signal, const godot::Callable &p_callable, const uint32_t &p_flags = 0, godot::Error *p_out_error = nullptr, std::function<godot::Callable()> p_custom_callable_to_connect = nullptr) {
		if (p_inst && !p_inst->is_connected(p_signal, p_callable)) {
			godot::Error err = p_inst->connect(p_signal, p_custom_callable_to_connect ? p_custom_callable_to_connect() : p_callable, p_flags);
			if (p_out_error)
				*p_out_error = err;
			return true;
		}
		return false;
	}

	template <class T>
	static bool disconnect_safe(T p_inst, const godot::StringName &p_signal, const godot::Callable &p_callable) {
		if (p_inst && p_inst->is_connected(p_signal, p_callable)) {
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
		ZoneScoped;

		TPool p;
		if (!arr.empty()) {
			auto *data = arr.data();
			if (data) {
				p.resize(arr.size());
				memcpy(p.ptrw(), data, sizeof(arr[0]) * arr.size());
			}
		}
		return p;
	}

	template <class TPool, class TContainer>
	static TPool convert_to_packed_array_diffrent_types(TContainer &arr) {
		ZoneScoped;

		TPool p;
		p.resize(1);
		long s = sizeof(p[0]);
		p.resize(0);

		if (!arr.empty()) {
			auto *data = arr.data();
			if (data) {
				p.resize(arr.size() * sizeof(arr[0]) / s);
				memcpy(p.ptrw(), data, sizeof(arr[0]) * arr.size());
			}
		}
		return p;
	}

	template <class TPool, class TContainer>
	static TPool convert_packed_array_to_diffrent_types(TContainer &arr) {
		ZoneScoped;

		TPool p;
		p.resize(1);
		long s = sizeof(p[0]);
		p.resize(0);

		if (!arr.is_empty()) {
			auto *data = arr.ptr();
			if (data) {
				p.resize(arr.size() * sizeof(arr[0]) / s);
				memcpy(p.ptrw(), data, sizeof(arr[0]) * arr.size());
			}
		}
		return p;
	}

	// TODO need to use make from API when it becomes possible
#pragma region HACK_FOR_DICTIONARIES
	template <class... Args>
	static godot::Dictionary make_dict(Args... args) {
		ZoneScoped;
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

#ifndef DISABLE_DEBUG_RENDERING
#ifndef WEB_ENABLED
#include <chrono>

class GodotScopedStopwatch {
	std::chrono::high_resolution_clock::time_point start_time;
	int64_t *m_time_val;
	bool m_add;

public:
	GodotScopedStopwatch(int64_t *p_time_val, bool p_add) {
		m_time_val = p_time_val;
		m_add = p_add;
		start_time = std::chrono::high_resolution_clock::now();
	}

	~GodotScopedStopwatch() {
		if (m_add)
			*m_time_val += static_cast<int64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count());
		else
			*m_time_val = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count());
	}
};
#else
GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/time.hpp>
GODOT_WARNING_RESTORE()

class GodotScopedStopwatch {
	uint64_t start_time;
	int64_t *m_time_val;
	bool m_add;

public:
	GodotScopedStopwatch(int64_t *p_time_val, bool p_add) {
		m_time_val = p_time_val;
		m_add = p_add;
		start_time = godot::Time::get_singleton()->get_ticks_usec();
	}

	~GodotScopedStopwatch() {
		if (m_add)
			*m_time_val += godot::Time::get_singleton()->get_ticks_usec() - start_time;
		else
			*m_time_val = godot::Time::get_singleton()->get_ticks_usec() - start_time;
	}
};
#endif

#define _GODOT_STOPWATCH_CONCAT_IMPL(name1, name2) name1##name2
#define _GODOT_STOPWATCH_CONCAT(name1, name2) _GODOT_STOPWATCH_CONCAT_IMPL(name1, name2)
#define GODOT_STOPWATCH(time_val) GodotScopedStopwatch _GODOT_STOPWATCH_CONCAT(godot_stopwatch_, __LINE__)(time_val, false)
#define GODOT_STOPWATCH_ADD(time_val) GodotScopedStopwatch _GODOT_STOPWATCH_CONCAT(godot_stopwatch_, __LINE__)(time_val, true)
#else
#define GODOT_STOPWATCH(time_val)
#define GODOT_STOPWATCH_ADD(time_val)
#endif