#pragma once

#include "utils/compiler.h"

#include <functional>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DDScopedConfig3D : public RefCounted {
	GDCLASS(DDScopedConfig3D, RefCounted)

protected:
	static void _bind_methods();
	uint64_t thread_id;
	uint64_t guard_id;

	typedef std::function<void(uint64_t, uint64_t)> unregister_func;
	unregister_func unregister_action;

	// Update constructor!
	real_t thickness;
	real_t center_brightness;

public:
	void _manual_unregister();

	Ref<DDScopedConfig3D> set_thickness(real_t value);
	real_t get_thickness();

	Ref<DDScopedConfig3D> set_center_brightness(real_t value);
	real_t get_center_brightness();

	DDScopedConfig3D();

	// `DDScopedConfig3D` is passed as Ref to avoid a random unreference
	DDScopedConfig3D(const uint64_t &p_thread_id, const uint64_t &p_guard_id, const DDScopedConfig3D *parent, const unregister_func p_unreg);
	~DDScopedConfig3D();
};
