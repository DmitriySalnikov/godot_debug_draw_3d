#include "config_scoped_3d.h"

#include "utils/utils.h"

void DDScopedConfig3D::_bind_methods() {
#define REG_CLASS_NAME DDScopedConfig3D
	REG_METHOD(_manual_unregister);

	REG_METHOD(set_thickness, "value");
	REG_METHOD(get_thickness);

	REG_METHOD(set_center_brightness, "value");
	REG_METHOD(get_center_brightness);
#undef REG_CLASS_NAME
}

void DDScopedConfig3D::_manual_unregister() {
	if (unregister_action) {
		unregister_action(thread_id, guard_id);
	}
	unregister_action = nullptr;
}

Ref<DDScopedConfig3D> DDScopedConfig3D::set_thickness(real_t value) {
	thickness = Math::clamp(value, (real_t)0, (real_t)100);
	return Ref<DDScopedConfig3D>(this);
}

real_t DDScopedConfig3D::get_thickness() {
	return thickness;
}

Ref<DDScopedConfig3D> DDScopedConfig3D::set_center_brightness(real_t value) {
	center_brightness = Math::clamp(value, (real_t)0, (real_t)1);
	return Ref<DDScopedConfig3D>(this);
}

real_t DDScopedConfig3D::get_center_brightness() {
	return center_brightness;
}

DDScopedConfig3D::DDScopedConfig3D() {
	unregister_action = nullptr;
	thread_id = 0;
	guard_id = 0;

	thickness = 0;
	center_brightness = 0;
}

DDScopedConfig3D::DDScopedConfig3D(const uint64_t &p_thread_id, const uint64_t &p_guard_id, const DDScopedConfig3D *parent, const unregister_func p_unreg) {
	unregister_action = p_unreg;

	thread_id = p_thread_id;
	guard_id = p_guard_id;

	thickness = parent->thickness;
	center_brightness = parent->center_brightness;
}

DDScopedConfig3D::~DDScopedConfig3D() {
	_manual_unregister();
}