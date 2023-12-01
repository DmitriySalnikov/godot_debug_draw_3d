#pragma once

#include "common/colors.h"
#include "utils/compiler.h"

#include <memory>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/font.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DataGraphManager;
class DebugDrawManager;
class DebugDrawConfig2D;
class DebugDrawGraph;
class DebugDrawStats2D;
class GroupedText;

class DebugDraw2D : public Object {
	GDCLASS(DebugDraw2D, Object)

	friend DebugDrawManager;
	static DebugDraw2D *singleton;

private:
	DebugDrawManager *root_node = nullptr;

	// 2d
	const static char *s_marked_dirty;

	bool _is_drawing_frame = true;
	bool _canvas_need_update = true;
	Ref<Font> _font;

	Control *default_canvas = nullptr;

#ifndef DISABLE_DEBUG_RENDERING
	// Text
	std::unique_ptr<GroupedText> grouped_text;

	// Graphs
	std::unique_ptr<DataGraphManager> data_graphs;
#endif

#ifndef DISABLE_DEBUG_RENDERING
	void _finish_frame_and_update();
#endif

	void _on_canvas_item_draw(Control *ci);
	inline bool _is_enabled_override() const;

	void process(double delta);

#pragma region Exposed Parameter Values

	// GENERAL

	/// Enable or disable all debug draw
	bool debug_enabled = true;
	/// Custom 'CanvasItem' to draw on it. Set to 'null' to disable.
	Control *custom_canvas = nullptr;

	Ref<DebugDrawConfig2D> config;

#pragma endregion // Exposed Parameter Values

protected:
	static void _bind_methods();

public:
	DebugDraw2D();
	~DebugDraw2D();

	void init(DebugDrawManager *root);

	static DebugDraw2D *get_singleton() {
		return singleton;
	};
	void mark_canvas_dirty();
	bool is_drawing_frame() const;

	Node *get_root_node();

#pragma region Exposed Parameters
	void set_empty_color(const Color &_col);
	Color get_empty_color() const;

	void set_debug_enabled(const bool &_state);
	bool is_debug_enabled() const;

	void set_config(Ref<DebugDrawConfig2D> _cfg);
	Ref<DebugDrawConfig2D> get_config() const;

	void set_custom_canvas(Control *_canvas);
	Control *get_custom_canvas() const;
#pragma endregion // Exposed Parametes

#pragma region Exposed Draw Functions

	/// Returns a dictionary with rendering statistics.
	/// Some data can be delayed by 1 frame.
	Ref<DebugDrawStats2D> get_render_stats();

	/// Clear all 2D objects
	void clear_all();

#pragma region Text

	/// Begin text group
	/// group_title: Group title and ID
	/// group_priority: Group priority
	/// group_color: Group color
	/// show_title: Whether to show the title
	void begin_text_group(String group_title, int group_priority = 0, Color group_color = Colors::empty_color, bool show_title = true, int title_size = 14, int text_size = 12);

	/// End text group. Should be called after 'begin_text_group' if you don't need more than one group.
	/// If you need to create 2+ groups just call again 'begin_text_group' and this function in the end.
	void end_text_group();

	/// Add or update text in overlay
	/// key: The name of the field, if there is a 'value', otherwise the whole string will be equal to the 'key'
	/// value: Value of field
	/// priority: Priority of this line. Lower value is higher position.
	/// color_of_value: Value color
	/// duration: Expiration time
	void set_text(String key, Variant value = Variant(), int priority = 0, Color color_of_value = Colors::empty_color, real_t duration = -1);

#pragma endregion // Text
#pragma region Graphs

	/// Create new graph with custom data.
	/// title: Title of the graph
	Ref<DebugDrawGraph> create_graph(const StringName &title);

	/// Create new graph with custom data.
	/// title: Title of the graph
	Ref<DebugDrawGraph> create_fps_graph(const StringName &title);

	/// Update custom graph data
	/// title: Title of the graph
	/// data: New data
	void graph_update_data(const StringName &title, real_t data);

	/// Remove graph
	/// title: Title of the graph
	void remove_graph(const StringName &title);

	/// Remove all graphs
	void clear_graphs();

	/// Get config for graph.
	/// title: Title of the graph
	Ref<DebugDrawGraph> get_graph(const StringName &title);

	/// Get all graph names
	PackedStringArray get_graph_names();

#pragma endregion // Graphs
#pragma endregion // Exposed Draw Functions
};
