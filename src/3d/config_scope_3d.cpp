#include "config_scope_3d.h"

#include "utils/utils.h"

void DebugDraw3DScopeConfig::_bind_methods() {
#define REG_CLASS_NAME DebugDraw3DScopeConfig
	REG_METHOD(_manual_unregister);

	REG_METHOD(set_thickness, "value");
	REG_METHOD(get_thickness);

	REG_METHOD(set_center_brightness, "value");
	REG_METHOD(get_center_brightness);

	REG_METHOD(set_hd_sphere, "value");
	REG_METHOD(is_hd_sphere);

	REG_METHOD(set_plane_size, "value");
	REG_METHOD(get_plane_size);
#undef REG_CLASS_NAME
}

void DebugDraw3DScopeConfig::_manual_unregister() {
	if (unregister_action) {
		unregister_action(thread_id, guard_id);
	}
	unregister_action = nullptr;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_thickness(real_t value) {
	thickness = Math::clamp(value, (real_t)0, (real_t)100);
	return Ref<DebugDraw3DScopeConfig>(this);
}

real_t DebugDraw3DScopeConfig::get_thickness() {
	return thickness;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_center_brightness(real_t value) {
	center_brightness = Math::clamp(value, (real_t)0, (real_t)1);
	return Ref<DebugDraw3DScopeConfig>(this);
}

real_t DebugDraw3DScopeConfig::get_center_brightness() {
	return center_brightness;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_hd_sphere(bool value) {
	hd_sphere = value;
	return this;
}

bool DebugDraw3DScopeConfig::is_hd_sphere() {
	return hd_sphere;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_plane_size(real_t value) {
	plane_size = value;
	return this;
}

real_t DebugDraw3DScopeConfig::get_plane_size() {
	return plane_size;
}

DebugDraw3DScopeConfig::DebugDraw3DScopeConfig() {
	unregister_action = nullptr;
	thread_id = 0;
	guard_id = 0;

	thickness = 0;
	center_brightness = 0;
	hd_sphere = false;
	plane_size = INFINITY;
}

DebugDraw3DScopeConfig::DebugDraw3DScopeConfig(const uint64_t &p_thread_id, const uint64_t &p_guard_id, const DebugDraw3DScopeConfig *parent, const unregister_func p_unreg) {
	unregister_action = p_unreg;

	thread_id = p_thread_id;
	guard_id = p_guard_id;

	thickness = parent->thickness;
	center_brightness = parent->center_brightness;
	hd_sphere = parent->hd_sphere;
	plane_size = parent->plane_size;
}

DebugDraw3DScopeConfig::~DebugDraw3DScopeConfig() {
	_manual_unregister();
}
