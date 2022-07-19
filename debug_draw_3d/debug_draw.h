#pragma once

#include "utils.h"

#include <CanvasItem.hpp>
#include <GlobalConstants.hpp>
#include <Godot.hpp>
#include <Node.hpp>
#include <Reference.hpp>
#include <Viewport.hpp>

using namespace godot;

class DebugDraw3D : public Node {
	GODOT_CLASS(DebugDraw3D, Node)

	static DebugDraw3D *singleton;
	static int instance_counter;

public:
	enum BlockPosition : int {
		LeftTop,
		RightTop,
		LeftBottom,
		RightBottom,
	};

	enum FPSGraphTextFlags : int {
		None = 0,
		Current = 1 << 0,
		Avarage = 1 << 1,
		Max = 1 << 2,
		Min = 1 << 3,
		All = Current | Avarage | Max | Min
	};

	// HACK for constants
#define CONST_GET(_enum, _const) \
	int64_t get_##_enum##_##_const() { return _enum::_const; }

	void fake_set_const(int64_t val){};
	CONST_GET(BlockPosition, LeftTop);
	CONST_GET(BlockPosition, RightTop);
	CONST_GET(BlockPosition, LeftBottom);
	CONST_GET(BlockPosition, RightBottom);

	CONST_GET(FPSGraphTextFlags, None);
	CONST_GET(FPSGraphTextFlags, Current);
	CONST_GET(FPSGraphTextFlags, Avarage);
	CONST_GET(FPSGraphTextFlags, Max);
	CONST_GET(FPSGraphTextFlags, Min);
	CONST_GET(FPSGraphTextFlags, All);

#undef CONST_GET

	int64_t rendered_wireframes = 0;
	int64_t rendered_instances = 0;

#pragma region Exposed Parameter Values

	// GENERAL

	/// Recall all calls from DebugDraw3D instance to its singleton if needed
	bool RecallToSingleton = true;
	/// Enable or disable all debug draw.
	bool DebugEnabled = true;
	/// Debug for debug...
	bool Freeze3DRender = false;
	/// Geometry culling based on camera frustum. Change to false to disable it
	bool UseFrustumCulling = true;
	/// Force use camera placed on edited scene. Usable for editor.
	bool ForceUseCameraFromScene = false;

	// TEXT

	/// Position of text block
	BlockPosition TextBlockPosition = BlockPosition::LeftTop;
	/// Offset from the corner selected in <see cref="TextBlockPosition"/>
	Vector2 TextBlockOffset = Vector2(8, 8);
	/// Text padding for each line
	Vector2 TextPadding = Vector2(2, 1);
	/// How long HUD text lines remain shown after being invoked.
	int64_t TextDefaultDuration = 500;
	/// Color of the text drawn as HUD
	Color TextForegroundColor = Color(1, 1, 1);
	/// Background color of the text drawn as HUD
	Color TextBackgroundColor = Color(0.3f, 0.3f, 0.3f, 0.8f);

	// FPS GRAPH

	/// Is FPSGraph enabled
	bool FPSGraphEnabled = false;
	/// Switch between frame time and FPS modes
	bool FPSGraphFrameTimeMode = true;
	/// Draw a graph line aligned vertically in the center
	bool FPSGraphCenteredGraphLine = true;
	/// Sets the text visibility
	FPSGraphTextFlags FPSGraphShowTextFlags = FPSGraphTextFlags::All;
	/// Size of the FPS Graph. The width is equal to the number of stored frames.
	Vector2 FPSGraphSize = Vector2(256, 64);
	/// Offset from the corner selected in <see cref="FPSGraphPosition"/>
	Vector2 FPSGraphOffset = Vector2(8, 8);
	/// FPS Graph position
	BlockPosition FPSGraphPosition = BlockPosition::RightTop;
	/// Graph line color
	Color FPSGraphLineColor = Color(1, 0.27f, 0, 1) /* OrangeRed */;
	/// Color of the info text
	Color FPSGraphTextColor = Color(0.96f, 0.96f, 0.96f, 1) /* WhiteSmoke */;
	/// Background color
	Color FPSGraphBackgroundColor = Color(0.2f, 0.2f, 0.2f, 0.6f);
	/// Border color
	Color FPSGraphBorderColor = Color(0, 0, 0, 1) /* Black */;

