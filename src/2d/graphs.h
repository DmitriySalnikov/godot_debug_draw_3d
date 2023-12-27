#pragma once

#include "common/circular_buffer.h"
#include "common/colors.h"
#include "utils/compiler.h"
#include "utils/profiler.h"

#include <mutex>
#include <vector>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DataGraphManager;

/**
 * @brief
 * Base class for drawing graphs.
 *
 * Must be created via DebugDraw2D.create_graph.
 */
class DebugDraw2DGraph : public RefCounted {
	GDCLASS(DebugDraw2DGraph, RefCounted);

public:
	/*enum GraphLinePosition : int {
		LINE_TOP = 0,
		LINE_CENTER = 1,
		LINE_BOTTOM = 2,
	};*/

	/**
	 * Available graph positions
	 */
	enum GraphPosition : int {
		POSITION_LEFT_TOP = 0,
		POSITION_RIGHT_TOP = 1,
		POSITION_LEFT_BOTTOM = 2,
		POSITION_RIGHT_BOTTOM = 3,
		POSITION_MAX
	};

	/**
	 * Which side of the parent graph this graph joins.
	 */
	enum GraphSide : int {
		SIDE_LEFT = 0,
		SIDE_TOP = 1,
		SIDE_RIGHT = 2,
		SIDE_BOTTOM = 3,
		SIDE_MAX
	};

	/**
	 * Flags for displaying text
	 */
	enum TextFlags : int64_t {
		TEXT_CURRENT = 1 << 0,
		TEXT_AVG = 1 << 1,
		TEXT_MAX = 1 << 2,
		TEXT_MIN = 1 << 3,
		TEXT_ALL = TEXT_CURRENT | TEXT_AVG | TEXT_MAX | TEXT_MIN,
	};

	/// @private
	enum GraphType {
		GRAPH_NORMAL,
		GRAPH_FPS,
	};

	/// @private
	struct graph_interpolated_values_range {
		double shrink_weight_max, shrink_weight_min;
		double upd_timer_max, upd_timer_min, max_timer_delay;
		double min, max, avg;
		uint32_t buffer_size;

		void update(const double &_min, const double &_max, const double &_avg, const double &_delta);
		void reset(uint32_t _buffer_size, double _upd_timer_delay = 2.0);
	};

private:
	/// Is Graph enabled
	bool enabled = true;
	/// Is Graph line is upside down
	bool upside_down = true;
	/// Draw Graph title
	bool show_title = false;
	/*
	/// Draw a graph line aligned vertically in the center
	GraphLinePosition line_position = GraphLinePosition::LINE_CENTER;
	*/
	/// Sets the text visibility *TextFlags*
	BitField<TextFlags> show_text_flags = TextFlags::TEXT_ALL;
	/// The size of the graph.
	Vector2i size = Vector2i(256, 64);
	/// The size of the buffer where the values are stored.
	int buffer_size = 256;
	/// Offset from the corner selected in position
	Vector2i offset = Vector2i(8, 8);
	/// FPS Graph position *GraphPosition*
	GraphPosition corner = GraphPosition::POSITION_RIGHT_TOP;
	/// Graph line width
	real_t line_width = 1.0f;
	/// Graph line color
	Color line_color = Colors::orange_red;
	/// Background color
	Color background_color = Colors::gray_graph_bg;
	/// Border color
	Color border_color = Colors::black;

	/// Text suffix
	String text_suffix = "";
	/// Custom Font
	Ref<Font> custom_font = Ref<Font>();
	/// Title Size
	int title_size = 14;
	/// Text Size
	int text_size = 12;
	/// Color of the info title
	Color title_color = Colors::white_smoke;
	/// Color of the info text
	Color text_color = Colors::white_smoke;
	/// Float precision
	int text_precision = 2;
	/// Parent graph to stack with
	StringName parent_graph;
	/// Parent graph side to stack with
	GraphSide parent_graph_side = GraphSide::SIDE_BOTTOM;
	/// Callable for automatic updating of graph data
	Callable data_getter;

private:
	mutable graph_interpolated_values_range graph_range = {};
	StringName title;

protected:
	/// @private
	DataGraphManager *owner = nullptr;
	/// @private
	mutable ProfiledMutex(std::recursive_mutex, datalock, "Graphs draw lock");
	/// @private
	std::unique_ptr<CircularBuffer<double> > buffer_data;

	/// @private
	static void _bind_methods();

	/// @private
	virtual void _update_received(double _value);
	/// @private
	void _init(DataGraphManager *_owner, StringName _title);

public:
	DebugDraw2DGraph(){};
	/// @private
	DebugDraw2DGraph(DataGraphManager *_owner, StringName _title);
	~DebugDraw2DGraph();

	/// @private
	virtual GraphType get_type() { return GraphType::GRAPH_NORMAL; };

	/**
	 * Get the graph title
	 */
	StringName get_title() const;

