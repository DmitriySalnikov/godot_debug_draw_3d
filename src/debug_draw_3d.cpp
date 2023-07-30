#include "debug_draw_3d.h"

#include "data_graphs.h"
#include "debug_draw_config_3d.h"
#include "debug_draw_manager.h"
#include "debug_geometry_container.h"
#include "draw_stats.h"
#include "utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/window.hpp>
GODOT_WARNING_RESTORE()

#include <limits.h>

#define NEED_LEAVE (!debug_enabled)

DebugDraw3D *DebugDraw3D::singleton = nullptr;

void DebugDraw3D::_bind_methods() {
#define REG_CLASS_NAME DebugDraw3D

#pragma region Parameters

	REG_PROP(empty_color, Variant::COLOR);
	REG_PROP_BOOL(debug_enabled);

	REG_PROP(config_3d, Variant::OBJECT);

	REG_PROP(custom_viewport, Variant::OBJECT);

#pragma endregion
#undef REG_CLASS_NAME

#pragma region Draw Functions
	ClassDB::bind_method(D_METHOD(NAMEOF(clear_3d_objects)), &DebugDraw3D::clear_3d_objects);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_sphere), "position", "radius", "color", "duration"), &DebugDraw3D::draw_sphere, 0.5f, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_sphere_xf), "transform", "color", "duration"), &DebugDraw3D::draw_sphere_xf, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_sphere_hd), "position", "radius", "color", "duration"), &DebugDraw3D::draw_sphere_hd, 0.5f, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_sphere_hd_xf), "transform", "color", "duration"), &DebugDraw3D::draw_sphere_hd_xf, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_cylinder), "transform", "color", "duration"), &DebugDraw3D::draw_cylinder, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_box), "position", "size", "color", "is_box_centered", "duration"), &DebugDraw3D::draw_box, Colors::empty_color, false, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_box_xf), "transform", "color", "is_box_centered", "duration"), &DebugDraw3D::draw_box_xf, Colors::empty_color, true, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_aabb), "aabb", "color", "duration"), &DebugDraw3D::draw_aabb, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_aabb_ab), "a", "b", "color", "duration"), &DebugDraw3D::draw_aabb_ab, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_line_hit), "start", "end", "hit", "is_hit", "hit_size", "hit_color", "after_hit_color", "duration"), &DebugDraw3D::draw_line_hit, 0.25f, Colors::empty_color, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_line_hit_offset), "start", "end", "is_hit", "unit_offset_of_hit", "hit_size", "hit_color", "after_hit_color", "duration"), &DebugDraw3D::draw_line_hit_offset, 0.5f, 0.25f, Colors::empty_color, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_line), "a", "b", "color", "duration"), &DebugDraw3D::draw_line, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_lines), "lines", "color", "duration"), &DebugDraw3D::draw_lines, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_ray), "origin", "direction", "length", "color", "duration"), &DebugDraw3D::draw_ray, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_line_path), "path", "color", "duration"), &DebugDraw3D::draw_line_path, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_arrow), "transform", "color", "duration"), &DebugDraw3D::draw_arrow, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_arrow_line), "a", "b", "color", "arrow_size", "is_absolute_size", "duration"), &DebugDraw3D::draw_arrow_line, Colors::empty_color, 0.5f, false, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_arrow_ray), "origin", "direction", "length", "color", "arrow_size", "is_absolute_size", "duration"), &DebugDraw3D::draw_arrow_ray, Colors::empty_color, 0.5f, false, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_arrow_path), "path", "color", "arrow_size", "is_absolute_size", "duration"), &DebugDraw3D::draw_arrow_path, Colors::empty_color, 0.75f, true, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_point_path), "path", "size", "points_color", "lines_color", "duration"), &DebugDraw3D::draw_point_path, 0.25f, Colors::empty_color, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_square), "position", "size", "color", "duration"), &DebugDraw3D::draw_square, 0.2f, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_points), "points", "size", "color", "duration"), &DebugDraw3D::draw_points, 0.2f, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_camera_frustum), "camera", "color", "duration"), &DebugDraw3D::draw_camera_frustum, Colors::empty_color, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_camera_frustum_planes), "camera_frustum", "color", "duration"), &DebugDraw3D::draw_camera_frustum_planes, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_position), "transform", "color", "duration"), &DebugDraw3D::draw_position, Colors::empty_color, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_gizmo), "transform", "color", "is_centered", "duration"), &DebugDraw3D::draw_gizmo, Colors::empty_color, false, 0);

	ClassDB::bind_method(D_METHOD(NAMEOF(draw_grid), "origin", "x_size", "y_size", "subdivision", "color", "is_centered", "duration"), &DebugDraw3D::draw_grid, Colors::empty_color, true, 0);
	ClassDB::bind_method(D_METHOD(NAMEOF(draw_grid_xf), "transform", "subdivision", "color", "is_centered", "duration"), &DebugDraw3D::draw_grid_xf, Colors::empty_color, true, 0);

