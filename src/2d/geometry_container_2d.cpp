#include "geometry_container_2d.h"

#ifndef DISABLE_DEBUG_RENDERING
#include "config_2d.h"
#include "debug_draw_2d.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/world3d.hpp>
GODOT_WARNING_RESTORE()

#include <array>

using namespace godot;

void GeometryPool2D::reset_counter(double _delta) {
	for (int i = 0; i < (int)InstanceType2D::ALL; i++) {
		instances[i].reset_counter(_delta, i);
	}
}

void GeometryPool2D::reset_visible_objects() {
	for (auto &i : instances) {
		i.reset_visible_counter();
	}
}

Ref<DebugDrawStats2D> GeometryPool2D::get_stats() const {
	size_t used_instances = 0;
	for (auto &i : instances) {
		used_instances += i._prev_used_instant;
		used_instances += i.used_delayed;
	}

	size_t visible_instances = 0;
	for (auto &i : instances) {
		visible_instances += i.visible_objects;
	}

	Ref<DebugDrawStats2D> stats;
	stats.instantiate();
	stats->setup(
			/* t_instances */ used_instances,
			/* t_lines */ 0,

			/* t_visible_instances */ visible_instances,
			/* t_visible_lines */ 0,

			/* t_time_filling_buffers_instances_usec */ time_spent_to_fill_buffers_of_instances,
			/* t_time_filling_buffers_lines_usec */ time_spent_to_fill_buffers_of_lines,

			/* t_time_culling_instant_usec */ time_spent_to_cull_instant,
			/* t_time_culling_delayed_usec */ time_spent_to_cull_delayed);
	return stats;
}

void GeometryPool2D::clear_pool() {
	for (auto &i : instances) {
		i.clear_pools();
	}
}

void GeometryPool2D::for_each_instance(const std::function<void(DelayedRendererInstance2D *)> &_func) {
	for (auto &inst : instances) {
		for (size_t i = 0; i < inst.used_instant; i++) {
			_func(&inst.instant[i]);
		}
		for (size_t i = 0; i < inst.delayed.size(); i++) {
			if (!inst.delayed[i].is_expired())
				_func(&inst.delayed[i]);
		}
	}
}

void GeometryPool2D::update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData2D &t_distance_data) {
	uint64_t instant_time = 0;
	uint64_t delayed_time = 0;
	for (auto &t : instances) { // loop over instance types
		uint64_t i_t = TIME()->get_ticks_usec();

		for (size_t i = 0; i < t.used_instant; i++)
			t.instant[i].update_visibility(t_frustums, t_distance_data, true);

		instant_time += TIME()->get_ticks_usec() - i_t;
		uint64_t d_t = TIME()->get_ticks_usec();

		for (size_t i = 0; i < t.delayed.size(); i++)
			t.delayed[i].update_visibility(t_frustums, t_distance_data, false);

		delayed_time += TIME()->get_ticks_usec() - d_t;
	}
}

void GeometryPool2D::update_expiration(double _delta) {
	for (auto &t : instances)
		for (size_t i = 0; i < t.delayed.size(); i++)
			t.delayed[i].update_expiration(_delta);
}

void GeometryPool2D::scan_visible_instances() {
	reset_visible_objects();

	for (auto &t : instances) {
		for (size_t i = 0; i < t.used_instant; i++)
			if (t.instant[i].is_visible)
				t.visible_objects++;
		for (size_t i = 0; i < t.delayed.size(); i++) {
			auto &o = t.delayed[i];
			if (o.is_visible && !o.is_expired())
				t.visible_objects++;
		}
	}
}

/////////////////////////////////////////////

GeometryContainer2D::GeometryContainer2D(class DebugDraw2D *root) {
	owner = root;

	// Create node with material and MultiMesh. Add to tree. Create array of instances
	set_render_layer_mask(1);
}

GeometryContainer2D::~GeometryContainer2D() {
	LOCK_GUARD(datalock);

	geometry_pool.clear_pool();
	geometry_pool.clear_pool();
}

void GeometryContainer2D::update_geometry(double delta) {
	LOCK_GUARD(datalock);

	// Don't clear geometry if frozen
	// TODO 2d
	// if (owner->get_config()->is_freeze_3d_render())
	//	return;

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		geometry_pool.reset_counter(delta);
		geometry_pool.reset_visible_objects();
		return;
	}

	// TODO: try to get all active cameras inside scene to properly calculate visibilty for SubViewports

	// Get camera frustum
	Camera3D *vp_cam = owner->get_root_node()->get_viewport()->get_camera_3d();
	if (IS_EDITOR_HINT()) {
		auto s_root = SCENE_TREE()->get_edited_scene_root();
		if (s_root) {
			vp_cam = s_root->get_viewport()->get_camera_3d();
		}
	}

	// Collect frustums and camera positions

	// Update visibility
	// geometry_pool.update_visibility(
	//		frustum_planes,
	//		GeometryPoolDistanceCullingData(owner->get_config()->get_cull_by_distance(), cameras_positions));

	// Draw immediate lines

	geometry_pool.scan_visible_instances();
	geometry_pool.update_expiration(delta);
	geometry_pool.reset_counter(delta);
}

Ref<DebugDrawStats2D> GeometryContainer2D::get_render_stats() {
	LOCK_GUARD(datalock);
	return geometry_pool.get_stats();
}

void GeometryContainer2D::set_render_layer_mask(int32_t layers) {
	LOCK_GUARD(datalock);

	// rs->instance_set_layer_mask(immediate_mesh_storage.instance, layers);
	render_layers = layers;
}

int32_t GeometryContainer2D::get_render_layer_mask() const {
	return render_layers;
}

void GeometryContainer2D::clear_2d_objects() {
	geometry_pool.clear_pool();
}

#endif