#include "render_instances.h"

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/texture.hpp>

DelayedRendererInstance::DelayedRendererInstance() :
		DelayedRenderer() {
	DEBUG_PRINT_STD("New " TEXT(DelayedRendererInstance) " created\n");
}

void DelayedRendererInstance::update(real_t _exp_time, InstanceType _type, Transform3D _transform, Color _col, SphereBounds _bounds) {
	_update(_exp_time, true);

	type = _type;
	bounds = _bounds;
	transform = _transform;
	color = _col;
}

DelayedRendererLine::DelayedRendererLine() :
		DelayedRenderer() {
	DEBUG_PRINT_STD("New " TEXT(DelayedRendererLine) " created\n");
}

void DelayedRendererLine::update(real_t _exp_time, const std::vector<Vector3> &lines, Color col) {
	_update(_exp_time, true);

	set_lines(lines);
	color = col;
}

void DelayedRendererLine::set_lines(std::vector<Vector3> lines) {
	_lines = lines;
	bounds = calculate_bounds_based_on_lines(_lines);
}

std::vector<Vector3> DelayedRendererLine::get_lines() {
	return _lines;
}

AABB DelayedRendererLine::calculate_bounds_based_on_lines(std::vector<Vector3> &lines) {
	if (lines.size() > 0) {
		// Using the original Godot expand_to code to avoid creating new AABB instances
		Vector3 begin = lines[0];
		Vector3 end = lines[0] + Vector3_ZERO;

		for (Vector3 v : lines) {
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

PackedFloat32Array GeometryPool::get_raw_data(InstanceType type) {
	PackedFloat32Array res;
	res.resize((int)((instances[type].used_instant + instances[type].delayed.size()) * INSTANCE_DATA_FLOAT_COUNT));

	size_t last_added = 0;
	if (res.size() > 0) {
		auto w = res.ptrw();

		auto write_data = [&last_added, &w](DelayedRendererInstance &o) {
			int id = (int)(last_added * INSTANCE_DATA_FLOAT_COUNT);

			o.is_used_one_time = true;
			if (o.is_visible) {
				last_added++;

				// TODO need testing
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
			}
		};

		for (size_t i = 0; i < instances[type].used_instant; i++) {
			write_data(instances[type].instant[i]);
		}
		instances[type]._prev_used_instant = instances[type].used_instant;

		instances[type].used_delayed = 0;
		for (size_t i = 0; i < instances[type].delayed.size(); i++) {
			auto &o = instances[type].delayed[i];
			if (!o.is_expired()) {
				instances[type].used_delayed++;
				write_data(o);
			}
		}
	}
	res.resize((int)(last_added * INSTANCE_DATA_FLOAT_COUNT));

	return res;
}

void GeometryPool::fill_lines_data(Ref<ImmediateMesh> ig) {
	if (lines.used_instant == 0 && lines.delayed.size() == 0)
		return;

	ig->surface_begin(Mesh::PrimitiveType::PRIMITIVE_LINES);

	for (size_t i = 0; i < lines.used_instant; i++) {
		auto &o = lines.instant[i];
		if (o.is_visible) {
			ig->surface_set_color(o.color);
			for (auto &l : o.get_lines()) {
				ig->surface_add_vertex(l);
			};
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
				ig->surface_set_color(o.color);
				for (auto &l : o.get_lines()) {
					ig->surface_add_vertex(l);
				};
			}
		}
		o.is_used_one_time = true;
	}

	ig->surface_end();
}

void GeometryPool::reset_counter(real_t delta) {
	lines.reset_counter(delta);

	for (int i = 0; i < InstanceType::ALL; i++) {
		instances[i].reset_counter(delta, i);
	}
}

size_t GeometryPool::get_visible_instances() {
	int sum = 0;
	for (auto &i : instances) {
		sum += i.visible_objects;
	}
	return sum;
}

size_t GeometryPool::get_visible_lines() {
	return lines.visible_objects;
}

void GeometryPool::reset_visible_objects() {
	for (auto &i : instances) {
		i.reset_visible_counter();
	}
	lines.reset_visible_counter();
}

size_t GeometryPool::get_used_instances_instant(InstanceType type) {
	return instances[type]._prev_used_instant;
}

size_t GeometryPool::get_used_instances_delayed(InstanceType type) {
	return instances[type].used_delayed;
}

size_t GeometryPool::get_used_instances_total() {
	size_t sum = 0;
	for (auto &i : instances) {
		sum += i._prev_used_instant;
		sum += i.used_delayed;
	}
	return sum;
}

size_t GeometryPool::get_used_lines_total() {
	return lines._prev_used_instant + lines.used_delayed;
}

size_t GeometryPool::get_used_lines_instant() {
	return lines._prev_used_instant;
}

size_t GeometryPool::get_used_lines_delayed() {
	return lines.used_delayed;
}

void GeometryPool::clear_pool() {
	for (auto &i : instances) {
		i.clear_pools();
	}
	lines.clear_pools();
}

void GeometryPool::for_each_instance(std::function<void(DelayedRendererInstance *)> func) {
	for (auto &inst : instances) {
		for (size_t i = 0; i < inst.used_instant; i++) {
			func(&inst.instant[i]);
		}
		for (size_t i = 0; i < inst.delayed.size(); i++) {
			if (!inst.delayed[i].is_expired())
				func(&inst.delayed[i]);
		}
	}
}

void GeometryPool::for_each_line(std::function<void(DelayedRendererLine *)> func) {
	for (size_t i = 0; i < lines.used_instant; i++) {
		func(&lines.instant[i]);
	}
	for (size_t i = 0; i < lines.delayed.size(); i++) {
		if (!lines.delayed[i].is_expired())
			func(&lines.delayed[i]);
	}
}

void GeometryPool::update_visibility(std::vector<std::vector<Plane> > frustums) {
	for (auto &t : instances) {
		for (size_t i = 0; i < t.used_instant; i++)
			t.instant[i].update_visibility(frustums, true);

		for (size_t i = 0; i < t.delayed.size(); i++)
			t.delayed[i].update_visibility(frustums, false);
	}

	for (size_t i = 0; i < lines.used_instant; i++)
		lines.instant[i].update_visibility(frustums, true);

	for (size_t i = 0; i < lines.delayed.size(); i++)
		lines.delayed[i].update_visibility(frustums, false);
}

void GeometryPool::update_expiration(real_t delta) {
	for (auto &t : instances)
		for (size_t i = 0; i < t.delayed.size(); i++)
			t.delayed[i].update_expiration(delta);

	for (size_t i = 0; i < lines.delayed.size(); i++)
		lines.delayed[i].update_expiration(delta);
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

void GeometryPool::add_or_update_instance(InstanceType _type, real_t _exp_time, Transform3D _transform, Color _col, SphereBounds _bounds, std::function<void(DelayedRendererInstance *)> custom_upd) {
	DelayedRendererInstance *inst = instances[_type].get(_exp_time > 0);
	inst->update(_exp_time, _type, _transform, _col, _bounds);
	if (custom_upd)
		custom_upd(inst);
}

void GeometryPool::add_or_update_line(real_t _exp_time, std::vector<Vector3> _lines, Color _col, std::function<void(DelayedRendererLine *)> custom_upd) {
	DelayedRendererLine *inst = lines.get(_exp_time > 0);
	inst->update(_exp_time, _lines, _col);
	if (custom_upd)
		custom_upd(inst);
}