	/**
	 * Set whether the graph is active
	 */
	void set_enabled(const bool _state);
	bool is_enabled() const;
	/**
	 * Set the direction to which the graph line is moving
	 */
	void set_upside_down(const bool _state);
	bool is_upside_down() const;
	/**
	 * Set whether to display the graph title
	 */
	void set_show_title(const bool _state);
	bool is_show_title() const;
	/*void set_line_position(const GraphLinePosition _position);
	GraphLinePosition get_line_position() const;*/
	/**
	 * Set text display flags
	 */
	void set_show_text_flags(const BitField<TextFlags> _flags);
	BitField<TextFlags> get_show_text_flags() const;
	/**
	 * Set the graph size
	 */
	void set_size(const Vector2i &_size);
	Vector2i get_size() const;
	/**
	 * Set the buffer size
	 */
	void set_buffer_size(const int _buf_size);
	int get_buffer_size() const;
	/**
	 * Set the offset from the corner
	 */
	void set_offset(const Vector2i &_offset);
	Vector2i get_offset() const;
	/**
	 * Set the corner where the graph will be drawn.
	 * It only works if the parent graph is not set via DebugDraw2DGraph.set_parent_graph.
	 */
	void set_corner(const GraphPosition _position);
	GraphPosition get_corner() const;
	/**
	 * Set the line width
	 */
	void set_line_width(const real_t _width);
	real_t get_line_width() const;
	/**
	 * Set the line color
	 */
	void set_line_color(const Color &_new_color);
	Color get_line_color() const;
	/**
	 * Set the background color
	 */
	void set_background_color(const Color &_new_color);
	Color get_background_color() const;
	/**
	 * Set the border color
	 */
	void set_border_color(const Color &_new_color);
	Color get_border_color() const;
	/**
	 * Set the text to be displayed after the numeric value
	 */
	void set_text_suffix(const String &_suffix);
	String get_text_suffix() const;
	/**
	 * Set the text font
	 */
	void set_custom_font(const Ref<Font> _custom_font);
	Ref<Font> get_custom_font() const;
	/**
	 * Set the title size
	 */
	void set_title_size(const int _size);
	int get_title_size() const;
	/**
	 * Set the text size
	 */
	void set_text_size(const int _size);
	int get_text_size() const;
	/**
	 * Set the title color
	 */
	void set_title_color(const Color &_new_color);
	Color get_title_color() const;
	/**
	 * Set the text color
	 */
	void set_text_color(const Color &_new_color);
	Color get_text_color() const;
	/**
	 * Set the number of decimal places
	 */
	void set_text_precision(const int _precision);
	int get_text_precision() const;
	/**
	 * Set the parent graph relative to which this graph will be drawn
	 */
	void set_parent_graph(const StringName &_graph);
	StringName get_parent_graph() const;
	/**
	 * Set the side of the parent graph relative to which this graph will be drawn.
	 * It only works if the parent graph is set via DebugDraw2DGraph.set_parent_graph
	 */
	void set_parent_graph_side(const GraphSide _side);
	GraphSide get_parent_graph_side() const;
	/**
	 * Set a Callable that will be called every frame to get a new value
	 */
	virtual void set_data_getter(const Callable &_callable);
	Callable get_data_getter() const;

	/**
	 * Set DebugDraw2DGraph.set_parent_graph and DebugDraw2DGraph.set_parent_graph_side at the same time
	 */
	void set_parent(const StringName &_name, const GraphSide _side = GraphSide::SIDE_BOTTOM);

public:
	/// @private
	void update(double value);

	/// @private
	struct graph_rects {
		Rect2i full;
		Rect2i base;
	};

	/// @private
	Vector2i _get_graph_position(const bool &_is_root, const DebugDraw2DGraph::GraphPosition &_corner, const DebugDraw2DGraph::graph_rects &_rects) const;
	/// @private
	graph_rects draw(CanvasItem *_ci, const Ref<Font> &_font, const graph_rects &_prev_rects, const GraphPosition &_corner, const bool &_is_root, const double &_delta) const;
};

VARIANT_ENUM_CAST(DebugDraw2DGraph::GraphPosition);
// VARIANT_ENUM_CAST(DebugDraw2DGraph::GraphLinePosition);
VARIANT_ENUM_CAST(DebugDraw2DGraph::GraphSide);
VARIANT_BITFIELD_CAST(DebugDraw2DGraph::TextFlags);

/**
 * @brief
 * This version of the graphs is automatically updated and displays FPS or Frametime.
 *
 * Must be created via DebugDraw2D.create_fps_graph.
 */
class DebugDraw2DFPSGraph : public DebugDraw2DGraph {
	GDCLASS(DebugDraw2DFPSGraph, DebugDraw2DGraph);

private:
	bool frametime_mode = true;
	bool is_ms = false;

protected:
	/// @private
	static void _bind_methods();

#ifndef DISABLE_DEBUG_RENDERING
	/// @private
	virtual void _update_received(double _value) override;

public:
	DebugDraw2DFPSGraph(){};
	/// @private
	DebugDraw2DFPSGraph(DataGraphManager *_owner, StringName _title);

	/// @private
	virtual GraphType get_type() override { return GraphType::GRAPH_FPS; };
	/**
	 * Not available for FPS Graph
	 */
	virtual void set_data_getter(const Callable &_callable) override;
#endif

	/**
	 * Switching between Frametime and FPS display.
	 */
	void set_frame_time_mode(const bool _state);
	bool is_frame_time_mode() const;
};

#ifndef DISABLE_DEBUG_RENDERING

/// @private
class DataGraphManager {
	std::vector<Ref<DebugDraw2DGraph> > graphs;
	mutable ProfiledMutex(std::recursive_mutex, datalock, "Graphs manager lock");
	class DebugDraw2D *owner = nullptr;

public:
	DataGraphManager(class DebugDraw2D *root);
	~DataGraphManager();

	void draw(CanvasItem *_ci, Ref<Font> _font, Vector2 _vp_size, double _delta) const;
	Ref<DebugDraw2DGraph> create_graph(const StringName &_title);
	Ref<DebugDraw2DGraph> create_fps_graph(const StringName &_title);
	void auto_update_graphs(double delta);
	void graph_update_data(const StringName &_title, const double &_data);
	void remove_graph(const StringName &_title);
	void clear_graphs();
	Ref<DebugDraw2DGraph> get_graph(const StringName &_title) const;
	PackedStringArray get_graph_names() const;
	size_t get_graphs_enabled() const;
	size_t get_graphs_total() const;
};

#endif