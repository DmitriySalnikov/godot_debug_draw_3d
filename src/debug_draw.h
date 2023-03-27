#pragma once

#include "colors.h"
#include "geometry_generators.h"

MSVC_WARNING_DISABLE(4244)
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
MSVC_WARNING_RESTORE(4244)

#include <memory>

using namespace godot;

class DataGraphManager;
class DebugDrawSceneManager;
class DebugDrawConfig2D;
class DebugDrawConfig3D;
class DebugDrawGraph;
class DebugDrawStats;
class DebugGeometryContainer;
class GroupedText;

class DebugDraw : public Object {
	GDCLASS(DebugDraw, Object)

	friend DebugDrawSceneManager;
	static DebugDraw *singleton;

private:
	std::vector<SubViewport *> custom_editor_viewports;
	DebugDrawSceneManager *root_node = nullptr;

	// Logs
	double log_flush_time = 0;

	// 2d
	CanvasLayer *_canvas_layer = nullptr;
	bool _canvas_need_update = true;
	Ref<Font> _font;

	Control *default_canvas = nullptr;

	// Text
	std::unique_ptr<GroupedText> grouped_text;

	// Graphs
	std::unique_ptr<DataGraphManager> data_graphs;

	// Meshes
	std::unique_ptr<DebugGeometryContainer> dgc;

	bool is_ready = false;
	bool is_closing = false;
	bool is_current_scene_is_null = true;

	void _on_canvas_marked_dirty();
	void _on_canvas_item_draw(Control *ci);
	void _set_base_world_node(Node *world_base);

	Node *_get_current_scene();
	void _scene_tree_found();
	void _connect_scene_changed();
	void _on_scene_changed(bool _is_scene_null);

	void enter_tree();
	void exit_tree();
	void ready();
	void process(double delta);

#pragma region Exposed Parameter Values

	// GENERAL

	/// Enable or disable all debug draw
	bool debug_enabled = true;
	/// Custom 'Viewport' to use for frustum culling.
	Viewport *custom_viewport = nullptr;
	/// Custom 'CanvasItem' to draw on it. Set to 'null' to disable.
	Control *custom_canvas = nullptr;

	Ref<DebugDrawConfig2D> config_2d;
	Ref<DebugDrawConfig3D> config_3d;

#pragma endregion // Exposed Parameter Values

protected:
	static void _bind_methods();

public:
	DebugDraw();
	~DebugDraw();

	static DebugDraw *get_singleton() {
		return singleton;
	};
	void mark_canvas_dirty();

	Node *get_root_node();
	void set_custom_editor_viewport(std::vector<SubViewport *> _viewports);
	std::vector<SubViewport *> get_custom_editor_viewports();

#pragma region Exposed Parameters
	void set_empty_color(const Color &_col);
	Color get_empty_color() const;

	void set_debug_enabled(const bool &_state);
	bool is_debug_enabled() const;

	void set_config_2d(Ref<DebugDrawConfig2D> _cfg);
	Ref<DebugDrawConfig2D> get_config_2d() const;

	void set_config_3d(Ref<DebugDrawConfig3D> _cfg);
	Ref<DebugDrawConfig3D> get_config_3d() const;

	void set_custom_viewport(Viewport *_viewport);
	Viewport *get_custom_viewport() const;

	void set_custom_canvas(Control *_canvas);
	Control *get_custom_canvas() const;
#pragma endregion // Exposed Parametes

#pragma region Exposed Draw Functions

	/// Returns a dictionary with rendering statistics.
	/// Some data can be delayed by 1 frame.
	Ref<DebugDrawStats> get_render_stats();

	/// Clear all 3D objects
	void clear_3d_objects();

	/// Clear all 2D objects
	void clear_2d_objects();

	/// Clear all debug objects
	void clear_all();

#pragma region 3D
#pragma region Spheres

