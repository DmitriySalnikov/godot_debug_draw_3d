#include "test_api_node.h"

#include "../src/utils/compiler.h"
#include "../src/utils/macro_utils.h"
#include "../src/utils/profiler.h"

#include "../../addons/debug_draw_3d/native_api/cpp/dd3d_cpp_api.hpp"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

void DD3DTestCppApiNode::_bind_methods() {
#define REG_CLASS_NAME DD3DTestCppApiNode
	REG_PROP_BOOL(one_million_boxes);
	REG_PROP_BOOL(many_camera_frustums);

	REG_PROP(draw_function, Variant::INT, PROPERTY_HINT_ENUM, "draw_box,draw_lines_c,draw_line_path_c,draw_points_c,draw_points_c_square,draw_point_path_c,draw_point_path_c_square,draw_arrow_path_c");

#undef REG_CLASS_NAME
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
	cfg->set_frustum_culling_mode(DebugDraw3DConfig::FRUSTUM_ROUGH);
	DebugDraw3D::set_config(cfg);
	auto cfg2 = DebugDraw3D::get_config();

	DEV_ASSERT(cfg2->get_frustum_culling_mode() == DebugDraw3DConfig::FRUSTUM_ROUGH);
	cfg->set_frustum_culling_mode(DebugDraw3DConfig::FRUSTUM_PRECISE);
	DEV_ASSERT(cfg2->is_visible_instance_bounds() == true);
	UtilityFunctions::print("New Visible bounds: ", cfg2->is_visible_instance_bounds());
	cfg2->set_visible_instance_bounds(false);
	DEV_ASSERT(cfg2->is_visible_instance_bounds() == false);
	UtilityFunctions::print("New Visible bounds: ", cfg2->is_visible_instance_bounds());

	auto cfg2D = DebugDraw2D::get_config();
	auto pos = cfg2D->get_text_block_position();
	cfg2D->set_text_block_position(pos);

	// Ref's
	{
		Ref<godot::Font> font1;
		{
			auto *ctrl = memnew(Control);
			add_child(ctrl);
			font1 = ctrl->get_theme_default_font();
			ctrl->queue_free();
		}
		auto font_id = font1->get_instance_id();
		Ref<godot::Font> font2(ObjectDB::get_instance(font_id));

		DEV_ASSERT(font1->get_instance_id() == font2->get_instance_id());
		DEV_ASSERT(font1.ptr() == font2.ptr());

		auto _fnt1 = DebugDraw3D::new_scoped_config()->set_text_font(font1);
		DEV_ASSERT(font1->get_instance_id() == _fnt1->get_text_font()->get_instance_id());
		DEV_ASSERT(_fnt1->get_text_font()->get_ascent());
	}

#ifdef DEV_ENABLED
	DEV_ASSERT(DebugDrawManager::DevTestEnum::FIRST_VALUE == 0);
	DEV_ASSERT(DebugDrawManager::DevTestEnum::SECOND_VALUE == 10);

	//DebugDrawManager::api_test6(nullptr, Variant(), false, 1, DebugDrawManager::SECOND_VALUE, 0.5f, "test", "test2", "test3");
#endif
}

