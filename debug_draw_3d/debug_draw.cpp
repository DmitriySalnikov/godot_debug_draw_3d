#include "debug_draw.h"

#include <ConfigFile.hpp>
#include <Directory.hpp>
#include <Engine.hpp>
#include <File.hpp>
#include <GlobalConstants.hpp>
#include <Label.hpp>
#include <ProjectSettings.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>

#include <chrono>

#define RECALL_TO_SINGLETON(func, ...)                  \
	if (RecallToSingleton && this != get_singleton()) { \
		get_singleton()->func(__VA_ARGS__);             \
		return;                                         \
	}

#define RECALL_TO_SINGLETON_NA(func)                    \
	if (RecallToSingleton && this != get_singleton()) { \
		get_singleton()->func();                        \
		return;                                         \
	}

DebugDraw3D *DebugDraw3D::singleton = nullptr;
int DebugDraw3D::instance_counter = 0;

void DebugDraw3D::_register_methods() {
	register_method("get_singleton", &DebugDraw3D::get_singleton_gdscript);

#define REG_METHOD(name) register_method(#name, &DebugDraw3D::name)

	REG_METHOD(_enter_tree);
	REG_METHOD(_exit_tree);
	REG_METHOD(_process);

#pragma region Constants
#define CONST_REG(_enum, _const) \
	register_property(TEXT(_enum##_##_const), &DebugDraw3D::fake_set_const, &DebugDraw3D::get_##_enum##_##_const, (int64_t)_enum::_const)

	CONST_REG(BlockPosition, LeftTop);
	CONST_REG(BlockPosition, RightTop);
	CONST_REG(BlockPosition, LeftBottom);
	CONST_REG(BlockPosition, RightBottom);

	CONST_REG(FPSGraphTextFlags, None);
	CONST_REG(FPSGraphTextFlags, Current);
	CONST_REG(FPSGraphTextFlags, Avarage);
	CONST_REG(FPSGraphTextFlags, Max);
	CONST_REG(FPSGraphTextFlags, Min);
	CONST_REG(FPSGraphTextFlags, All);

#undef CONST_REG
#pragma endregion

#pragma region Parameters

#define REG_PROP(name, def) register_property(#name, &DebugDraw3D::set_##name, &DebugDraw3D::get_##name, def);
#define REG_PROP_BOOL(name, def) register_property(#name, &DebugDraw3D::set_##name, &DebugDraw3D::is_##name, def);

	REG_PROP_BOOL(recall_to_singleton, true);
	REG_PROP_BOOL(debug_enabled, true);
	REG_PROP_BOOL(freeze_3d_render, false);
	REG_PROP_BOOL(use_frustum_culling, true);
	REG_PROP_BOOL(force_use_camera_from_scene, false);
	REG_PROP(text_block_position, (int)BlockPosition::LeftTop);
	REG_PROP(text_block_offset, Vector2(8, 8));
	REG_PROP(text_padding, Vector2(2, 1));
	REG_PROP(text_default_duration, (int64_t)500);
	REG_PROP(text_foreground_color, Color(1, 1, 1));
	REG_PROP(text_background_color, Color(0.3f, 0.3f, 0.3f, 0.8f));
	REG_PROP_BOOL(fps_graph_enabled, false);
	REG_PROP_BOOL(fps_graph_frame_time_mode, true);
	REG_PROP_BOOL(fps_graph_centered_graph_line, true);
	REG_PROP(fps_graph_show_text_flags, (int)FPSGraphTextFlags::All);
	REG_PROP(fps_graph_size, Vector2(256, 64));
	REG_PROP(fps_graph_offset, Vector2(8, 8));
	REG_PROP(fps_graph_position, (int)BlockPosition::RightTop);
	REG_PROP(fps_graph_line_color, Color(1, 0.27f, 0, 1));
	REG_PROP(fps_graph_text_color, Color(0.96f, 0.96f, 0.96f, 1));
	REG_PROP(fps_graph_background_color, Color(0.2f, 0.2f, 0.2f, 0.6f));
	REG_PROP(fps_graph_border_color, Color(0, 0, 0, 1));
	REG_PROP(line_hit_color, Color(1, 0, 0, 1));
	REG_PROP(line_after_hit_color, Color(0, 1, 0, 1));
	REG_PROP(custom_viewport, (Viewport *)nullptr);
	REG_PROP(custom_canvas, (CanvasItem *)nullptr);

#undef REG_PROP
#undef REG_PROP_BOOL

#pragma endregion

	REG_METHOD(get_rendered_primitives_count);

#undef REG_METHOD
}

void DebugDraw3D::_init() {
}

void DebugDraw3D::_enter_tree() {
	instance_counter++;

	if (instance_counter > (IS_EDITOR_HINT() ? 2 : 1)) {
		PRINT_WARNING("Too many " TEXT(DebugDraw3D) " instances entered tree: " + String::num_int64(instance_counter));
	}

	if (IS_EDITOR_HINT() && get_parent() == get_tree()->get_root()) {
		PRINT_WARNING("Ignore this instance. It may be created as an Autoload script inside the editor");
		return;
	}

	if (!singleton) {
		singleton = this;
	} else {
		PRINT_ERROR("Only 1 instance of " TEXT(DebugDraw3D) " is allowed");
	}

	if (IS_EDITOR_HINT()) {
		TextBlockPosition = BlockPosition::LeftBottom;
		FPSGraphOffset = Vector2(12, 72);
		FPSGraphPosition = BlockPosition::LeftTop;
	}

	set_process_priority(INT64_MAX);
}

void DebugDraw3D::_exit_tree() {
	instance_counter--;

	if (instance_counter < 0) {
		PRINT_WARNING("Too many " TEXT(DebugDraw3D) " instances exited tree: " + String::num_int64(instance_counter));
	}

	if (singleton && singleton == this) {
		singleton = nullptr;
	}
}

void DebugDraw3D::_process() {
	// TODO
}

#pragma region Exposed Parameters

void DebugDraw3D::set_recall_to_singleton(bool state) {
	RecallToSingleton = state;
}

bool DebugDraw3D::is_recall_to_singleton() {
	return RecallToSingleton;
}

void DebugDraw3D::set_debug_enabled(bool state) {
	DebugEnabled = state;

	if (!state) {
		clear_all();
	}
}

bool DebugDraw3D::is_debug_enabled() {
	return DebugEnabled;
}

void DebugDraw3D::set_freeze_3d_render(bool state) {
	Freeze3DRender = state;
}

bool DebugDraw3D::is_freeze_3d_render() {
	return Freeze3DRender;
}

void DebugDraw3D::set_use_frustum_culling(bool state) {
	UseFrustumCulling = state;
}

bool DebugDraw3D::is_use_frustum_culling() {
	return UseFrustumCulling;
}

void DebugDraw3D::set_force_use_camera_from_scene(bool state) {
	ForceUseCameraFromScene = state;
}

bool DebugDraw3D::is_force_use_camera_from_scene() {
	return ForceUseCameraFromScene;
}

void DebugDraw3D::set_text_block_position(int position) {
	TextBlockPosition = (BlockPosition)position;
}

int DebugDraw3D::get_text_block_position() {
	return TextBlockPosition;
}

void DebugDraw3D::set_text_block_offset(Vector2 offset) {
	TextBlockOffset = offset;
}

Vector2 DebugDraw3D::get_text_block_offset() {
	return TextBlockOffset;
}

void DebugDraw3D::set_text_padding(Vector2 padding) {
	TextPadding = padding;
}

Vector2 DebugDraw3D::get_text_padding() {
	return TextPadding;
}

void DebugDraw3D::set_text_default_duration(int64_t duration) {
	TextDefaultDuration = duration;
}

int64_t DebugDraw3D::get_text_default_duration() {
	return TextDefaultDuration;
}

void DebugDraw3D::set_text_foreground_color(Color new_color) {
	TextForegroundColor = new_color;
}

Color DebugDraw3D::get_text_foreground_color() {
	return TextForegroundColor;
}

void DebugDraw3D::set_text_background_color(Color new_color) {
	TextBackgroundColor = new_color;
}

Color DebugDraw3D::get_text_background_color() {
	return TextBackgroundColor;
}

void DebugDraw3D::set_fps_graph_enabled(bool state) {
	FPSGraphEnabled = state;
}

bool DebugDraw3D::is_fps_graph_enabled() {
	return FPSGraphEnabled;
}

void DebugDraw3D::set_fps_graph_frame_time_mode(bool state) {
	FPSGraphFrameTimeMode = state;
}

bool DebugDraw3D::is_fps_graph_frame_time_mode() {
	return FPSGraphFrameTimeMode;
}

void DebugDraw3D::set_fps_graph_centered_graph_line(bool state) {
	FPSGraphCenteredGraphLine = state;
}

bool DebugDraw3D::is_fps_graph_centered_graph_line() {
	return FPSGraphCenteredGraphLine;
}

void DebugDraw3D::set_fps_graph_show_text_flags(int flags) {
	FPSGraphShowTextFlags = (FPSGraphTextFlags)flags;
}

int DebugDraw3D::get_fps_graph_show_text_flags() {
	return FPSGraphShowTextFlags;
}

void DebugDraw3D::set_fps_graph_size(Vector2 size) {
	FPSGraphSize = size;
}

Vector2 DebugDraw3D::get_fps_graph_size() {
	return FPSGraphSize;
}

void DebugDraw3D::set_fps_graph_offset(Vector2 offset) {
	FPSGraphOffset = offset;
}

Vector2 DebugDraw3D::get_fps_graph_offset() {
	return FPSGraphOffset;
}

void DebugDraw3D::set_fps_graph_position(int position) {
	FPSGraphPosition = (BlockPosition)position;
}

int DebugDraw3D::get_fps_graph_position() {
	return FPSGraphPosition;
}

void DebugDraw3D::set_fps_graph_line_color(Color new_color) {
	FPSGraphLineColor = new_color;
}

Color DebugDraw3D::get_fps_graph_line_color() {
	return FPSGraphLineColor;
}

void DebugDraw3D::set_fps_graph_text_color(Color new_color) {
	FPSGraphTextColor = new_color;
}

Color DebugDraw3D::get_fps_graph_text_color() {
	return FPSGraphTextColor;
}

void DebugDraw3D::set_fps_graph_background_color(Color new_color) {
	FPSGraphBackgroundColor = new_color;
}

Color DebugDraw3D::get_fps_graph_background_color() {
	return FPSGraphBackgroundColor;
}

void DebugDraw3D::set_fps_graph_border_color(Color new_color) {
	FPSGraphBorderColor = new_color;
}

Color DebugDraw3D::get_fps_graph_border_color() {
	return FPSGraphBorderColor;
}

void DebugDraw3D::set_line_hit_color(Color new_color) {
	LineHitColor = new_color;
}

Color DebugDraw3D::get_line_hit_color() {
	return LineHitColor;
}

void DebugDraw3D::set_line_after_hit_color(Color new_color) {
	LineAfterHitColor = new_color;
}

Color DebugDraw3D::get_line_after_hit_color() {
	return LineAfterHitColor;
}

void DebugDraw3D::set_custom_viewport(Viewport *viewport) {
	CustomViewport = viewport;
	// TODO
}

Viewport *DebugDraw3D::get_custom_viewport() {
	return CustomViewport;
}

void DebugDraw3D::set_custom_canvas(CanvasItem *canvas) {
	CustomCanvas = canvas;
	// TODO
}

CanvasItem *DebugDraw3D::get_custom_canvas() {
	return CustomCanvas;
}

#pragma endregion

Dictionary DebugDraw3D::get_rendered_primitives_count() {
	return Dictionary::make(
			"instances", rendered_instances,
			"wireframes", rendered_wireframes,
			"total", rendered_instances + rendered_wireframes);
}

void DebugDraw3D::OnCanvaItemDraw(CanvasItem ci) {
	RECALL_TO_SINGLETON(OnCanvaItemDraw, ci);
}

#pragma region Draw Functions

void DebugDraw3D::clear_3d_objects() {
	RECALL_TO_SINGLETON_NA(clear_3d_objects);
}

void DebugDraw3D::clear_2d_objects() {
	RECALL_TO_SINGLETON_NA(clear_2d_objects);
}

void DebugDraw3D::clear_all() {
	RECALL_TO_SINGLETON_NA(clear_all);
}

#pragma region 3D

#pragma region Spheres

void DebugDraw3D::draw_sphere(Vector3 position, float radius, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_sphere, position, radius, color, duration);
}

void DebugDraw3D::draw_sphere_xf(Transform transform, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_sphere_xf, transform, color, duration);
}