#pragma endregion // Draw Functions

	ClassDB::bind_method(D_METHOD(NAMEOF(get_render_stats)), &DebugDraw3D::get_render_stats);
}

DebugDraw3D::DebugDraw3D() {
	ASSIGN_SINGLETON(DebugDraw3D);
}

void DebugDraw3D::init(DebugDrawManager *root) {
	root_node = root;
	set_config_3d(nullptr);

#ifndef DISABLE_DEBUG_RENDERING
	dgc = std::make_unique<DebugGeometryContainer>(this);
#endif
}

DebugDraw3D::~DebugDraw3D() {
	UNASSIGN_SINGLETON(DebugDraw3D);

#ifndef DISABLE_DEBUG_RENDERING
	dgc.reset();
#endif

	config_3d.unref();
	root_node = nullptr;
}

void DebugDraw3D::process(double delta) {
#ifndef DISABLE_DEBUG_RENDERING
	// Update 3D debug
	dgc->update_geometry(delta);
#endif
}

void DebugDraw3D::_set_base_world_node(Node *_world_base) {
#ifndef DISABLE_DEBUG_RENDERING
	dgc->set_world(_world_base);
#else
	return;
#endif
}

Node *DebugDraw3D::get_root_node() {
	return root_node;
}

void DebugDraw3D::set_custom_editor_viewport(std::vector<SubViewport *> _viewports) {
	custom_editor_viewports = _viewports;
}

std::vector<SubViewport *> DebugDraw3D::get_custom_editor_viewports() {
	return custom_editor_viewports;
}

void DebugDraw3D::set_empty_color(const Color &_col) {
}

Color DebugDraw3D::get_empty_color() const {
	return Colors::empty_color;
}

#pragma region Exposed Parameters

void DebugDraw3D::set_debug_enabled(const bool &_state) {
	debug_enabled = _state;

	if (!_state) {
		clear_3d_objects();
	}
}

bool DebugDraw3D::is_debug_enabled() const {
	return debug_enabled;
}

void DebugDraw3D::set_config_3d(Ref<DebugDrawConfig3D> _cfg) {
	if (_cfg.is_valid()) {
		config_3d = _cfg;
	} else {
		config_3d = Ref<DebugDrawConfig3D>();
		config_3d.instantiate();
	}
}

Ref<DebugDrawConfig3D> DebugDraw3D::get_config_3d() const {
	return config_3d;
}

void DebugDraw3D::set_custom_viewport(Viewport *_viewport) {
	custom_viewport = _viewport;
}

Viewport *DebugDraw3D::get_custom_viewport() const {
	return custom_viewport;
}

#pragma endregion

#pragma region Draw Functions

Ref<DebugDrawStats> DebugDraw3D::get_render_stats() {
#ifndef DISABLE_DEBUG_RENDERING
	if (!dgc) return Ref<DebugDrawStats>();
	return dgc->get_render_stats();
#else
	return Ref<DebugDrawStats>();
#endif
}

void DebugDraw3D::clear_3d_objects() {
#ifndef DISABLE_DEBUG_RENDERING
	if (!dgc) return;
	dgc->clear_3d_objects();
#else
	return;
#endif
}

#ifndef DISABLE_DEBUG_RENDERING
#define CALL_TO_DGC(func, ...)                                          \
	if (!dgc || NEED_LEAVE || config_3d->is_freeze_3d_render()) return; \
	dgc->func(__VA_ARGS__)
#else
#define CALL_TO_DGC(func, ...) return
#endif

#pragma region Spheres

void DebugDraw3D::draw_sphere(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_sphere, position, radius, color, duration);
}