	/// Draw sphere
	/// position: Position of the sphere center
	/// radius: Sphere radius
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	void draw_sphere(const Vector3 &position, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw sphere
	/// transform: Transform3D of the Sphere
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	void draw_sphere_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw sphere with higher lines count
	/// position: Position of the sphere center
	/// radius: Sphere radius
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	void draw_sphere_hd(const Vector3 &position, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw sphere with higher lines count
	/// transform: Transform3D of the sphere
	/// color: Sphere color
	/// duration: Duration of existence in seconds
	void draw_sphere_hd_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);

#pragma endregion // Spheres

#pragma region Cylinders

	/// Draw vertical cylinder
	/// transform: Transform3D of the Cylinder
	/// color: Cylinder color
	/// duration: Duration of existence in seconds
	void draw_cylinder(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);

#pragma endregion // Cylinders

#pragma region Boxes

	/// Draw box
	/// position: Position of the Box
	/// size: Size of the Box
	/// color: Box color
	/// is_box_centered: Use 'position' as the center of the box, not as the bottom corner
	/// duration: Duration of existence in seconds
	void draw_box(const Vector3 &position, const Vector3 &size, const Color &color = Colors::empty_color, const bool &is_box_centered = false, const real_t &duration = 0);

	/// Draw box
	/// transform: Transform3D of the Box
	/// color: Box color
	/// is_box_centered: Use 'transform' as the center of the box, not as the bottom corner
	/// duration: Duration of existence in seconds
	void draw_box_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const bool &is_box_centered = true, const real_t &duration = 0);

	/// Draw AABB
	/// aabb: AABB
	/// color: Box color
	/// duration: Duration of existence in seconds
	void draw_aabb(const AABB &aabb, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw AABB from 'a' to 'b'
	/// a: Firts corner
	/// b: Second corner
	/// color: Box color
	/// duration: Duration of existence in seconds
	void draw_aabb_ab(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &duration = 0);

#pragma endregion // Boxes

#pragma region Lines

	/// Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
	/// start: Start point
	/// end: End point
	/// hit: Hit point
	/// is_hit: Is hit
	/// hit_size: Size of the hit point
	/// hit_color: Color of the hit point and line before hit
	/// after_hit_color: Color of line after hit position
	/// duration: Duration of existence in seconds
	void draw_line_hit(const Vector3 &start, const Vector3 &end, const Vector3 &hit, const bool &is_hit, const real_t &hit_size = 0.25f, const Color &hit_color = Colors::empty_color, const Color &after_hit_color = Colors::empty_color, const real_t &duration = 0);

	/// Draw line separated by hit point (billboard square) or not separated if 'is_hit' = 'false'
	/// start: Start point
	/// end: End point
	/// is_hit: Is hit
	/// unit_offset_of_hit: Unit offset on the line where the hit occurs
	/// hit_size: Size of the hit point
	/// hit_color: Color of the hit point and line before hit
	/// after_hit_color: Color of line after hit position
	/// duration: Duration of existence in seconds
	void draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit = 0.5f, const real_t &hit_size = 0.25f, const Color &hit_color = Colors::empty_color, const Color &after_hit_color = Colors::empty_color, const real_t &duration = 0);

#pragma region Normal

