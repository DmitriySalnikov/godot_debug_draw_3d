#pragma once

#include <Godot.hpp>

#define TEXT(s) #s

#pragma region PRINTING

#define PRINT(text) Godot::print(text)
#define PRINT_ERROR(text) Godot::print_error(text, __FUNCTION__, get_file_name_in_repository(__FILE__), __LINE__)
#define PRINT_WARNING(text) Godot::print_warning(text, __FUNCTION__, get_file_name_in_repository(__FILE__), __LINE__)

namespace godot {
static String get_file_name_in_repository(String name) {
	if (name != "") {
		int idx = name.find("debug_draw_3d");
		if (idx != -1)
			name = name.substr(name.find("debug_draw_3d"), name.length());
	}
	return name;
}
} // namespace godot

#pragma endregion PRINTING

#define IS_EDITOR_HINT() Engine::get_singleton()->is_editor_hint()
#define DEBUG_DRAW() DebugDraw3D::get_singleton()
#define TIME_NOW() std::chrono::system_clock::now()
#define TIME_TO_MS(time) std::chrono::duration_cast<std::chrono::milliseconds>(time)

#define C_ARR_SIZE(arr) (sizeof(arr) / sizeof(*arr))
#define LOCK_METHOD(_mutex) std::lock_guard<std::mutex> __guard(_mutex)

class Utils {
public:
	const static godot::Color empty_color;
};