#pragma endregion // Spheres

#pragma region Cylinders

void DebugDraw3D::draw_cylinder(Vector3 position, float radius, float height, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_cylinder, position, radius, height, color, duration);
}

void DebugDraw3D::draw_cylinder_xf(Transform transform, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_cylinder_xf, transform, color, duration);
}

#pragma endregion // Cylinders

#pragma region Boxes

void DebugDraw3D::draw_box(Vector3 position, Quat rotation, Vector3 size, Color color, float duration, bool isBoxCentered) {
	RECALL_TO_SINGLETON(draw_box, position, rotation, size, color, duration, isBoxCentered);
}

void DebugDraw3D::draw_box_xf(Transform transform, Color color, float duration, bool isBoxCentered) {
	RECALL_TO_SINGLETON(draw_box_xf, transform, color, duration, isBoxCentered);
}

void DebugDraw3D::draw_aabb(AABB aabb, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_aabb, aabb, color, duration);
}

void DebugDraw3D::draw_aabb_ab(Vector3 a, Vector3 b, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_aabb_ab, a, b, color, duration);
}

#pragma endregion // Boxes

#pragma region Lines

void DebugDraw3D::draw_line_3d_hit(Vector3 a, Vector3 b, bool is_hit, float unitOffsetOfHit, float hitSize, float duration, Color hitColor, Color afterHitColor) {
	RECALL_TO_SINGLETON(draw_line_3d_hit, a, b, is_hit, unitOffsetOfHit, hitSize, duration, hitColor, afterHitColor);
}

