#pragma once

#include "circular_buffer.h"
#include "math_utils.h"
#include "utils.h"

#include <CanvasItem.hpp>
#include <Font.hpp>
#include <Godot.hpp>
#include <Reference.hpp>

#include <map>

using namespace godot;

class GraphParameters : public Reference {
	GODOT_CLASS(GraphParameters, Reference);

	/// Is FPSGraph enabled
	bool FPSGraphEnabled = true;
	/// Draw Graph title
	bool FPSGraphShowTitle = true;
	/// Switch between frame time and FPS modes
	bool FPSGraphFrameTimeMode = true;
	/// Draw a graph line aligned vertically in the center
	bool FPSGraphCenteredGraphLine = true;
	/// Sets the text visibility *FPSGraphTextFlags*
	int FPSGraphShowTextFlags = 15 /*FPSGraphTextFlags::All*/;
	/// Size of the FPS Graph. The width is equal to the number of stored frames.
	Vector2 FPSGraphSize = Vector2(256, 64);
	/// Size of the FPS Graph. The width is equal to the number of stored frames.
	int FPSBufferSize = 256;
	/// Offset from the corner selected in <see cref="FPSGraphPosition"/>
	Vector2 FPSGraphOffset = Vector2(8, 8);
	/// FPS Graph position *BlockPosition*
	int FPSGraphPosition = 1 /*BlockPosition::RightTop*/;
	/// Graph line color
	Color FPSGraphLineColor = Color(1, 0.27f, 0, 1) /* OrangeRed */;
	/// Color of the info text
	Color FPSGraphTextColor = Color(0.96f, 0.96f, 0.96f, 1) /* WhiteSmoke */;
	/// Background color
	Color FPSGraphBackgroundColor = Color(0.2f, 0.2f, 0.2f, 0.6f);
	/// Border color
	Color FPSGraphBorderColor = Color(0, 0, 0, 1) /* Black */;
	/// Border color
	String FPSGraphTextSuffix = "";
	/// Custom Font
	Ref<Font> FPSGraphCustomFont = nullptr;

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
	void set_show_text_flags(int /*FPSGraphTextFlags*/ flags);
	int /*FPSGraphTextFlags*/ get_show_text_flags();
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
	void set_custom_font(Ref<Font> custom_font);
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
	CircularBuffer<real_t> data;
	std::mutex datalock;
	Type type;

	virtual void _update_added(real_t value);

public:
	DataGraph(Ref<GraphParameters> _owner);

	Type get_type();
	Ref<GraphParameters> get_config();
	void update(real_t value);

	// Must return edge Y to render next graph with this offset
	real_t draw(CanvasItem *ci, Ref<Font> font, Vector2 vp_size, String title, real_t y_offset);
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
	std::mutex datalock;

public:
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