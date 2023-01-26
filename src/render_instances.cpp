#include "render_instances.h"
#include "draw_stats.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#endif

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/texture.hpp>
#include <godot_cpp/classes/time.hpp>

#if defined(_MSC_VER)
#pragma warning(default : 4244)
#endif

DelayedRendererInstance::DelayedRendererInstance() :
		DelayedRenderer() {
	DEV_PRINT_STD("New " TEXT(DelayedRendererInstance) " created\n");
}

void DelayedRendererInstance::update(real_t _exp_time, const InstanceType &_type, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds) {
	_update(_exp_time, true);

	type = _type;
	bounds = _bounds;
	transform = _transform;
	color = _col;
}

DelayedRendererLine::DelayedRendererLine() :
		DelayedRenderer() {
	DEV_PRINT_STD("New " TEXT(DelayedRendererLine) " created\n");
}

void DelayedRendererLine::update(real_t _exp_time, const std::vector<Vector3> &_lines, const Color &_col) {
	_update(_exp_time, true);

	set_lines(_lines);
	color = _col;
}

void DelayedRendererLine::set_lines(const std::vector<Vector3> &_lines) {
	lines = _lines;
	bounds = calculate_bounds_based_on_lines(lines);
}

std::vector<Vector3> &DelayedRendererLine::get_lines() {
	return lines;
}

AABB DelayedRendererLine::calculate_bounds_based_on_lines(const std::vector<Vector3> &_lines) {
	if (_lines.size() > 0) {
		// Using the original Godot expand_to code to avoid creating new AABB instances
		Vector3 begin = _lines[0];
		Vector3 end = _lines[0] + Vector3_ZERO;

		for (Vector3 v : _lines) {
			if (v.x < begin.x) {
				begin.x = v.x;
			}
			if (v.y < begin.y) {
				begin.y = v.y;
			}
			if (v.z < begin.z) {
				begin.z = v.z;
			}

			if (v.x > end.x) {
				end.x = v.x;
			}
			if (v.y > end.y) {
				end.y = v.y;
			}
			if (v.z > end.z) {
				end.z = v.z;
			}
		}

		return AABB(begin, end - begin);
	} else {
		return AABB();
	}
}

void GeometryPool::fill_instance_data(const std::array<Ref<MultiMesh> *, InstanceType::ALL> &t_meshes) {
	time_spent_to_fill_buffers_of_instances = TIME()->get_ticks_usec();

	// TODO: need static buffer to avoid constant vector's creation. 450kb is needed for 7500 instances of all types at the same time..
	// 512kb as starting point will be good
	// mb need to add a timer here to reduce this buffer after a while
	for (size_t i = 0; i < t_meshes.size(); i++) {
		auto &mesh = *t_meshes[i];
		mesh->set_visible_instance_count(-1);

		auto a = get_raw_data((InstanceType)i);
		mesh->set_instance_count((int)(a.size() / INSTANCE_DATA_FLOAT_COUNT));

		if (a.size()) {
			mesh->set_buffer(a);
		}
	}

	time_spent_to_fill_buffers_of_instances = TIME()->get_ticks_usec() - time_spent_to_fill_buffers_of_instances;
}

