#pragma once

#include <memory>
#include <cstdlib>

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

#define ASSIGN_SINGLETON(class_name)                                          \
	if (!singleton) {                                                         \
		singleton = this;                                                     \
	} else {                                                                  \
		PRINT_ERROR("Only 1 instance of {0} is allowed", NAMEOF(class_name)); \
	}

#define UNASSIGN_SINGLETON(class_name)                                               \
	if (singleton) {                                                                 \
		if (singleton == this) {                                                     \
			singleton = nullptr;                                                     \
		} else {                                                                     \
			PRINT_ERROR("More than 1 {0} instance was created", NAMEOF(class_name)); \
		}                                                                            \
	}

struct malloc_free_delete {
	void operator()(void *x) { free(x); }
};

#define ADD_THREAD_LOCAL_BUFFER(_buffer_name, _buffer_type, _current_size, _size_change_step)                                                           \
	thread_local static std::unique_ptr<_buffer_type, malloc_free_delete> _buffer_name((_buffer_type *)malloc(sizeof(_buffer_type) * (_current_size))); \
	thread_local static size_t _buffer_name##buffer_size = (_current_size);                                                                             \
	if ((_current_size) > _buffer_name##buffer_size) {                                                                                                  \
		ZoneScopedN("Update buffer size");                                                                                                              \
		_buffer_name##buffer_size = (size_t)Math::ceil((_current_size) / (double)(_size_change_step)) * (_size_change_step);                            \
		ZoneTextF("Actual size: %llu, New buffer size: %llu", (_current_size), _buffer_name##buffer_size);                                              \
		_buffer_name = std::unique_ptr<_buffer_type, malloc_free_delete>((_buffer_type *)malloc(sizeof(_buffer_type) * _buffer_name##buffer_size));     \
	}
