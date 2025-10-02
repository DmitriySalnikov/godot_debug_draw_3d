#include "config_scope_3d.h"

#include "utils/utils.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/templates/hashfuncs.hpp>
GODOT_WARNING_RESTORE()

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

	REG_METHOD(set_transform, "value");
	REG_METHOD(get_transform);

	REG_METHOD(set_viewport, "value");
	REG_METHOD(get_viewport);

	REG_METHOD(set_no_depth_test, "value");
	REG_METHOD(is_no_depth_test);

	REG_METHOD(set_text_outline_color, "value");
	REG_METHOD(get_text_outline_color);

	REG_METHOD(set_text_outline_size, "value");
	REG_METHOD(get_text_outline_size);

	REG_METHOD(set_text_font, "value");
	REG_METHOD(get_text_font);

	REG_METHOD(set_text_fixed_size, "value");
	REG_METHOD(get_text_fixed_size);
#undef REG_CLASS_NAME
}

void DebugDraw3DScopeConfig::_manual_unregister() {
	if (unregister_action) {
		unregister_action(thread_id, guard_id);
	}
	unregister_action = nullptr;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_thickness(real_t _value) const {
	data->thickness = Math::clamp(_value, (real_t)0, (real_t)100);
	return Ref<DebugDraw3DScopeConfig>(this);
}

real_t DebugDraw3DScopeConfig::get_thickness() const {
	return data->thickness;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_center_brightness(real_t _value) const {
	data->center_brightness = Math::clamp(_value, (real_t)0, (real_t)1);
	return Ref<DebugDraw3DScopeConfig>(this);
}

real_t DebugDraw3DScopeConfig::get_center_brightness() const {
	return data->center_brightness;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_hd_sphere(bool _value) const {
	data->hd_sphere = _value;
	return Ref<DebugDraw3DScopeConfig>(this);
}

bool DebugDraw3DScopeConfig::is_hd_sphere() const {
	return data->hd_sphere;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_plane_size(real_t _value) const {
	data->plane_size = _value;
	return Ref<DebugDraw3DScopeConfig>(this);
}

real_t DebugDraw3DScopeConfig::get_plane_size() const {
	return data->plane_size;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_transform(Transform3D _value) const {
	const static Transform3D identity = Transform3D();

	data->transform = _value;
	data->custom_xform = _value != identity;
	return Ref<DebugDraw3DScopeConfig>(this);
}

Transform3D DebugDraw3DScopeConfig::get_transform() const {
	return data->transform;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_text_outline_color(Color _value) const {
	data->text_outline_color = _value;
	uint32_t hash = hash_murmur3_one_float(_value.r);
	hash = hash_murmur3_one_float(_value.g, hash);
	hash = hash_murmur3_one_float(_value.b, hash);
	data->text_outline_color_hash = hash_murmur3_one_float(_value.a, hash);

	return Ref<DebugDraw3DScopeConfig>(this);
}

Color DebugDraw3DScopeConfig::get_text_outline_color() const {
	return data->text_outline_color;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_text_outline_size(int32_t _value) const {
	data->text_outline_size = _value;
	return Ref<DebugDraw3DScopeConfig>(this);
}

int32_t DebugDraw3DScopeConfig::get_text_outline_size() const {
	return data->text_outline_size;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_text_fixed_size(bool value) const {
	data->text_fixed_size = value;
	return Ref<DebugDraw3DScopeConfig>(this);
}

bool DebugDraw3DScopeConfig::get_text_fixed_size() const {
	return data->text_fixed_size;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_text_font(Ref<Font> _value) const {
	data->text_font = _value;
	return Ref<DebugDraw3DScopeConfig>(this);
}

Ref<Font> DebugDraw3DScopeConfig::get_text_font() const {
	return data->text_font;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_viewport(Viewport *_value) const {
	data->dcd.viewport = _value;
	data->dcd.viewport_id = _value ? _value->get_instance_id() : 0;
	return Ref<DebugDraw3DScopeConfig>(this);
}

Viewport *DebugDraw3DScopeConfig::get_viewport() const {
	return data->dcd.viewport;
}

Ref<DebugDraw3DScopeConfig> DebugDraw3DScopeConfig::set_no_depth_test(bool _value) const {
	data->dcd.no_depth_test = _value;
	return Ref<DebugDraw3DScopeConfig>(this);
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

DebugDraw3DScopeConfig::DebugDraw3DScopeConfig(const uint64_t &p_thread_id, const uint64_t &p_guard_id, const DebugDraw3DScopeConfig::Data *p_parent, const unregister_func p_unreg) {
	unregister_action = p_unreg;

	thread_id = p_thread_id;
	guard_id = p_guard_id;

	data = std::make_shared<Data>(p_parent);
}

DebugDraw3DScopeConfig::~DebugDraw3DScopeConfig() {
	_manual_unregister();
}

DebugDraw3DScopeConfig::Data::Data() :
		thickness(0),
		center_brightness(0),
		plane_size(INFINITY),
		transform(Transform3D()),
		text_outline_color(Color(0, 0, 0, 1)),
		text_outline_size(12),
		text_fixed_size(false),
		text_font(nullptr),
		dcd({}),
		hd_sphere(false),
		custom_xform(false) {
	uint32_t hash = hash_murmur3_one_float(text_outline_color.r);
	hash = hash_murmur3_one_float(text_outline_color.g, hash);
	hash = hash_murmur3_one_float(text_outline_color.b, hash);
	text_outline_color_hash = hash_murmur3_one_float(text_outline_color.a, hash);
}

DebugDraw3DScopeConfig::Data::Data(const Data *p_parent) :
		thickness(p_parent->thickness),
		center_brightness(p_parent->center_brightness),
		plane_size(p_parent->plane_size),
		transform(p_parent->transform),
		text_outline_color(p_parent->text_outline_color),
		text_outline_color_hash(p_parent->text_outline_color_hash),
		text_outline_size(p_parent->text_outline_size),
		text_fixed_size(p_parent->text_fixed_size),
		text_font(p_parent->text_font),
		dcd(p_parent->dcd),
		hd_sphere(p_parent->hd_sphere),
		custom_xform(p_parent->custom_xform) {
}
