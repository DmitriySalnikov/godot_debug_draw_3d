#include "debug_draw_3d.h"

#include "data_graphs.h"
#include "debug_draw_config_3d.h"
#include "debug_draw_manager.h"
#include "debug_geometry_container.h"
#include "draw_stats.h"
#include "geometry_generators.h"
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
#define IS_DEFAULT_COLOR(name) (name == Colors::empty_color)
#define CHECK_BEFORE_CALL() \
	if (!dgc || NEED_LEAVE || config_3d->is_freeze_3d_render()) return;
#else
#define CHECK_BEFORE_CALL()
#endif

#ifndef DISABLE_DEBUG_RENDERING

#pragma region Spheres

void DebugDraw3D::draw_sphere_base(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration, const bool &hd) {
	CHECK_BEFORE_CALL();
	Transform3D t(Basis(), position);
	t.basis.scale(Vector3_ONE * (radius * 2));

	draw_sphere_xf_base(t, color, duration, hd);
}

void DebugDraw3D::draw_sphere_xf_base(const Transform3D &transform, const Color &color, const real_t &duration, const bool &hd) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_instance(
			hd ? InstanceType::SPHERES_HD : InstanceType::SPHERES,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::chartreuse : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * 0.51f));
}

void DebugDraw3D::draw_sphere(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	draw_sphere_base(position, radius, color, duration, false);
}

void DebugDraw3D::draw_sphere_xf(const Transform3D &transform, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	draw_sphere_xf_base(transform, color, duration, false);
}

void DebugDraw3D::draw_sphere_hd(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	draw_sphere_base(position, radius, color, duration, true);
}

void DebugDraw3D::draw_sphere_hd_xf(const Transform3D &transform, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	draw_sphere_xf_base(transform, color, duration, true);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugDraw3D::draw_cylinder(const Transform3D &transform, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_instance(
			InstanceType::CYLINDERS,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::forest_green : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CylinderRadiusForSphere));
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugDraw3D::draw_box(const Vector3 &position, const Vector3 &size, const Color &color, const bool &is_box_centered, const real_t &duration) {
	CHECK_BEFORE_CALL();
	draw_box_xf(Transform3D(Basis().scaled(size), position), color, is_box_centered, duration);
}

void DebugDraw3D::draw_box_xf(const Transform3D &transform, const Color &color, const bool &is_box_centered, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);

	/// It's possible to use less space to contain box, but this method works better in more cases
	SphereBounds sb(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CubeRadiusForSphere);

	if (!is_box_centered) {
		sb.position = transform.origin + transform.basis.get_scale() * 0.5f;
	}
	dgc->geometry_pool.add_or_update_instance(
			is_box_centered ? InstanceType::CUBES_CENTERED : InstanceType::CUBES,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::forest_green : color,
			sb);
}

void DebugDraw3D::draw_aabb(const AABB &aabb, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(aabb.position, aabb.position + aabb.size, bottom, top, diag);
	draw_box(bottom, diag, color, false, duration);
}