float timer_cubes = 0;
void DD3DTestCppApiNode::_process(double p_delta) {
	FrameMark;
	ZoneScoped;

	DebugDraw3D::draw_sphere(Vector3(1, 1, 1), 2, Color(1, 1, 0), 0);

	PackedVector3Array a;
	a.append(Vector3(0, 0, 0));
	a.append(Vector3(1, 1, 1));
	DebugDraw3D::draw_lines_c(a.ptrw(), a.size());
	a[1] = Vector3(0, 1, 1);
	DebugDraw3D::draw_lines(a, Color(0, 1, 1));

	// 16ms GD
	// 2.5ms cpp
	// Lots of boxes to check performance..
	int x_size = 50;
	int y_size = 50;
	int z_size = 3;
	float mul = 1;
	float cubes_max_time = 1.25;
	auto cfg = DebugDraw3D::new_scoped_config();
	SelectedDrawFunction selected_func = (SelectedDrawFunction)draw_function;

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
		{
			uint64_t lines_buff_size = x_size * y_size * z_size;
			ADD_THREAD_LOCAL_BUFFER(lines_buff, godot::Vector3, selected_func == SelectedDrawFunction::draw_box ? 0 : lines_buff_size, 10000);

			ZoneScopedN("Draw cubes or Fill arrays");
			for (int x = 0; x < x_size; x++)
				for (int y = 0; y < y_size; y++)
					for (int z = 0; z < z_size; z++) {
						if (selected_func == SelectedDrawFunction::draw_box) {
							Vector3 size = Vector3(1, 1, 1);
							cfg->set_thickness((float)rng->randf_range(0, 0.1));
							// var size = Vector3(randf_range(0.1, 100), randf_range(0.1, 100), randf_range(0.1, 100))
							DebugDraw3D::draw_box(Vector3(x * mul, (-4 - z) * mul, y * mul), Quaternion(), size, Color(0, 0, 0, 0), false, cubes_max_time);
						} else {
							float x_off = (rand() - (RAND_MAX * 0.5f)) / RAND_MAX * 0.5f;
							float y_off = (rand() - (RAND_MAX * 0.5f)) / RAND_MAX * 0.5f;
							float z_off = (rand() - (RAND_MAX * 0.5f)) / RAND_MAX * 0.5f;
							lines_buff.get()[z * x_size * y_size + y * y_size + x] = Vector3(x * mul + x_off, (-4 - z) * mul + y_off, y * mul + z_off);
						}
					}

			switch (selected_func) {
				case SelectedDrawFunction::draw_lines_c: {
					DebugDraw3D::draw_lines_c(lines_buff.get(), lines_buff_size, Color(0, 0, 0, 0), cubes_max_time);
					break;
				}
				case SelectedDrawFunction::draw_line_path_c: {
					DebugDraw3D::draw_line_path_c(lines_buff.get(), lines_buff_size, Color(0, 0, 0, 0), cubes_max_time);
					break;
				}
				case SelectedDrawFunction::draw_points_c: {
					DebugDraw3D::draw_points_c(lines_buff.get(), lines_buff_size, DebugDraw3D::POINT_TYPE_SPHERE, 0.25f, Color(0, 0, 0, 0), cubes_max_time);
					break;
				}
				case SelectedDrawFunction::draw_points_c_square: {
					DebugDraw3D::draw_points_c(lines_buff.get(), lines_buff_size, DebugDraw3D::POINT_TYPE_SQUARE, 0.25f, Color(0, 0, 0, 0), cubes_max_time);
					break;
				}
				case SelectedDrawFunction::draw_point_path_c: {
					DebugDraw3D::draw_point_path_c(lines_buff.get(), lines_buff_size, DebugDraw3D::POINT_TYPE_SPHERE, 0.25f, Color(0, 0, 0, 0), Color(0, 0, 0, 0), cubes_max_time);
					break;
				}
				case SelectedDrawFunction::draw_point_path_c_square: {
					DebugDraw3D::draw_point_path_c(lines_buff.get(), lines_buff_size, DebugDraw3D::POINT_TYPE_SQUARE, 0.25f, Color(0, 0, 0, 0), Color(0, 0, 0, 0), cubes_max_time);
					break;
				}
				case SelectedDrawFunction::draw_arrow_path_c: {
					DebugDraw3D::draw_arrow_path_c(lines_buff.get(), lines_buff_size, Color(0, 0, 0, 0), 0.25f, true, cubes_max_time);
					break;
				}
			}
		}

		timer_cubes = cubes_max_time;

		DebugDraw2D::begin_text_group("Stats");
		DebugDraw2D::set_text("F", String::num_int64(draw_function), 0, { 0, 0, 0, 0 }, cubes_max_time);
		DebugDraw2D::set_text("Draw Grid C++", String("{0}ms").format(Array::make((Time::get_singleton()->get_ticks_usec() - _start_time) / 1000.0)), 1, { 0, 0, 0, 0 }, cubes_max_time);
		DebugDraw2D::set_text("Total Draw time C++", String("{0}ms").format(Array::make(DebugDraw3D::get_render_stats()->get_total_time_spent_usec() / 1000.0)), 2, { 0, 0, 0, 0 }, cubes_max_time);
	}

	timer_cubes -= p_delta;
	if (timer_cubes > cubes_max_time) {
		DebugDraw3D::clear_all();
		timer_cubes = 0;
	}

	{
		uint64_t _start_time = Time::get_singleton()->get_ticks_usec();

		auto *cam = get_viewport()->get_camera_3d();
		if (cam) {
			Basis cam_basis = cam->get_global_transform().basis;
			auto _s1 = DebugDraw3D::new_scoped_config()->set_thickness(0);

			int n = many_camera_frustums ? 1000 : 1;
			uint64_t t = many_camera_frustums ? Time::get_singleton()->get_ticks_msec() : 0;
			for (int i = 0; i < n; i++) {
				DebugDraw3D::draw_camera_frustum(cam, Color::from_hsv(Math::wrapf(i / float(n) + t * 0.001f, 0, 1), 0.5f, 0.9f));

				// small forward movement
				Transform3D xf = _s1->get_transform();
				xf.origin -= cam_basis.get_column(2) * 0.001f;
				_s1->set_transform(xf);
			}
		}

		if (many_camera_frustums) {
			DebugDraw2D::begin_text_group("Stats");
			DebugDraw2D::set_text("Draw Cameras C++", String("{0}ms").format(Array::make((Time::get_singleton()->get_ticks_usec() - _start_time) / 1000.0)), 10);
		}
	}
}
