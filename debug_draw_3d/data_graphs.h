#pragma once

#include "circular_buffer.h"

#include <CanvasItem.hpp>
#include <Font.hpp>
#include <Godot.hpp>
#include <Reference.hpp>

#include <map>

using namespace godot;

class GraphParameters : public Reference {
	GODOT_CLASS(GraphParameters, Reference);

	/// Is FPSGraph enabled
	bool enabled;
	/// Draw Graph title
	bool show_title;
	/// Switch between frame time and FPS modes
	bool frametime_mode;
	/// Draw a graph line aligned vertically in the center
	bool centered_graph_line = true;
	/// Sets the text visibility *GraphTextFlags*
	int show_text_flags;
	/// The size of the graph. The width is equal to the number of saved frames.
	Vector2 size;
	/// The size of the buffer where the values are stored.
	int buffer_size;
	/// Offset from the corner selected in position
	Vector2 offset;
	/// FPS Graph position *BlockPosition*
	int position;
	/// Graph line color
	Color line_color;
	/// Color of the info text
	Color text_color;
	/// Background color
	Color background_color;
	/// Border color
	Color border_color;
	/// Border color
	String text_suffix;
	/// Custom Font
	Ref<Font> custom_font;

public:
	static void _register_methods();
	void _init();

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
	void update(String title, real_t data);
	void remove_graph(String title);
	void clear_graphs();
	Ref<GraphParameters> get_graph_config(String title);
	PoolStringArray get_graph_names();
};