#pragma once

#include "utils/compiler.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/node.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DD3DTestCppApiNode : public Node {
	GDCLASS(DD3DTestCppApiNode, Node)
protected:
	static void _bind_methods();

	enum class SelectedDrawFunction {
		draw_box,
		draw_lines_c,
		draw_line_path_c,
		draw_points_c,
		draw_points_c_square,
		draw_point_path_c,
		draw_point_path_c_square,
		draw_arrow_path_c,
	};

	bool one_million_boxes = false;
	bool many_camera_frustums = false;
	int draw_function = 0;

public:
	DD3DTestCppApiNode();
	virtual void _ready() override;
	virtual void _process(double p_delta) override;

	void set_one_million_boxes(bool val) { one_million_boxes = val; }
	bool is_one_million_boxes() { return one_million_boxes; }
	void set_many_camera_frustums(bool val) { many_camera_frustums = val; }
	bool is_many_camera_frustums() { return many_camera_frustums; }
	void set_draw_function(int val) { draw_function = val; }
	int get_draw_function() { return draw_function; }
};