PackedFloat32Array GeometryPool::get_raw_data(InstanceType _type) {
	PackedFloat32Array res;
	res.resize((instances[_type].used_instant + instances[_type].delayed.size()) * INSTANCE_DATA_FLOAT_COUNT);

	size_t last_added = 0;
	if (res.size() > 0) {
		auto w = res.ptrw();

		auto write_data = [&last_added, &w](DelayedRendererInstance &o) {
			int id = (int)(last_added * INSTANCE_DATA_FLOAT_COUNT);

			o.is_used_one_time = true;
			if (o.is_visible) {
				last_added++;

				w[id + 0] = o.transform.basis.rows[0][0];
				w[id + 1] = o.transform.basis.rows[0][1];
				w[id + 2] = o.transform.basis.rows[0][2];
				w[id + 3] = o.transform.origin.x;
				w[id + 4] = o.transform.basis.rows[1][0];
				w[id + 5] = o.transform.basis.rows[1][1];
				w[id + 6] = o.transform.basis.rows[1][2];
				w[id + 7] = o.transform.origin.y;
				w[id + 8] = o.transform.basis.rows[2][0];
				w[id + 9] = o.transform.basis.rows[2][1];
				w[id + 10] = o.transform.basis.rows[2][2];
				w[id + 11] = o.transform.origin.z;
				w[id + 12] = o.color[0];
				w[id + 13] = o.color[1];
				w[id + 14] = o.color[2];
				w[id + 15] = o.color[3];
				// TODO: mb use custom data to implement volumetric shapes...
			}
		};

		for (size_t i = 0; i < instances[_type].used_instant; i++) {
			write_data(instances[_type].instant[i]);
		}
		instances[_type]._prev_used_instant = instances[_type].used_instant;

		instances[_type].used_delayed = 0;
		for (size_t i = 0; i < instances[_type].delayed.size(); i++) {
			auto &o = instances[_type].delayed[i];
			if (!o.is_expired()) {
				instances[_type].used_delayed++;
				write_data(o);
			}
		}
	}
	res.resize(last_added * INSTANCE_DATA_FLOAT_COUNT);

	return res;
}

void GeometryPool::fill_lines_data(Ref<ArrayMesh> _ig) {
	time_spent_to_fill_buffers_of_lines = 0;

	if (lines.used_instant == 0 && lines.delayed.size() == 0)
		return;

	time_spent_to_fill_buffers_of_lines = TIME()->get_ticks_usec();

	PackedVector3Array verticies;
	PackedColorArray colors;

	auto append_lines = [&](DelayedRendererLine &o) {
		size_t lines_size = o.get_lines().size();

		PackedColorArray cols;
		cols.resize(lines_size);
		cols.fill(o.color);
		colors.append_array(cols);

		size_t p_size = verticies.size();
		verticies.resize(verticies.size() + lines_size);
		std::copy(o.get_lines().begin(), o.get_lines().end(), verticies.ptrw() + p_size);
	};

	for (size_t i = 0; i < lines.used_instant; i++) {
		auto &o = lines.instant[i];
		if (o.is_visible) {
			append_lines(o);
		}
		o.is_used_one_time = true;
	}
	lines._prev_used_instant = lines.used_instant;

	lines.used_delayed = 0;
	for (size_t i = 0; i < lines.delayed.size(); i++) {
		auto &o = lines.delayed[i];
		if (!o.is_expired()) {
			lines.used_delayed++;
			if (o.is_visible) {
				append_lines(o);
			}
		}
		o.is_used_one_time = true;
	}

	if (verticies.size() > 1) {
		Array mesh = Array();
		mesh.resize(ArrayMesh::ArrayType::ARRAY_MAX);
		mesh[ArrayMesh::ArrayType::ARRAY_VERTEX] = verticies;
		mesh[ArrayMesh::ArrayType::ARRAY_COLOR] = colors;

		_ig->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_LINES, mesh);
	}

	time_spent_to_fill_buffers_of_lines = TIME()->get_ticks_usec() - time_spent_to_fill_buffers_of_lines;
}

void GeometryPool::reset_counter(double _delta) {
	lines.reset_counter(_delta);

	for (int i = 0; i < InstanceType::ALL; i++) {
		instances[i].reset_counter(_delta, i);
	}
}

void GeometryPool::reset_visible_objects() {
	for (auto &i : instances) {
		i.reset_visible_counter();
	}
	lines.reset_visible_counter();
}

Ref<DebugDrawStats> GeometryPool::get_stats() const {
	size_t used_instances = 0;
	for (auto &i : instances) {
		used_instances += i._prev_used_instant;
		used_instances += i.used_delayed;
	}

	size_t visible_instances = 0;
	for (auto &i : instances) {
		visible_instances += i.visible_objects;
	}

	size_t used_lines = lines._prev_used_instant + lines.used_delayed;

	Ref<DebugDrawStats> stats;
	stats.instantiate();
	stats->setup(
			/* t_instances */ used_instances,
			/* t_lines */ used_lines,

			/* t_visible_instances */ visible_instances,
			/* t_visible_lines */ lines.visible_objects,

			/* t_time_filling_buffers_instances_usec */ time_spent_to_fill_buffers_of_instances,
			/* t_time_filling_buffers_lines_usec */ time_spent_to_fill_buffers_of_lines,

			/* t_time_culling_instant_usec */ time_spent_to_cull_instant,
			/* t_time_culling_delayed_usec */ time_spent_to_cull_delayed);
	return stats;
}