#pragma region Normal

void DebugDraw3D::draw_line_3d(Vector3 a, Vector3 b, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_line_3d, a, b, color, duration);
}

void DebugDraw3D::draw_ray_3d(Vector3 origin, Vector3 direction, float length, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_ray_3d, origin, direction, length, color, duration);
}

void DebugDraw3D::draw_line_path_3d(PoolVector3Array path, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_line_path_3d, path, color, duration);
}

void DebugDraw3D::draw_line_path_3d_arr(Array path, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_line_path_3d_arr, path, color, duration);
}

#pragma endregion // Normal

#pragma region Arrows

void DebugDraw3D::draw_arrow_line_3d(Vector3 a, Vector3 b, Color color, float duration, float arrowSize, bool absoluteSize) {
	RECALL_TO_SINGLETON(draw_arrow_line_3d, a, b, color, duration, arrowSize, absoluteSize);
}

void DebugDraw3D::draw_arrow_ray_3d(Vector3 origin, Vector3 direction, float length, Color color, float duration, float arrowSize, bool absoluteSize) {
	RECALL_TO_SINGLETON(draw_arrow_ray_3d, origin, direction, length, color, duration, arrowSize, absoluteSize);
}

void DebugDraw3D::draw_arrow_path_3d(PoolVector3Array path, Color color, float duration, float arrowSize, bool absoluteSize) {
	RECALL_TO_SINGLETON(draw_arrow_path_3d, path, color, duration, arrowSize, absoluteSize);
}

