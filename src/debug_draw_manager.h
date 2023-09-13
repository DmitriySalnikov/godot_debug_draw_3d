#pragma once

#include "utils/compiler.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/camera3d.hpp> // TODO: need to be removed with vararg functions in release build.
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
GODOT_WARNING_RESTORE()

#include <memory>

using namespace godot;

class DebugDrawManager : public CanvasLayer {
	GDCLASS(DebugDrawManager, CanvasLayer)
protected:
	static DebugDrawManager *singleton;

	static void _bind_methods();

	double log_flush_time = 0;
	bool is_closing = false;
	bool is_current_scene_is_null = true;

	class DebugDraw3D *debug_draw_3d_singleton = nullptr;
	class DebugDraw2D *debug_draw_2d_singleton = nullptr;

	Node *_get_current_scene();
	void _connect_scene_changed();
	void _on_scene_changed(bool _is_scene_null);

	void _integrate_into_engine();

public:
	DebugDrawManager();
	~DebugDrawManager();

	static DebugDrawManager *get_singleton() {
		return singleton;
	};

	void init();
	virtual void _process(double delta) override;
	virtual void _exit_tree() override;

#ifdef TOOLS_ENABLED
	void try_to_update_cs_bindings();
#endif
};