#pragma once

#include "utils_compiler.h"

MSVC_WARNING_DISABLE(4244)
#include <godot_cpp/variant/color.hpp>
MSVC_WARNING_RESTORE(4244)

class Colors {
public:
	const static godot::Color empty_color;
	const static godot::Color axis_x;
	const static godot::Color axis_y;
	const static godot::Color axis_z;
	const static godot::Color black;
	const static godot::Color chartreuse;
	const static godot::Color crimson;
	const static godot::Color dark_orange;
	const static godot::Color dark_salmon;
	const static godot::Color debug_bounds;
	const static godot::Color dodgerblue;
	const static godot::Color forest_green;
	const static godot::Color gray_bg;
	const static godot::Color gray_graph_bg;
	const static godot::Color green;
	const static godot::Color light_green;
	const static godot::Color orange_red;
	const static godot::Color orange;
	const static godot::Color red;
	const static godot::Color white_smoke;
	const static godot::Color white;
	const static godot::Color yellow;
};
