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

	REG_METHOD(set_viewport, "value");
	REG_METHOD(get_viewport);

	REG_METHOD(set_no_depth_test, "value");
	REG_METHOD(is_no_depth_test);
#undef REG_CLASS_NAME
}

void DebugDraw3DScopeConfig::_manual_unregister() {
	if (unregister_action) {
		unregister_action(thread_id, guard_id);
	}
	unregister_action = nullptr;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_thickness(real_t _value) const {
	set_thickness_selfreturn(_value);
	return Ref<DebugDraw3DScopeConfig>(this);
}

NAPI NSELF_RETURN DebugDraw3DScopeConfig::set_thickness_selfreturn(real_t _value) const {
	data->thickness = Math::clamp(_value, (real_t)0, (real_t)100);
}

real_t DebugDraw3DScopeConfig::get_thickness() const {
	return data->thickness;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_center_brightness(real_t _value) const {
	set_center_brightness_selfreturn(_value);
	return Ref<DebugDraw3DScopeConfig>(this);
}

NAPI NSELF_RETURN DebugDraw3DScopeConfig::set_center_brightness_selfreturn(real_t _value) const {
	data->center_brightness = Math::clamp(_value, (real_t)0, (real_t)1);
}

real_t DebugDraw3DScopeConfig::get_center_brightness() const {
	return data->center_brightness;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_hd_sphere(bool _value) const {
	data->hd_sphere = _value;
	return Ref<DebugDraw3DScopeConfig>(this);
}

NAPI NSELF_RETURN DebugDraw3DScopeConfig::set_hd_sphere_selfreturn(bool _value) const {
	data->hd_sphere = _value;
}

bool DebugDraw3DScopeConfig::is_hd_sphere() const {
	return data->hd_sphere;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_plane_size(real_t _value) const {
	data->plane_size = _value;
	return Ref<DebugDraw3DScopeConfig>(this);
}

NAPI NSELF_RETURN DebugDraw3DScopeConfig::set_plane_size_selfreturn(real_t _value) const {
	data->plane_size = _value;
}

real_t DebugDraw3DScopeConfig::get_plane_size() const {
	return data->plane_size;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_viewport(Viewport *_value) const {
	data->dcd.viewport = _value;
	return Ref<DebugDraw3DScopeConfig>(this);
}

NAPI NSELF_RETURN DebugDraw3DScopeConfig::set_viewport_selfreturn(godot::Viewport *_value) const {
	data->dcd.viewport = _value;
}

Viewport *DebugDraw3DScopeConfig::get_viewport() const {
	return data->dcd.viewport;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_no_depth_test(bool _value) const {
	data->dcd.no_depth_test = _value;
	return Ref<DebugDraw3DScopeConfig>(this);
}

NAPI NSELF_RETURN DebugDraw3DScopeConfig::set_no_depth_test_selfreturn(bool _value) const {
	data->dcd.no_depth_test = _value;
}

bool DebugDraw3DScopeConfig::is_no_depth_test() const {
	return data->dcd.no_depth_test;
}

DebugDraw3DScopeConfig::DebugDraw3DScopeConfig() {
	unregister_action = nullptr;
	thread_id = 0;
	guard_id = 0;

	data = std::make_shared<Data>();
}

DebugDraw3DScopeConfig::DebugDraw3DScopeConfig(const uint64_t &p_thread_id, const uint64_t &p_guard_id, const std::shared_ptr<DebugDraw3DScopeConfig::Data> &p_parent, const unregister_func p_unreg) {
	unregister_action = p_unreg;

	thread_id = p_thread_id;
	guard_id = p_guard_id;

	data = std::make_shared<Data>(p_parent);
}

DebugDraw3DScopeConfig::~DebugDraw3DScopeConfig() {
	_manual_unregister();
}

DebugDraw3DScopeConfig::Data::Data() {
	thickness = 0;
	center_brightness = 0;
	hd_sphere = false;
	plane_size = INFINITY;
	dcd = {};
}

DebugDraw3DScopeConfig::Data::Data(const std::shared_ptr<Data> &p_parent) {
	thickness = p_parent->thickness;
	center_brightness = p_parent->center_brightness;
	hd_sphere = p_parent->hd_sphere;
	plane_size = p_parent->plane_size;

	dcd.viewport = p_parent->dcd.viewport;
	dcd.no_depth_test = p_parent->dcd.no_depth_test;
}
