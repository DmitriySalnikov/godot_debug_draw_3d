#pragma once

#include "common/colors.h"
#include "utils/compiler.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

using namespace godot;

class DataGraphManager;
class DebugDrawManager;
class DebugDrawConfig2D;
class DebugDrawGraph;
class DebugDrawStats2D;
class GeometryContainer2D;
class GroupedText;

class DDTransform2DGuard : public RefCounted {
	GDCLASS(DDTransform2DGuard, RefCounted)

protected:
	static void _bind_methods(){};
	static std::atomic<uint64_t> create_counter;

public:
	uint64_t thread_id;
	uint64_t guard_id;

	DDTransform2DGuard();
	DDTransform2DGuard(const Transform2D &xform);
	~DDTransform2DGuard();
};

class DebugDraw2D : public Object {
	GDCLASS(DebugDraw2D, Object)

	friend DebugDrawManager;
	static DebugDraw2D *singleton;

private:
	DebugDrawManager *root_node = nullptr;

	// 2d
	typedef std::pair<uint64_t, Transform2D> transform_pair;
	std::map<uint64_t, std::vector<transform_pair> > transform_stack;
	mutable std::recursive_mutex transform_stack_mutex;

	// TODO create vector of pairs<Transform, DrawInstanceBaseType>, to combine as much as possible instances by transforms
	// or create a map for <uint64_t, Transform2D> to remap transform to some id's and then create a map<uint64_t, DrawInstanceBaseType> to store data
	// in anyway this will need a new pooling system

	bool _canvas_need_update = true;
	Ref<Font> _font;

	Control *default_canvas = nullptr;

#ifndef DISABLE_DEBUG_RENDERING
	// Text
	std::unique_ptr<GroupedText> grouped_text;

	// Graphs
	std::unique_ptr<DataGraphManager> data_graphs;

	// Objects
	std::unique_ptr<GeometryContainer2D> dgc2d;
#endif

#ifndef DISABLE_DEBUG_RENDERING
	void _finish_frame_and_update();
#endif

	void _on_canvas_marked_dirty();
	void _on_canvas_item_draw(Control *ci);
	void _draw_2d_geometry(Control *ci);
	inline bool _is_enabled_override() const;

	void process(double delta);

#pragma region Exposed Parameter Values

	// GENERAL

	/// Enable or disable all debug draw
	bool debug_enabled = true;
	/// Custom 'CanvasItem' to draw on it. Set to 'null' to disable.
	Control *custom_canvas = nullptr;

	Transform2D default_transform_2d;

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

	Node *get_root_node();

	Ref<DDTransform2DGuard> draw_set_transform_2d(const Vector2 &position, double rotation = 0.0, const Vector2 &scale = Vector2(1, 1));
	Ref<DDTransform2DGuard> draw_set_transform_matrix_2d(const Transform2D &xform);

	Transform2D get_transform_for_current_thread();
	void register_transform_guard(uint64_t guard_id, uint64_t thread_id, const Transform2D &xform);
	void unregister_transform_guard(uint64_t guard_id, uint64_t thread_id);
	void clear_transform_guards();

#pragma region Exposed Parameters
	void set_empty_color(const Color &_col);
	Color get_empty_color() const;

	void set_debug_enabled(const bool &_state);
	bool is_debug_enabled() const;

	void set_default_transform_2d(const Transform2D &_xform);
	Transform2D get_default_transform_2d() const;

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

#pragma region 2D

