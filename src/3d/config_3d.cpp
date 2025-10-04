#include "config_3d.h"
#include "utils/utils.h"

#include <limits.h>

void DebugDraw3DConfig::_bind_methods() {
#define REG_CLASS_NAME DebugDraw3DConfig

#pragma region Parameters

	REG_PROP_BOOL(freeze_3d_render);
	REG_PROP_BOOL(visible_instance_bounds);
	REG_PROP_BOOL(use_frustum_culling);
	REG_PROP(frustum_culling_mode, Variant::INT);
	REG_PROP(frustum_length_scale, Variant::FLOAT);
	REG_PROP_BOOL(force_use_camera_from_scene);
	REG_PROP(geometry_render_layers, Variant::INT);
	REG_PROP(line_hit_color, Variant::COLOR);
	REG_PROP(line_after_hit_color, Variant::COLOR);

#pragma endregion
#undef REG_CLASS_NAME

	BIND_ENUM_CONSTANT(FRUSTUM_DISABLED);
	BIND_ENUM_CONSTANT(FRUSTUM_ROUGH);
	BIND_ENUM_CONSTANT(FRUSTUM_PRECISE);
}

void DebugDraw3DConfig::set_freeze_3d_render(const bool &_state) {
	freeze_3d_render = _state;
}

bool DebugDraw3DConfig::is_freeze_3d_render() const {
	return freeze_3d_render;
}

void DebugDraw3DConfig::set_visible_instance_bounds(const bool &_state) {
	visible_instance_bounds = _state;
}

bool DebugDraw3DConfig::is_visible_instance_bounds() const {
	return visible_instance_bounds;
}

void DebugDraw3DConfig::set_use_frustum_culling(const bool &_state) {
	frustum_culling_mode = _state ? CullingMode::FRUSTUM_PRECISE : CullingMode::FRUSTUM_ROUGH;
}

bool DebugDraw3DConfig::is_use_frustum_culling() const {
	return frustum_culling_mode != CullingMode::FRUSTUM_DISABLED;
}

void DebugDraw3DConfig::set_frustum_culling_mode(const CullingMode _mode) {
	frustum_culling_mode = _mode;
}

DebugDraw3DConfig::CullingMode DebugDraw3DConfig::get_frustum_culling_mode() const {
	return frustum_culling_mode;
}

void DebugDraw3DConfig::set_frustum_length_scale(const real_t &_distance) {
	frustum_length_scale = Math::clamp(_distance, (real_t)0.0, (real_t)1.0);
}

real_t DebugDraw3DConfig::get_frustum_length_scale() const {
	return frustum_length_scale;
}

void DebugDraw3DConfig::set_force_use_camera_from_scene(const bool &_state) {
	force_use_camera_from_scene = _state;
}

bool DebugDraw3DConfig::is_force_use_camera_from_scene() const {
	return force_use_camera_from_scene;
}

void DebugDraw3DConfig::set_geometry_render_layers(const int32_t &_layers) {
	geometry_render_layers = _layers;
}

int32_t DebugDraw3DConfig::get_geometry_render_layers() const {
	return geometry_render_layers;
}

void DebugDraw3DConfig::set_line_hit_color(const Color &_new_color) {
	line_hit_color = _new_color;
}

Color DebugDraw3DConfig::get_line_hit_color() const {
	return line_hit_color;
}

void DebugDraw3DConfig::set_line_after_hit_color(const Color &_new_color) {
	line_after_hit_color = _new_color;
}

Color DebugDraw3DConfig::get_line_after_hit_color() const {
	return line_after_hit_color;
}