	/// Draw line
	/// a: Start point
	/// b: End point
	/// color: Line color
	/// duration: Duration of existence in seconds
	void draw_line(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw many lines
	/// lines: Array of line points. 1 line = 2 Vector3. The size of the array must be even.
	/// color: Lines color
	/// duration: Duration of existence in seconds
	void draw_lines(const PackedVector3Array &lines, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw ray
	/// origin: Origin
	/// direction: Direction
	/// length: Length
	/// color: Ray color
	/// duration: Duration of existence in seconds
	void draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw a sequence of points connected by lines
	/// path: Sequence of points
	/// color: Lines Color
	/// duration: Duration of existence in seconds
	void draw_line_path(const PackedVector3Array &path, const Color &color = Colors::empty_color, const real_t &duration = 0);

#pragma endregion // Normal

#pragma region Arrows

	/// Draw arrow
	/// Transform: Transform3D of the Arrow
	/// color: Arrow color
	/// duration: Duration of existence in seconds
	void draw_arrow(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw line with arrow
	/// a: Start point
	/// b: End point
	/// color: Line color
	/// arrow_size: Size of the arrow
	/// absolute_size: Is the 'arrowSize' absolute or relative to the length of the line?
	/// duration: Duration of existence in seconds
	void draw_arrow_line(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.5f, const bool &is_absolute_size = false, const real_t &duration = 0);

	/// Draw ray with arrow
	/// origin: Origin
	/// direction: Direction
	/// length: Length
	/// color: Ray color
	/// arrow_size: Size of the arrow
	/// absolute_size: Is the 'arrowSize' absolute or relative to the length of the line?
	/// duration: Duration of existence in seconds
	void draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.5f, const bool &is_absolute_size = false, const real_t &duration = 0);

	/// Draw a sequence of points connected by lines with arrows
	/// path: Sequence of points
	/// color: Color
	/// arrow_size: Size of the arrow
	/// absolute_size: Is the 'arrowSize' absolute or relative to the length of the line?
	/// duration: Duration of existence in seconds
	void draw_arrow_path(const PackedVector3Array &path, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.75f, const bool &is_absolute_size = true, const real_t &duration = 0);

#pragma endregion // Arrows
#pragma region Points

	/// Draw a sequence of points connected by lines with billboard squares
	/// path: Sequence of points
	/// points_color: Color of points
	/// lines_color: Color of lines
	/// size: Size of squares
	/// duration: Duration of existence in seconds
	void draw_point_path(const PackedVector3Array &path, const real_t &size = 0.25f, const Color &points_color = Colors::empty_color, const Color &lines_color = Colors::empty_color, const real_t &duration = 0);

#pragma endregion // Points
#pragma endregion // Lines

#pragma region Misc

	/// Draw a square that will always be turned towards the camera
	/// position: Center position of square
	/// size: Square size
	/// color: Color
	/// duration: Duration of existence in seconds
	void draw_square(const Vector3 &position, const real_t &size = 0.2f, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw a sequence of points using billboard squares
	/// path: Sequence of points
	/// color: Color
	/// size: Size of squares
	/// duration: Duration of existence in seconds
	void draw_points(const PackedVector3Array &points, const real_t &size = 0.25f, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw 3 intersecting lines with the given transformations
	/// transform: Transform3D of lines
	/// color: Color
	/// duration: Duration of existence in seconds
	void draw_position(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw 3 lines with the given transformations and arrows at the ends
	/// transform: Transform3D of lines
	/// color: Color
	/// is_centered: If 'true', then the lines will intersect in the center of the transform
	/// duration: Duration of existence in seconds
	void draw_gizmo(const Transform3D &transform, const Color &color = Colors::empty_color, const bool &is_centered = false, const real_t &duration = 0);

	/// Draw simple grid with given size and subdivision
	/// origin: Grid origin
	/// x_size: Direction and size of the X side. As an axis in the Basis.
	/// y_size: Direction and size of the Y side. As an axis in the Basis.
	/// subdivision: Number of cells for the X and Y axes
	/// color: Lines color
	/// is_centered: Draw lines relative to origin
	/// duration: Duration of existence in seconds
	void draw_grid(const Vector3 &origin, const Vector3 &x_size, const Vector3 &y_size, const Vector2i &subdivision, const Color &color = Colors::empty_color, const bool &is_centered = true, const real_t &duration = 0);

	/// Draw simple grid with given size and subdivision
	/// transform: Transform3D of the Grid
	/// subdivision: Number of cells for the X and Y axes
	/// color: Lines color
	/// is_centered: Draw lines relative to origin
	/// duration: Duration of existence in seconds
	void draw_grid_xf(const Transform3D &transform, const Vector2i &_subdivision, const Color &color = Colors::empty_color, const bool &is_centered = true, const real_t &duration = 0);

#pragma region Camera Frustum

	/// Draw camera frustum area
	/// camera: Camera node
	/// color: Color
	/// duration: Duration of existence in seconds
	void draw_camera_frustum(const class Camera3D *camera, const Color &color = Colors::empty_color, const real_t &duration = 0);

	/// Draw camera frustum area
	/// cameraFrustum: Array of frustum planes
	/// color: Color
	/// duration: Duration of existence in seconds
	void draw_camera_frustum_planes(const Array &camera_frustum, const Color &color = Colors::empty_color, const real_t &duration = 0);

#pragma endregion // Camera Frustum

#pragma endregion // Misc
#pragma endregion // 3D

#pragma region 2D
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
#pragma endregion // 2D
#pragma endregion // Exposed Draw Functions
};

class DebugDrawSceneManager : public CanvasLayer {
	GDCLASS(DebugDrawSceneManager, CanvasLayer)
protected:
	static void _bind_methods(){};
	void _notification(int what);

public:
	DebugDrawSceneManager(){};
};