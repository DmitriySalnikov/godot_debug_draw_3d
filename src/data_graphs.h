#pragma once

#include "circular_buffer.h"
#include "colors.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

#include <map>

using namespace godot;

class GraphParameters : public RefCounted {
	GDCLASS(GraphParameters, RefCounted);

	enum GraphPosition : int {
		POSITION_LEFT_TOP = 0,
		POSITION_RIGHT_TOP = 1,
		POSITION_LEFT_BOTTOM = 2,
		POSITION_RIGHT_BOTTOM = 3,
	};

	enum TextFlags : int64_t {
		TEXT_CURRENT = 1 << 0,
		TEXT_AVG = 1 << 1,
		TEXT_MAX = 1 << 2,
		TEXT_MIN = 1 << 3,
		TEXT_ALL = TEXT_CURRENT | TEXT_AVG | TEXT_MAX | TEXT_MIN,
	};

	/// Is Graph enabled
	bool enabled = true;
	/// Draw Graph title
	bool show_title = false;
	/// Switch between frame time and FPS modes
	/// Only for FPS Graphs
	bool frametime_mode = true;
	/// Draw a graph line aligned vertically in the center
	bool centered_graph_line = true;
	/// Sets the text visibility *TextFlags*
	BitField<TextFlags> show_text_flags = TextFlags::TEXT_ALL;
	/// The size of the graph.
	Vector2 size = Vector2(256, 64);
	/// The size of the buffer where the values are stored.
	int buffer_size = 256;
	/// Offset from the corner selected in position
	Vector2 offset = Vector2(0, 8);
	/// FPS Graph position *GraphPosition*
	GraphPosition position = GraphPosition::POSITION_RIGHT_TOP;
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
	bool is_enabled() const;
	void set_show_title(bool state);
	bool is_show_title() const;
	void set_frame_time_mode(bool state);
	bool is_frame_time_mode() const;
	void set_centered_graph_line(bool state);
	bool is_centered_graph_line() const;
	void set_show_text_flags(BitField<TextFlags> flags);
	BitField<TextFlags> get_show_text_flags() const;
	void set_size(Vector2 size);
	Vector2 get_size() const;
	void set_buffer_size(int buf_size);
	int get_buffer_size() const;
	void set_offset(Vector2 offset);
	Vector2 get_offset() const;
	void set_position(GraphPosition position);
	GraphPosition get_position() const;
	void set_line_color(Color new_color);
	Color get_line_color() const;
	void set_text_color(Color new_color);
	Color get_text_color() const;
	void set_background_color(Color new_color);
	Color get_background_color() const;
	void set_border_color(Color new_color);
	Color get_border_color() const;
	void set_text_suffix(String suffix);
	String get_text_suffix() const;
	void set_custom_font(Ref<Font> _custom_font);
	Ref<Font> get_custom_font() const;
};

VARIANT_ENUM_CAST(GraphParameters::GraphPosition);
VARIANT_BITFIELD_CAST(GraphParameters::TextFlags);

class DataGraph {
public:
	enum Type {
		Custom,
		FPS,
	};

protected:
	Ref<GraphParameters> config;
	std::shared_ptr<CircularBuffer<double> > data;
	mutable std::recursive_mutex datalock;
	Type type;

	virtual void _update_added(double value);

public:
	DataGraph(Ref<GraphParameters> _owner);

	Type get_type() const;
	Ref<GraphParameters> get_config() const;
	void update(double value);

	// Must return edge Y to render next graph with this offset
	Vector2 draw(CanvasItem *ci, Ref<Font> font, Vector2 vp_size, String title, Vector2 base_offset) const;
};

class FPSGraph : public DataGraph {
protected:
	virtual void _update_added(double value) override;
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
	std::map<String, std::unique_ptr<DataGraph>, _data_graph_manager_graphs_comp> graphs;
	mutable std::recursive_mutex datalock;
	class DebugDraw *owner;

public:
	DataGraphManager(class DebugDraw *root);
	~DataGraphManager();

	Ref<GraphParameters> create_graph(String title);
	Ref<GraphParameters> create_fps_graph(String title);
	void _update_fps(double delta);
	void draw(CanvasItem *ci, Ref<Font> font, Vector2 vp_size) const;
	void graph_update_data(String title, double data);
	void remove_graph(String title);
	void clear_graphs();
	Ref<GraphParameters> get_graph_config(const String &title) const;
	PackedStringArray get_graph_names() const;
};