void DebugDraw3D::draw_arrow_path_3d_arr(Array path, Color color, float duration, float arrowSize, bool absoluteSize) {
	RECALL_TO_SINGLETON(draw_arrow_path_3d_arr, path, color, duration, arrowSize, absoluteSize);
}

#pragma endregion // Arrows
#pragma endregion // Lines

#pragma region Misc

void DebugDraw3D::draw_billboard_square(Vector3 position, float size, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_billboard_square, position, size, color, duration);
}

#pragma region Camera Frustum

void DebugDraw3D::draw_camera_frustum(class Camera *camera, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_camera_frustum, camera, color, duration);
}

void DebugDraw3D::draw_camera_frustum_planes(Array cameraFrustum, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_camera_frustum_planes, cameraFrustum, color, duration);
}

void DebugDraw3D::draw_camera_frustum_planes_c(Plane planes[], Color color, float duration) {
	RECALL_TO_SINGLETON(draw_camera_frustum_planes_c, planes, color, duration);
}

#pragma endregion // Camera Frustum

void DebugDraw3D::draw_position_3d(Vector3 position, Quat rotation, Vector3 scale, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_position_3d, position, rotation, scale, color, duration);
}

void DebugDraw3D::draw_position_3d_xf(Transform transform, Color color, float duration) {
	RECALL_TO_SINGLETON(draw_position_3d_xf, transform, color, duration);
}

#pragma endregion // Misc
#pragma endregion // 3D

#pragma region 2D

void DebugDraw3D::begin_text_group(String groupTitle, int groupPriority, Color groupColor, bool showTitle) {
	RECALL_TO_SINGLETON(begin_text_group, groupTitle, groupPriority, groupColor, showTitle);
}

void DebugDraw3D::end_text_group() {
	RECALL_TO_SINGLETON_NA(end_text_group);
}

void DebugDraw3D::set_text(String key, Variant value, int priority, Color colorOfValue, float duration) {
	RECALL_TO_SINGLETON(set_text, key, value, priority, colorOfValue, duration);
}

#pragma endregion // 2D

#pragma endregion // Draw Functions

#undef RECALL_TO_SINGLETON
#undef RECALL_TO_SINGLETON_NA