	// TODO: add method to preconfig duration or "duration" guard which will set duration while exists. When its removed, duration will be 0
	// or simply add "duration" to the end of all methods..
	void draw_line(const Vector2 &from, const Vector2 &to, const Color &color, double width = -1.0, bool antialiased = false);
	void draw_dashed_line(const Vector2 &from, const Vector2 &to, const Color &color, double width = -1.0, double dash = 2.0, bool aligned = true);
	void draw_polyline(const PackedVector2Array &points, const Color &color, double width = -1.0, bool antialiased = false);
	void draw_polyline_colors(const PackedVector2Array &points, const PackedColorArray &colors, double width = -1.0, bool antialiased = false);
	void draw_arc(const Vector2 &center, double radius, double start_angle, double end_angle, int32_t point_count, const Color &color, double width = -1.0, bool antialiased = false);
	void draw_multiline(const PackedVector2Array &points, const Color &color, double width = -1.0);
	void draw_multiline_colors(const PackedVector2Array &points, const PackedColorArray &colors, double width = -1.0);
	void draw_rect(const Rect2 &rect, const Color &color, bool filled = true, double width = -1.0);
	void draw_circle(const Vector2 &position, double radius, const Color &color);
	void draw_texture(const Ref<Texture2D> &texture, const Vector2 &position, const Color &modulate = Color(1, 1, 1, 1));
	void draw_texture_rect(const Ref<Texture2D> &texture, const Rect2 &rect, bool tile, const Color &modulate = Color(1, 1, 1, 1), bool transpose = false);
	void draw_texture_rect_region(const Ref<Texture2D> &texture, const Rect2 &rect, const Rect2 &src_rect, const Color &modulate = Color(1, 1, 1, 1), bool transpose = false, bool clip_uv = true);
	void draw_style_box(const Ref<StyleBox> &style_box, const Rect2 &rect);
	void draw_primitive(const PackedVector2Array &points, const PackedColorArray &colors, const PackedVector2Array &uvs, const Ref<Texture2D> &texture = nullptr);
	void draw_polygon(const PackedVector2Array &points, const PackedColorArray &colors, const PackedVector2Array &uvs = PackedVector2Array(), const Ref<Texture2D> &texture = nullptr);
	void draw_colored_polygon(const PackedVector2Array &points, const Color &color, const PackedVector2Array &uvs = PackedVector2Array(), const Ref<Texture2D> &texture = nullptr);
	void draw_string(const Ref<Font> &font, const Vector2 &pos, const String &text, HorizontalAlignment alignment = (HorizontalAlignment)0, double width = -1, int32_t font_size = 16, const Color &modulate = Color(1, 1, 1, 1), BitField<TextServer::JustificationFlag> justification_flags = (BitField<TextServer::JustificationFlag>)3, TextServer::Direction direction = (TextServer::Direction)0, TextServer::Orientation orientation = (TextServer::Orientation)0) const;
	void draw_multiline_string(const Ref<Font> &font, const Vector2 &pos, const String &text, HorizontalAlignment alignment = (HorizontalAlignment)0, double width = -1, int32_t font_size = 16, int32_t max_lines = -1, const Color &modulate = Color(1, 1, 1, 1), BitField<TextServer::LineBreakFlag> brk_flags = (BitField<TextServer::LineBreakFlag>)3, BitField<TextServer::JustificationFlag> justification_flags = (BitField<TextServer::JustificationFlag>)3, TextServer::Direction direction = (TextServer::Direction)0, TextServer::Orientation orientation = (TextServer::Orientation)0) const;
	void draw_string_outline(const Ref<Font> &font, const Vector2 &pos, const String &text, HorizontalAlignment alignment = (HorizontalAlignment)0, double width = -1, int32_t font_size = 16, int32_t size = 1, const Color &modulate = Color(1, 1, 1, 1), BitField<TextServer::JustificationFlag> justification_flags = (BitField<TextServer::JustificationFlag>)3, TextServer::Direction direction = (TextServer::Direction)0, TextServer::Orientation orientation = (TextServer::Orientation)0) const;
	void draw_multiline_string_outline(const Ref<Font> &font, const Vector2 &pos, const String &text, HorizontalAlignment alignment = (HorizontalAlignment)0, double width = -1, int32_t font_size = 16, int32_t max_lines = -1, int32_t size = 1, const Color &modulate = Color(1, 1, 1, 1), BitField<TextServer::LineBreakFlag> brk_flags = (BitField<TextServer::LineBreakFlag>)3, BitField<TextServer::JustificationFlag> justification_flags = (BitField<TextServer::JustificationFlag>)3, TextServer::Direction direction = (TextServer::Direction)0, TextServer::Orientation orientation = (TextServer::Orientation)0) const;
	void draw_mesh(const Ref<Mesh> &mesh, const Ref<Texture2D> &texture, const Transform2D &transform = Transform2D(), const Color &modulate = Color(1, 1, 1, 1));
	void draw_multimesh(const Ref<MultiMesh> &multimesh, const Ref<Texture2D> &texture);

#pragma endregion // 2D

#pragma region Overlay
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
#pragma endregion // Overlay
#pragma endregion // Exposed Draw Functions
};
