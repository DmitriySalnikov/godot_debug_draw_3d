#include "test_api_node.h"

#include "../src/utils/compiler.h"
#include "../src/utils/profiler.h"

#include "../../addons/debug_draw_3d/native_api/cpp/dd3d_cpp_api.hpp"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

#define REG_CLASS_NAME DD3DTestCppApiNode
#define REG_PROP_BASE(name, getter, prop_info)                                                \
	ClassDB::bind_method(D_METHOD(NAMEOF(set_##name), "value"), &REG_CLASS_NAME::set_##name); \
	ClassDB::bind_method(D_METHOD(NAMEOF(getter##name)), &REG_CLASS_NAME::getter##name);      \
	ADD_PROPERTY(prop_info, NAMEOF(set_##name), NAMEOF(getter##name));
/// name, type, hint, hint_string, usage
#define REG_PROP(name, type, ...) REG_PROP_BASE(name, get_, PropertyInfo(type, #name, ##__VA_ARGS__))
/// name, type, hint, hint_string, usage
#define REG_PROP_BOOL(name, ...) REG_PROP_BASE(name, is_, PropertyInfo(Variant::BOOL, #name, ##__VA_ARGS__))

void DD3DTestCppApiNode::_bind_methods() {
	REG_PROP_BOOL(one_million_boxes);
}

DD3DTestCppApiNode::DD3DTestCppApiNode() {
}

void DD3DTestCppApiNode::_ready() {
	// Some Ref tests
	DebugDraw3DScopeConfig();

	auto cfg = DebugDraw3D::get_config();
	UtilityFunctions::print("Frustum scale: ", cfg->get_frustum_length_scale(), ", Visible bounds: ", cfg->is_visible_instance_bounds());

	cfg = std::make_shared<DebugDraw3DConfig>(false);
	DebugDraw3D::set_config(cfg);

	cfg = std::make_shared<DebugDraw3DConfig>();
	cfg->set_visible_instance_bounds(true);
	DebugDraw3D::set_config(cfg);
	auto cfg2 = DebugDraw3D::get_config();
	DEV_ASSERT(cfg2->is_visible_instance_bounds() == true);
	UtilityFunctions::print("New Visible bounds: ", cfg2->is_visible_instance_bounds());
	cfg2->set_visible_instance_bounds(false);
	DEV_ASSERT(cfg2->is_visible_instance_bounds() == false);
	UtilityFunctions::print("New Visible bounds: ", cfg2->is_visible_instance_bounds());

#ifdef DEV_ENABLED
	DebugDrawManager::api_test6(nullptr, Variant(), false, 1, DebugDrawManager::SECOND_VALUE, 0.5f, "test", "test2", "test3");
#endif
}

float timer_cubes = 0;
void DD3DTestCppApiNode::_process(double p_delta) {
	FrameMark;
	ZoneScoped;

	DebugDraw3D::draw_sphere(Vector3(1, 1, 1), 2, Color(1, 1, 0), 0);

	// 16ms GD
	// 2.5ms cpp
	// Lots of boxes to check performance..
	int x_size = 50;
	int y_size = 50;
	int z_size = 3;
	float mul = 1;
	float cubes_max_time = 1.25;
	auto cfg = DebugDraw3D::new_scoped_config();

	// 2060ms GD
	// 430ms cpp
	if (one_million_boxes) {
		x_size = 100;
		y_size = 100;
		z_size = 100;
		mul = 4;
		cubes_max_time = 60;
	}

	if (timer_cubes < 0) {
		uint64_t _start_time = Time::get_singleton()->get_ticks_usec();
		Ref<RandomNumberGenerator> rng;
		rng.instantiate();
		for (int x = 0; x < x_size; x++)
			for (int y = 0; y < y_size; y++)
				for (int z = 0; z < z_size; z++) {
					Vector3 size = Vector3(1, 1, 1);
					cfg->set_thickness((float)rng->randf_range(0, 0.1));
					// var size = Vector3(randf_range(0.1, 100), randf_range(0.1, 100), randf_range(0.1, 100))
					DebugDraw3D::draw_box(Vector3(x * mul, (-4 - z) * mul, y * mul), Quaternion(), size, Color(0, 0, 0, 0), false, cubes_max_time);
				}
		timer_cubes = cubes_max_time;

		UtilityFunctions::print("Draw Cubes C++: ", (Time::get_singleton()->get_ticks_usec() - _start_time) / 1000.0, "ms",
				"\nTotal Draw time C++: ", DebugDraw3D::get_render_stats()->get_total_time_spent_usec() / 1000.0, "ms");
	}

	timer_cubes -= p_delta;
	if (timer_cubes > cubes_max_time) {
		DebugDraw3D::clear_all();
		timer_cubes = 0;
	}
}