	// GEOMETRY

	/// Color of line with hit
	Color LineHitColor = Color(1, 0, 0, 1) /* Red */;
	/// Color of line after hit
	Color LineAfterHitColor = Color(0, 1, 0, 1) /* Green */;

	// Misc

	/// Custom <see cref="Viewport"/> to use for frustum culling. Usually used in editor.
	Viewport *CustomViewport = nullptr;
	/// Custom <see cref="CanvasItem"/> to draw on it. Set to <see langword="null"/> to disable.
	CanvasItem *CustomCanvas = nullptr;
#pragma endregion

	DebugDraw3D *get_singleton_gdscript() { return singleton; };

public:
	static void _register_methods();
	void _init();

	static DebugDraw3D *get_singleton() { return singleton; };

	void _enter_tree();
	void _exit_tree();
	void _process();

#pragma region Exposed Parameters
	void set_recall_to_singleton(bool state);
	bool is_recall_to_singleton();
	void set_debug_enabled(bool state);
	bool is_debug_enabled();
	void set_freeze_3d_render(bool state);
	bool is_freeze_3d_render();
	void set_use_frustum_culling(bool state);
	bool is_use_frustum_culling();
	void set_force_use_camera_from_scene(bool state);
	bool is_force_use_camera_from_scene();
	void set_text_block_position(int /*BlockPosition*/ position);
	int /*BlockPosition*/ get_text_block_position();
	void set_text_block_offset(Vector2 offset);
	Vector2 get_text_block_offset();
	void set_text_padding(Vector2 padding);
	Vector2 get_text_padding();
	void set_text_default_duration(int64_t duration);
	int64_t get_text_default_duration();
	void set_text_foreground_color(Color new_color);
	Color get_text_foreground_color();
	void set_text_background_color(Color new_color);
	Color get_text_background_color();
	void set_fps_graph_enabled(bool state);
	bool is_fps_graph_enabled();
	void set_fps_graph_frame_time_mode(bool state);
	bool is_fps_graph_frame_time_mode();
	void set_fps_graph_centered_graph_line(bool state);
	bool is_fps_graph_centered_graph_line();
	void set_fps_graph_show_text_flags(int /*FPSGraphTextFlags*/ flags);
	int /*FPSGraphTextFlags*/ get_fps_graph_show_text_flags();
	void set_fps_graph_size(Vector2 size);
	Vector2 get_fps_graph_size();
	void set_fps_graph_offset(Vector2 offset);
	Vector2 get_fps_graph_offset();
	void set_fps_graph_position(int /*BlockPosition*/ position);
	int /*BlockPosition*/ get_fps_graph_position();
	void set_fps_graph_line_color(Color new_color);
	Color get_fps_graph_line_color();
	void set_fps_graph_text_color(Color new_color);
	Color get_fps_graph_text_color();
	void set_fps_graph_background_color(Color new_color);
	Color get_fps_graph_background_color();
	void set_fps_graph_border_color(Color new_color);
	Color get_fps_graph_border_color();
	void set_line_hit_color(Color new_color);
	Color get_line_hit_color();
	void set_line_after_hit_color(Color new_color);
	Color get_line_after_hit_color();
	void set_custom_viewport(Viewport *viewport);
	Viewport *get_custom_viewport();
	void set_custom_canvas(CanvasItem *canvas);
	CanvasItem *get_custom_canvas();
#pragma endregion

	Dictionary get_rendered_primitives_count();

	void OnCanvaItemDraw(CanvasItem ci);

#pragma region Draw Functions

	/// Clear all 3D objects
	void clear_3d_objects();

	/// Clear all 2D objects
	void clear_2d_objects();

	/// Clear all debug objects
	void clear_all();

#pragma region 3D

#pragma region Spheres

	/// Draw sphere
	/// <param name="position">Position of the sphere center</param>
	/// <param name="radius">Sphere radius</param>
	/// <param name="color">Sphere color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_sphere(Vector3 position, float radius, Color color = Utils::empty_color, float duration = 0);

	/// Draw sphere
	/// <param name="transform">Transform of the sphere</param>
	/// <param name="color">Sphere color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_sphere_xf(Transform transform, Color color = Utils::empty_color, float duration = 0);

#pragma endregion // Spheres

#pragma region Cylinders

