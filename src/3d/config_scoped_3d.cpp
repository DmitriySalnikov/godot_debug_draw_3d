#include "config_scoped_3d.h"

#include "utils/utils.h"

void DebugDraw3DScopedConfig::_bind_methods() {
#define REG_CLASS_NAME DebugDraw3DScopedConfig
	REG_METHOD(_manual_unregister);

	REG_METHOD(set_thickness, "value");
	REG_METHOD(get_thickness);

	REG_METHOD(set_center_brightness, "value");
	REG_METHOD(get_center_brightness);
#undef REG_CLASS_NAME
}

void DebugDraw3DScopedConfig::_manual_unregister() {
	if (unregister_action) {
		unregister_action(thread_id, guard_id);
	}
	unregister_action = nullptr;
}

Ref<DebugDraw3DScopedConfig> DebugDraw3DScopedConfig::set_thickness(real_t value) {
	thickness = Math::clamp(value, (real_t)0, (real_t)100);
	return Ref<DebugDraw3DScopedConfig>(this);
}

real_t DebugDraw3DScopedConfig::get_thickness() {
	return thickness;
}

Ref<DebugDraw3DScopedConfig> DebugDraw3DScopedConfig::set_center_brightness(real_t value) {
	center_brightness = Math::clamp(value, (real_t)0, (real_t)1);
	return Ref<DebugDraw3DScopedConfig>(this);
}

real_t DebugDraw3DScopedConfig::get_center_brightness() {
	return center_brightness;
}

DebugDraw3DScopedConfig::DebugDraw3DScopedConfig() {
	unregister_action = nullptr;
	thread_id = 0;
	guard_id = 0;

	thickness = 0;
	center_brightness = 0;
}

DebugDraw3DScopedConfig::DebugDraw3DScopedConfig(const uint64_t &p_thread_id, const uint64_t &p_guard_id, const DebugDraw3DScopedConfig *parent, const unregister_func p_unreg) {
	unregister_action = p_unreg;

	thread_id = p_thread_id;
	guard_id = p_guard_id;

	thickness = parent->thickness;
	center_brightness = parent->center_brightness;
}

DebugDraw3DScopedConfig::~DebugDraw3DScopedConfig() {
	_manual_unregister();
}