void GeometryPool::clear_pool() {
	for (auto &i : instances) {
		i.clear_pools();
	}
	lines.clear_pools();
}

void GeometryPool::for_each_instance(const std::function<void(DelayedRendererInstance *)> &_func) {
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

void GeometryPool::for_each_line(const std::function<void(DelayedRendererLine *)> &_func) {
	for (size_t i = 0; i < lines.used_instant; i++) {
		_func(&lines.instant[i]);
	}
	for (size_t i = 0; i < lines.delayed.size(); i++) {
		if (!lines.delayed[i].is_expired())
			_func(&lines.delayed[i]);
	}
}

void GeometryPool::update_visibility(const std::vector<std::vector<Plane> > &_frustums, const GeometryPoolDistanceCullingData &t_distance_data) {
	uint64_t instant_time = 0;
	uint64_t delayed_time = 0;
	for (auto &t : instances) { // loop over instance types
		instant_time = TIME()->get_ticks_usec();

		for (size_t i = 0; i < t.used_instant; i++)
			t.instant[i].update_visibility(_frustums, t_distance_data, true);

		instant_time = TIME()->get_ticks_usec() - instant_time;
		delayed_time = TIME()->get_ticks_usec();

		for (size_t i = 0; i < t.delayed.size(); i++)
			t.delayed[i].update_visibility(_frustums, t_distance_data, false);

		delayed_time = TIME()->get_ticks_usec() - delayed_time;
	}

	// loop over lines
	time_spent_to_cull_instant = TIME()->get_ticks_usec();

	for (size_t i = 0; i < lines.used_instant; i++)
		lines.instant[i].update_visibility(_frustums, t_distance_data, true);

	time_spent_to_cull_instant = TIME()->get_ticks_usec() - time_spent_to_cull_instant + instant_time;
	time_spent_to_cull_delayed = TIME()->get_ticks_usec();

	for (size_t i = 0; i < lines.delayed.size(); i++)
		lines.delayed[i].update_visibility(_frustums, t_distance_data, false);

	time_spent_to_cull_delayed = TIME()->get_ticks_usec() - time_spent_to_cull_delayed + delayed_time;
}

void GeometryPool::update_expiration(double _delta) {
	for (auto &t : instances)
		for (size_t i = 0; i < t.delayed.size(); i++)
			t.delayed[i].update_expiration(_delta);

	for (size_t i = 0; i < lines.delayed.size(); i++)
		lines.delayed[i].update_expiration(_delta);
}

void GeometryPool::scan_visible_instances() {
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

	for (size_t i = 0; i < lines.used_instant; i++)
		if (lines.instant[i].is_visible)
			lines.visible_objects++;

	for (size_t i = 0; i < lines.delayed.size(); i++) {
		auto &o = lines.delayed[i];
		if (o.is_visible && !o.is_expired())
			lines.visible_objects++;
	}
}

void GeometryPool::add_or_update_instance(InstanceType _type, real_t _exp_time, const Transform3D &_transform, const Color &_col, const SphereBounds &_bounds, const std::function<void(DelayedRendererInstance *)> &_custom_upd) {
	DelayedRendererInstance *inst = instances[_type].get(_exp_time > 0);
	inst->update(_exp_time, _type, _transform, _col, _bounds);
	if (_custom_upd)
		_custom_upd(inst);
}

void GeometryPool::add_or_update_line(real_t _exp_time, const std::vector<Vector3> &_lines, const Color &_col, const std::function<void(DelayedRendererLine *)> _custom_upd) {
	DelayedRendererLine *inst = lines.get(_exp_time > 0);
	inst->update(_exp_time, _lines, _col);
	if (_custom_upd)
		_custom_upd(inst);
}