void DebugDraw3D::draw_sphere_xf(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_sphere_xf, transform, color, duration);
}

void DebugDraw3D::draw_sphere_hd(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_sphere_hd, position, radius, color, duration);
}

void DebugDraw3D::draw_sphere_hd_xf(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_sphere_hd_xf, transform, color, duration);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugDraw3D::draw_cylinder(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_cylinder, transform, color, duration);
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugDraw3D::draw_box(const Vector3 &position, const Vector3 &size, const Color &color, const bool &is_box_centered, const real_t &duration) {
	CALL_TO_DGC(draw_box, position, size, color, is_box_centered, duration);
}

void DebugDraw3D::draw_box_xf(const Transform3D &transform, const Color &color, const bool &is_box_centered, const real_t &duration) {
	CALL_TO_DGC(draw_box_xf, transform, color, is_box_centered, duration);
}

void DebugDraw3D::draw_aabb(const AABB &aabb, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_aabb, aabb, color, duration);
}

void DebugDraw3D::draw_aabb_ab(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_aabb_ab, a, b, color, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugDraw3D::draw_line_hit(const Vector3 &start, const Vector3 &end, const Vector3 &hit, const bool &is_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	CALL_TO_DGC(draw_line_hit, start, end, hit, is_hit, hit_size, hit_color, after_hit_color, duration);
}

void DebugDraw3D::draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	CALL_TO_DGC(draw_line_hit_offset, start, end, is_hit, unit_offset_of_hit, hit_size, hit_color, after_hit_color, duration);
}

#pragma region Normal

void DebugDraw3D::draw_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_line, a, b, color, duration);
}

void DebugDraw3D::draw_lines(const PackedVector3Array &lines, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_lines, lines, color, duration);
}

void DebugDraw3D::draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_ray, origin, direction, length, color, duration);
}

void DebugDraw3D::draw_line_path(const PackedVector3Array &path, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_line_path, path, color, duration);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugDraw3D::draw_arrow(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_arrow, transform, color, duration);
}

void DebugDraw3D::draw_arrow_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CALL_TO_DGC(draw_arrow_line, a, b, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw3D::draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CALL_TO_DGC(draw_arrow_ray, origin, direction, length, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw3D::draw_arrow_path(const PackedVector3Array &path, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CALL_TO_DGC(draw_arrow_path, path, color, arrow_size, is_absolute_size, duration);
}

#pragma endregion // Arrows
#pragma region Points

void DebugDraw3D::draw_point_path(const PackedVector3Array &path, const real_t &size, const Color &points_color, const Color &lines_color, const real_t &duration) {
	CALL_TO_DGC(draw_point_path, path, size, points_color, lines_color, duration);
}

#pragma endregion // Points
#pragma endregion // Lines
#pragma region Misc

void DebugDraw3D::draw_square(const Vector3 &position, const real_t &size, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_square, position, size, color, duration);
}

void DebugDraw3D::draw_points(const PackedVector3Array &points, const real_t &size, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_points, points, size, color, duration);
}

void DebugDraw3D::draw_position(const Transform3D &transform, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_position, transform, color, duration);
}

void DebugDraw3D::draw_gizmo(const Transform3D &transform, const Color &color, const bool &is_centered, const real_t &duration) {
	CALL_TO_DGC(draw_gizmo, transform, color, is_centered, duration);
}

void DebugDraw3D::draw_grid(const Vector3 &origin, const Vector3 &x_size, const Vector3 &y_size, const Vector2i &subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
	CALL_TO_DGC(draw_grid, origin, x_size, y_size, subdivision, color, is_centered, duration);
}

void DebugDraw3D::draw_grid_xf(const Transform3D &transform, const Vector2i &_subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
	CALL_TO_DGC(draw_grid_xf, transform, _subdivision, color, is_centered, duration);
}

#pragma region Camera Frustum

void DebugDraw3D::draw_camera_frustum(const Camera3D *camera, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_camera_frustum, camera, color, duration);
}

void DebugDraw3D::draw_camera_frustum_planes(const Array &camera_frustum, const Color &color, const real_t &duration) {
	CALL_TO_DGC(draw_camera_frustum_planes, camera_frustum, color, duration);
}

#pragma endregion // Camera Frustum
#pragma endregion // Misc

#undef NEED_LEAVE