#include "config_3d.h"
#include "utils/utils.h"

#include <limits.h>

void DebugDrawConfig3D::_bind_methods() {
#define REG_CLASS_NAME DebugDrawConfig3D

#pragma region Parameters

	REG_PROP_BOOL(freeze_3d_render);
	REG_PROP_BOOL(visible_instance_bounds);
	REG_PROP_BOOL(use_frustum_culling);
	REG_PROP(cull_by_distance, Variant::FLOAT);
	REG_PROP_BOOL(force_use_camera_from_scene);
	REG_PROP(geometry_render_layers, Variant::INT);
	REG_PROP(line_hit_color, Variant::COLOR);
	REG_PROP(line_after_hit_color, Variant::COLOR);

#pragma endregion
#undef REG_CLASS_NAME
}

void DebugDrawConfig3D::set_freeze_3d_render(const bool &_state) {
	freeze_3d_render = _state;
}

bool DebugDrawConfig3D::is_freeze_3d_render() const {
	return freeze_3d_render;
}

void DebugDrawConfig3D::set_visible_instance_bounds(const bool &_state) {
	visible_instance_bounds = _state;
}

bool DebugDrawConfig3D::is_visible_instance_bounds() const {
	return visible_instance_bounds;
}

void DebugDrawConfig3D::set_use_frustum_culling(const bool &_state) {
	use_frustum_culling = _state;
}

bool DebugDrawConfig3D::is_use_frustum_culling() const {
	return use_frustum_culling;
}

void DebugDrawConfig3D::set_cull_by_distance(const real_t &_distance) {
	cull_by_distance = Math::clamp(_distance, -1.0f, (real_t)INT_MAX);
}

real_t DebugDrawConfig3D::get_cull_by_distance() const {
	return cull_by_distance;
}

void DebugDrawConfig3D::set_force_use_camera_from_scene(const bool &_state) {
	force_use_camera_from_scene = _state;
}

bool DebugDrawConfig3D::is_force_use_camera_from_scene() const {
	return force_use_camera_from_scene;
}

void DebugDrawConfig3D::set_geometry_render_layers(const int32_t &_layers) {
	geometry_render_layers = _layers;
}

int32_t DebugDrawConfig3D::get_geometry_render_layers() const {
	return geometry_render_layers;
}

void DebugDrawConfig3D::set_line_hit_color(const Color &_new_color) {
	line_hit_color = _new_color;
}

Color DebugDrawConfig3D::get_line_hit_color() const {
	return line_hit_color;
}

void DebugDrawConfig3D::set_line_after_hit_color(const Color &_new_color) {
	line_after_hit_color = _new_color;
}

Color DebugDrawConfig3D::get_line_after_hit_color() const {
	return line_after_hit_color;
}
