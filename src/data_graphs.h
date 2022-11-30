#pragma once

#include "circular_buffer.h"
#include "colors.h"
#include "enums.h"

#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/builtin_types.hpp>

#include <map>

using namespace godot;

class GraphParameters : public RefCounted {
	GDCLASS(GraphParameters, RefCounted);

	/// Is Graph enabled
	bool enabled = true;
	/// Draw Graph title
	bool show_title = false;
	/// Switch between frame time and FPS modes
	/// Only for FPS Graphs
	bool frametime_mode = true;
	/// Draw a graph line aligned vertically in the center
	bool centered_graph_line = true;
	/// Sets the text visibility *GraphTextFlags*
	int show_text_flags = GraphTextFlags::All;
	/// The size of the graph.
	Vector2 size = Vector2(256, 64);
	/// The size of the buffer where the values are stored.
	int buffer_size = 256;
	/// Offset from the corner selected in position
	Vector2 offset = Vector2(0, 8);
	/// FPS Graph position *BlockPosition*
	int position = BlockPosition::RightTop;
	/// Graph line color
	Color line_color = Colors::orange_red;
	/// Color of the info text
	Color text_color = Colors::white_smoke;
	/// Background color
	Color background_color = Colors::gray_graph_bg;
	/// Border color
	Color border_color = Colors::black;
	/// Border color
	String text_suffix = "";
	/// Custom Font
	Ref<Font> custom_font = Ref<Font>();

protected:
	static void _bind_methods();

public:
	GraphParameters();

	void set_enabled(bool state);
	bool is_enabled();
	void set_show_title(bool state);
	bool is_show_title();
	void set_frame_time_mode(bool state);
	bool is_frame_time_mode();
	void set_centered_graph_line(bool state);
	bool is_centered_graph_line();
	void set_show_text_flags(int /*GraphTextFlags*/ flags);
	int /*GraphTextFlags*/ get_show_text_flags();
	void set_size(Vector2 size);
	Vector2 get_size();
	void set_buffer_size(int buf_size);
	int get_buffer_size();
	void set_offset(Vector2 offset);
	Vector2 get_offset();
	void set_position(int /*BlockPosition*/ position);
	int /*BlockPosition*/ get_position();
	void set_line_color(Color new_color);
	Color get_line_color();
	void set_text_color(Color new_color);
	Color get_text_color();
	void set_background_color(Color new_color);
	Color get_background_color();
	void set_border_color(Color new_color);
	Color get_border_color();
	void set_text_suffix(String suffix);
	String get_text_suffix();
	void set_custom_font(Ref<Font> _custom_font);
	Ref<Font> get_custom_font();
};

class DataGraph {
public:
	enum Type {
		Custom,
		FPS,
	};

protected:
	Ref<GraphParameters> config;
	std::shared_ptr<CircularBuffer<real_t> > data;
	std::recursive_mutex datalock;
	Type type;

	virtual void _update_added(real_t value);

public:
	DataGraph(Ref<GraphParameters> _owner);

	Type get_type();
	Ref<GraphParameters> get_config();
	void update(real_t value);

	// Must return edge Y to render next graph with this offset
	Vector2 draw(CanvasItem *ci, Ref<Font> font, Vector2 vp_size, String title, Vector2 base_offset);
};

class FPSGraph : public DataGraph {
protected:
	virtual void _update_added(real_t value) override;
	bool is_ms = false;

public:
	FPSGraph(Ref<GraphParameters> _owner) :
			DataGraph{ _owner } {
		type = DataGraph::Type::FPS;
	};
};

class DataGraphManager {
	struct _data_graph_manager_graphs_comp {
		bool operator()(const String &a, const String &b) const {
			return a.naturalnocasecmp_to(b) < 0;
		}
	};
	std::map<String, std::shared_ptr<DataGraph>, _data_graph_manager_graphs_comp> graphs;
	std::recursive_mutex datalock;
	class DebugDraw3D *owner;

public:
	DataGraphManager(class DebugDraw3D *root);
	~DataGraphManager();

	Ref<GraphParameters> create_graph(String title);
	Ref<GraphParameters> create_fps_graph(String title);
	void _update_fps(real_t delta);
	void draw(CanvasItem *ci, Ref<Font> font, Vector2 vp_size);
	void graph_update_data(String title, real_t data);
	void remove_graph(String title);
	void clear_graphs();
	Ref<GraphParameters> get_graph_config(String title);
	PackedStringArray get_graph_names();
};
