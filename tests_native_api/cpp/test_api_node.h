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

	bool one_million_boxes = false;

public:
	DD3DTestCppApiNode();
	virtual void _ready() override;
	virtual void _process(double p_delta) override;

	void set_one_million_boxes(bool val) { one_million_boxes = val; };
	bool is_one_million_boxes() { return one_million_boxes; };
};