	/// Draw vertical cylinder
	/// <param name="position">Center position</param>
	/// <param name="radius">Cylinder radius</param>
	/// <param name="height">Cylinder height</param>
	/// <param name="color">Cylinder color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_cylinder(Vector3 position, float radius, float height, Color color = Utils::empty_color, float duration = 0);

	/// Draw vertical cylinder
	/// <param name="transform">Cylinder transform</param>
	/// <param name="color">Cylinder color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_cylinder_xf(Transform transform, Color color = Utils::empty_color, float duration = 0);

#pragma endregion // Cylinders

#pragma region Boxes

	/// Draw rotated box
	/// <param name="position">Position of the box</param>
	/// <param name="rotation">Box rotation</param>
	/// <param name="size">Box size</param>
	/// <param name="color">Box color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	/// <param name="isBoxCentered">Use <paramref name="position"/> as center of the box</param>
	void draw_box(Vector3 position, Quat rotation, Vector3 size, Color color = Utils::empty_color, float duration = 0, bool isBoxCentered = true);

	/// Draw rotated box
	/// <param name="transform">Box transform</param>
	/// <param name="color">Box color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	/// <param name="isBoxCentered">Use <paramref name="position"/> as center of the box</param>
	void draw_box_xf(Transform transform, Color color = Utils::empty_color, float duration = 0, bool isBoxCentered = true);

	/// Draw AABB
	/// <param name="aabb">AABB</param>
	/// <param name="color">Box color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_aabb(AABB aabb, Color color = Utils::empty_color, float duration = 0);

	/// Draw AABB from <paramref name="a"/> to <paramref name="b"/>
	/// <param name="a">Firts corner</param>
	/// <param name="b">Second corner</param>
	/// <param name="color">Box color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_aabb_ab(Vector3 a, Vector3 b, Color color = Utils::empty_color, float duration = 0);

#pragma endregion // Boxes

#pragma region Lines

	/// Draw line separated by hit point (billboard square) or not separated if <paramref name="is_hit"/> = <see langword="false"/>
	/// <param name="a">Start point</param>
	/// <param name="b">End point</param>
	/// <param name="is_hit">Is hit</param>
	/// <param name="unitOffsetOfHit">Unit offset on the line where the hit occurs</param>
	/// <param name="duration">Duration of existence in seconds</param>
	/// <param name="hitColor">Color of the hit point and line before hit</param>
	/// <param name="afterHitColor">Color of line after hit position</param>
	void draw_line_3d_hit(Vector3 a, Vector3 b, bool is_hit, float unitOffsetOfHit = 0.5f, float hitSize = 0.25f, float duration = 0, Color hitColor = Utils::empty_color, Color afterHitColor = Utils::empty_color);

#pragma region Normal

	/// Draw line
	/// <param name="a">Start point</param>
	/// <param name="b">End point</param>
	/// <param name="color">Line color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_line_3d(Vector3 a, Vector3 b, Color color = Utils::empty_color, float duration = 0);

	/// Draw ray
	/// <param name="origin">Origin</param>
	/// <param name="direction">Direction</param>
	/// <param name="length">Length</param>
	/// <param name="color">Ray color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_ray_3d(Vector3 origin, Vector3 direction, float length, Color color = Utils::empty_color, float duration = 0);

	/// Draw a sequence of points connected by lines
	/// <param name="path">Sequence of points</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_line_path_3d(PoolVector3Array path, Color color = Utils::empty_color, float duration = 0);

	/// Draw a sequence of points connected by lines
	/// <param name="path">Sequence of points</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_line_path_3d_arr(Array path, Color color = Utils::empty_color, float duration = 0);

#pragma endregion // Normal

#pragma region Arrows

	/// Draw line with arrow
	/// <param name="a">Start point</param>
	/// <param name="b">End point</param>
	/// <param name="color">Line color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	/// <param name="arrowSize">Size of the arrow</param>
	/// <param name="absoluteSize">Is the <paramref name="arrowSize"/> absolute or relative to the length of the line?</param>
	void draw_arrow_line_3d(Vector3 a, Vector3 b, Color color = Utils::empty_color, float duration = 0, float arrowSize = 0.15f, bool absoluteSize = false);