void DebugDraw3D::draw_aabb_ab(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(a, b, bottom, top, diag);
	draw_box(bottom, diag, color, false, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugDraw3D::draw_line_hit(const Vector3 &start, const Vector3 &end, const Vector3 &hit, const bool &is_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	if (is_hit) {
		dgc->geometry_pool.add_or_update_line(duration, { start, hit }, IS_DEFAULT_COLOR(hit_color) ? get_config_3d()->get_line_hit_color() : hit_color);
		dgc->geometry_pool.add_or_update_line(duration, { hit, end }, IS_DEFAULT_COLOR(after_hit_color) ? get_config_3d()->get_line_after_hit_color() : after_hit_color);

		dgc->geometry_pool.add_or_update_instance(
				InstanceType::BILLBOARD_SQUARES,
				duration,
				Transform3D(Basis().scaled(Vector3_ONE * hit_size), hit),
				IS_DEFAULT_COLOR(hit_color) ? get_config_3d()->get_line_hit_color() : hit_color,
				SphereBounds(hit, MathUtils::CubeRadiusForSphere * hit_size));
	} else {
		dgc->geometry_pool.add_or_update_line(duration, { start, end }, IS_DEFAULT_COLOR(hit_color) ? get_config_3d()->get_line_hit_color() : hit_color);
	}
}

void DebugDraw3D::draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	if (is_hit && unit_offset_of_hit >= 0 && unit_offset_of_hit <= 1) {
		draw_line_hit(start, end, ((end - start).normalized() * start.distance_to(end) * unit_offset_of_hit + start), is_hit, hit_size, hit_color, after_hit_color, duration);
	} else {
		draw_line_hit(start, end, {}, is_hit, hit_size, hit_color, after_hit_color, duration);
	}
}

#pragma region Normal

void DebugDraw3D::draw_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_line(duration, { a, b }, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_lines(const PackedVector3Array &lines, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);

	if (lines.size() % 2 != 0) {
		PRINT_ERROR("The size of the lines array must be even. " + String::num_int64(lines.size()) + " is not even.");
		return;
	}

	std::vector<Vector3> l(lines.size());
	{
		memcpy(l.data(), lines.ptr(), (size_t)lines.size() * sizeof(Vector3));
	}

	dgc->geometry_pool.add_or_update_line(duration, l, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_lines_c(const std::vector<Vector3> &lines, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);

	if (lines.size() % 2 != 0) {
		PRINT_ERROR("The size of the lines array must be even. " + String::num_int64(lines.size()) + " is not even.");
		return;
	}

	dgc->geometry_pool.add_or_update_line(duration, lines, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_line(duration, { origin, origin + direction * length }, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_line_path(const PackedVector3Array &path, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	if (path.size() < 2) {
		PRINT_ERROR("Line path must contains at least 2 points. " + String::num_int64(path.size()) + " is not enough.");
		return;
	}

	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_line(duration, GeometryGenerator::CreateLinesFromPath(path), IS_DEFAULT_COLOR(color) ? Colors::light_green : color);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugDraw3D::create_arrow(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	Vector3 dir = (b - a);
	real_t size = (is_absolute_size ? arrow_size : dir.length() * arrow_size) * 2;
	Vector3 pos = b - dir.normalized() * size;

	const Vector3 UP = Vector3(0.0000000001f, 1, 0);
	Transform3D t;
	t.set_look_at(pos, b, UP);
	t.scale(Vector3_ONE * (size));
	t.origin = pos;

	dgc->geometry_pool.add_or_update_instance(
			InstanceType::ARROWHEADS,
			duration,
			t,
			IS_DEFAULT_COLOR(color) ? Colors::light_green : color,
			SphereBounds(t.origin - t.basis.get_column(2) * 0.5f, MathUtils::ArrowRadiusForSphere * size));
}

void DebugDraw3D::draw_arrow(const Transform3D &transform, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);

	dgc->geometry_pool.add_or_update_instance(
			InstanceType::ARROWHEADS,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::light_green : color,
			SphereBounds(transform.origin - transform.basis.get_column(2) * 0.5f, MathUtils::ArrowRadiusForSphere * MathUtils::get_max_basis_length(transform.basis)));
}

void DebugDraw3D::draw_arrow_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_line(duration, { a, b }, IS_DEFAULT_COLOR(color) ? Colors::light_green : color);

	create_arrow(a, b, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw3D::draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CHECK_BEFORE_CALL();
	draw_arrow_line(origin, origin + direction * length, color, arrow_size, is_absolute_size, duration);
}

void DebugDraw3D::draw_arrow_path(const PackedVector3Array &path, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_line(duration, GeometryGenerator::CreateLinesFromPath(path), IS_DEFAULT_COLOR(color) ? Colors::light_green : color);

	for (int i = 0; i < path.size() - 1; i++) {
		create_arrow(path[i], path[i + 1], color, arrow_size, is_absolute_size, duration);
	}
}

#pragma endregion // Arrows
#pragma region Points

void DebugDraw3D::draw_point_path(const PackedVector3Array &path, const real_t &size, const Color &points_color, const Color &lines_color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	draw_points(path, size, IS_DEFAULT_COLOR(points_color) ? Colors::red : points_color, duration);
	draw_line_path(path, IS_DEFAULT_COLOR(lines_color) ? Colors::green : lines_color, duration);
}

#pragma endregion // Points
#pragma endregion // Lines

#pragma region Misc

void DebugDraw3D::draw_square(const Vector3 &position, const real_t &size, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	Transform3D t(Basis(), position);
	t.basis.scale(Vector3_ONE * size);

	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_instance(
			InstanceType::BILLBOARD_SQUARES,
			duration,
			t,
			IS_DEFAULT_COLOR(color) ? Colors::red : color,
			SphereBounds(position, MathUtils::CubeRadiusForSphere * size));
}

void DebugDraw3D::draw_points(const PackedVector3Array &points, const real_t &size, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	for (int i = 0; i < points.size(); i++) {
		draw_square(points[i], size, color, duration);
	}
}

void DebugDraw3D::draw_position(const Transform3D &transform, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_instance(
			InstanceType::POSITIONS,
			duration,
			transform,
			IS_DEFAULT_COLOR(color) ? Colors::crimson : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::AxisRadiusForSphere));
}

void DebugDraw3D::draw_gizmo(const Transform3D &transform, const Color &color, const bool &is_centered, const real_t &duration) {
	CHECK_BEFORE_CALL();
	LOCK_GUARD(dgc->datalock);

	bool is_color_empty = IS_DEFAULT_COLOR(color);
#define COLOR(axis) is_color_empty ? Colors::axis_##axis : color
#define MINUS(axis) transform.origin - transform.basis.get_column(axis)
#define PLUS(axis) transform.origin + transform.basis.get_column(axis)

	if (is_centered) {
		draw_arrow_line(MINUS(0 /** 0.5f*/), PLUS(0 /** 0.5f*/), COLOR(x), 0.1f, true, duration);
		draw_arrow_line(MINUS(1 /** 0.5f*/), PLUS(1 /** 0.5f*/), COLOR(y), 0.1f, true, duration);
		draw_arrow_line(MINUS(2 /** 0.5f*/), PLUS(2 /** 0.5f*/), COLOR(z), 0.1f, true, duration);
	} else {
		draw_arrow_line(transform.origin, PLUS(0), COLOR(x), 0.15f, true, duration);
		draw_arrow_line(transform.origin, PLUS(1), COLOR(y), 0.15f, true, duration);
		draw_arrow_line(transform.origin, PLUS(2), COLOR(z), 0.15f, true, duration);
	}

#undef COLOR
#undef MINUS
#undef PLUS
}

void DebugDraw3D::draw_grid(const Vector3 &origin, const Vector3 &x_size, const Vector3 &y_size, const Vector2i &subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
	draw_grid_xf(Transform3D(Basis(x_size, y_size.cross(x_size).normalized(), y_size), origin),
			subdivision, color, is_centered, duration);
}

void DebugDraw3D::draw_grid_xf(const Transform3D &transform, const Vector2i &_subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
	CHECK_BEFORE_CALL();

#define MAX_SUBDIVISIONS 1024 * 1024
	ERR_FAIL_COND(_subdivision.x > MAX_SUBDIVISIONS);
	ERR_FAIL_COND(_subdivision.y > MAX_SUBDIVISIONS);
	LOCK_GUARD(dgc->datalock);

	Vector2i subdivision = _subdivision.abs();
	subdivision = Vector2i(Math::clamp(subdivision.x, 1, MAX_SUBDIVISIONS), Math::clamp(subdivision.y, 1, MAX_SUBDIVISIONS));
	Vector3 x_axis = transform.basis.get_column(0);
	Vector3 z_axis = transform.basis.get_column(2);
	Vector3 x_d = x_axis / (real_t)subdivision.x;
	Vector3 z_d = z_axis / (real_t)subdivision.y;

#undef MAX_SUBDIVISIONS

	Vector3 origin = is_centered ?
							 transform.origin - x_d * (real_t)subdivision.x * 0.5 - z_d * (real_t)subdivision.y * 0.5 :
							 transform.origin;

	std::vector<Vector3> lines;
	for (int x = 0; x < subdivision.x + 1; x++) {
		lines.push_back(origin + x_d * (real_t)x);
		lines.push_back(origin + x_d * (real_t)x + z_axis);
	}

	for (int y = 0; y < subdivision.y + 1; y++) {
		lines.push_back(origin + z_d * (real_t)y);
		lines.push_back(origin + z_d * (real_t)y + x_axis);
	}

	draw_lines_c(lines, IS_DEFAULT_COLOR(color) ? Colors::white : color, duration);
}

#pragma region Camera Frustum

void DebugDraw3D::draw_camera_frustum_planes_c(const std::array<Plane, 6> &planes, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	auto lines = GeometryGenerator::CreateCameraFrustumLines(planes);

	LOCK_GUARD(dgc->datalock);
	dgc->geometry_pool.add_or_update_line(duration, lines, IS_DEFAULT_COLOR(color) ? Colors::red : color);
}

void DebugDraw3D::draw_camera_frustum(const Camera3D *camera, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	ERR_FAIL_COND(!camera);
	draw_camera_frustum_planes(camera->get_frustum(), color, duration);
}

void DebugDraw3D::draw_camera_frustum_planes(const Array &camera_frustum, const Color &color, const real_t &duration) {
	CHECK_BEFORE_CALL();
	std::array<Plane, 6> planes = { Plane() };

	if (camera_frustum.size() == 6) {
		for (int i = 0; i < camera_frustum.size(); i++) {
			planes[i] = camera_frustum[i];
		}
	} else {
		PRINT_ERROR("Camera frustum requires an array of 6 planes. Recieved " + String::num_int64(camera_frustum.size()));
	}

	draw_camera_frustum_planes_c(planes, color, duration);
}

#pragma endregion // Camera Frustum

#pragma endregion // Misc
#endif

#undef IS_DEFAULT_COLOR
#undef NEED_LEAVE