	/// Draw ray with arrow
	/// <param name="origin">Origin</param>
	/// <param name="direction">Direction</param>
	/// <param name="length">Length</param>
	/// <param name="color">Ray color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	/// <param name="arrowSize">Size of the arrow</param>
	/// <param name="absoluteSize">Is the <paramref name="arrowSize"/> absolute or relative to the length of the line?</param>
	void draw_arrow_ray_3d(Vector3 origin, Vector3 direction, float length, Color color = Utils::empty_color, float duration = 0, float arrowSize = 0.15f, bool absoluteSize = false);

	/// Draw a sequence of points connected by lines with arrows
	/// <param name="path">Sequence of points</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	/// <param name="arrowSize">Size of the arrow</param>
	/// <param name="absoluteSize">Is the <paramref name="arrowSize"/> absolute or relative to the length of the line?</param>
	void draw_arrow_path_3d(PoolVector3Array path, Color color = Utils::empty_color, float duration = 0, float arrowSize = 0.75f, bool absoluteSize = true);

	/// Draw a sequence of points connected by lines with arrows
	/// <param name="path">Sequence of points</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	/// <param name="arrowSize">Size of the arrow</param>
	/// <param name="absoluteSize">Is the <paramref name="arrowSize"/> absolute or relative to the length of the line?</param>
	void draw_arrow_path_3d_arr(Array path, Color color = Utils::empty_color, float duration = 0, float arrowSize = 0.75f, bool absoluteSize = true);

#pragma endregion // Arrows
#pragma endregion // Lines

#pragma region Misc

	/// Draw a square that will always be turned towards the camera
	/// <param name="position">Center position of square</param>
	/// <param name="color">Color</param>
	/// <param name="size">Unit size</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_billboard_square(Vector3 position, float size = 0.2f, Color color = Utils::empty_color, float duration = 0);

#pragma region Camera Frustum

	/// Draw camera frustum area
	/// <param name="camera">Camera node</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_camera_frustum(class Camera *camera, Color color = Utils::empty_color, float duration = 0);

	/// Draw camera frustum area
	/// <param name="cameraFrustum">Array of frustum planes</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_camera_frustum_planes(Array cameraFrustum, Color color = Utils::empty_color, float duration = 0);

	/// Draw camera frustum area
	/// <param name="planes">Array of frustum planes</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_camera_frustum_planes_c(Plane planes[], Color color = Utils::empty_color, float duration = 0);

#pragma endregion // Camera Frustum

	/// Draw 3 intersecting lines with the given transformations
	/// <param name="position">Center position</param>
	/// <param name="rotation">Rotation</param>
	/// <param name="scale">Scale</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_position_3d(Vector3 position, Quat rotation, Vector3 scale, Color color = Utils::empty_color, float duration = 0);

	/// Draw 3 intersecting lines with the given transformations
	/// <param name="transform">Transform</param>
	/// <param name="color">Color</param>
	/// <param name="duration">Duration of existence in seconds</param>
	void draw_position_3d_xf(Transform transform, Color color = Utils::empty_color, float duration = 0);

#pragma endregion // Misc
#pragma endregion // 3D

#pragma region 2D

	/// Begin text group
	/// <param name="groupTitle">Group title and ID</param>
	/// <param name="groupPriority">Group priority</param>
	/// <param name="showTitle">Whether to show the title</param>
	void begin_text_group(String groupTitle, int groupPriority = 0, Color groupColor = Utils::empty_color, bool showTitle = true);

	/// End text group. Should be called after <see cref="BeginTextGroup(String, int, bool)"/> if you don't need more than one group.
	/// If you need to create 2+ groups just call again <see cref="BeginTextGroup(String, int, bool)"/>
	/// and this function in the end.
	/// <param name="groupTitle">Group title and ID</param>
	/// <param name="groupPriority">Group priority</param>
	/// <param name="showTitle">Whether to show the title</param>
	void end_text_group();

	/// Add or update text in overlay
	/// <param name="key">Name of field if <paramref name="value"/> exists, otherwise whole line will equal <paramref name="key"/>.</param>
	/// <param name="value">Value of field</param>
	/// <param name="priority">Priority of this line. Lower value is higher position.</param>
	/// <param name="duration">Expiration time</param>
	void set_text(String key, Variant value = Variant(), int priority = 0, Color colorOfValue = Utils::empty_color, float duration = -1);

#pragma endregion // 2D

#pragma endregion // Draw Functions
};
