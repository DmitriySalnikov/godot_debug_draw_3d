#include "render_instances.h"

#include <Mesh.hpp>
#include <MultiMesh.hpp>
#include <Texture.hpp>

DelayedRenderer::DelayedRenderer() :
		expiration_time(0),
		IsUsedOneTime(true),
		IsDelayedForOneFrame(false),
		IsVisible(false) {
}

void DelayedRenderer::_update(real_t exp_time, bool is_vis) {
	expiration_time = exp_time;
	IsUsedOneTime = false;
	IsDelayedForOneFrame = true;
	IsVisible = is_vis;
}

bool DelayedRenderer::IsExpired() const {
	if (expiration_time > 0) {
		return false;
	} else
		return IsUsedOneTime;
}

DelayedRendererInstance::DelayedRendererInstance() :
		type(InstanceType::CUBES),
		bounds(),
		DelayedRenderer() {
	DEBUG_PRINT_STD(L"New " TEXT(DelayedRendererInstance) " created\n");
}

void DelayedRendererInstance::update(real_t _exp_time, bool _is_visible, InstanceType _type, Transform _transform, Color _col, SphereBounds _bounds) {
	_update(_exp_time, _is_visible);

	type = _type;
	bounds = _bounds;
	transform = _transform;
	color = _col;
}

DelayedRendererLine::DelayedRendererLine() {
	DEBUG_PRINT_STD(L"New %s created\n", TEXTL(DelayedRendererLine));
}

void DelayedRendererLine::update(real_t exp_time, bool is_visible, const std::vector<Vector3> &lines, Color col) {
	_update(exp_time, is_visible);

	set_lines(lines);
	LinesColor = col;
}

void DelayedRendererLine::set_lines(std::vector<Vector3> lines) {
	_lines = lines;
	bounds = CalculateBoundsBasedOnLines(_lines);
}

std::vector<Vector3> DelayedRendererLine::get_lines() {
	return _lines;
}

AABB DelayedRendererLine::CalculateBoundsBasedOnLines(std::vector<Vector3> &lines) {
	if (lines.size() > 0) {
		// Using the original Godot expand_to code to avoid creating new AABB instances
		Vector3 begin = lines[0];
		Vector3 end = lines[0] + Vector3::ZERO;

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

PoolRealArray GeometryPool::get_raw_data(InstanceType type) {
	PoolRealArray res;
	res.resize((int)((instances[type].used_instant + instances[type].delayed.size()) * INSTANCE_DATA_FLOAT_COUNT));

	size_t last_added = 0;
	{
		auto w = res.write();

		auto write_data = [&last_added, &w](DelayedRendererInstance &o) {
			int id = (int)(last_added * INSTANCE_DATA_FLOAT_COUNT);

			o.IsUsedOneTime = true;
			if (o.IsVisible) {
				last_added++;
				w[id + 0] = o.transform.basis[0][0];
				w[id + 1] = o.transform.basis[1][0];
				w[id + 2] = o.transform.basis[2][0];
				w[id + 3] = o.transform.origin.x;
				w[id + 4] = o.transform.basis[0][1];
				w[id + 5] = o.transform.basis[1][1];
				w[id + 6] = o.transform.basis[2][1];
				w[id + 7] = o.transform.origin.y;
				w[id + 8] = o.transform.basis[0][2];
				w[id + 9] = o.transform.basis[1][2];
				w[id + 10] = o.transform.basis[2][2];
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

		instances[type].used_delayed = 0;
		for (size_t i = 0; i < instances[type].delayed.size(); i++) {
			auto &o = instances[type].delayed[i];
			if (!o.IsExpired()) {
				instances[type].used_delayed++;
				write_data(o);
			} else {
				if (o.IsDelayedForOneFrame) {
					o.IsDelayedForOneFrame = false;
					instances[type].used_delayed++;
					write_data(o);
				}
			}
		}
	}
	res.resize((int)(last_added * INSTANCE_DATA_FLOAT_COUNT));

	return res;
}

void GeometryPool::fill_lines_data(ImmediateGeometry *ig) {
	ig->begin(Mesh::PrimitiveType::PRIMITIVE_LINES);

	for (size_t i = 0; i < lines.used_instant; i++) {
		auto &o = lines.instant[i];
		if (o.IsVisible) {
			ig->set_color(o.LinesColor);
			for (auto &l : o.get_lines()) {
				ig->add_vertex(l);
			};
		}
		o.IsUsedOneTime = true;
	}

	lines.used_delayed = 0;
	for (size_t i = 0; i < lines.delayed.size(); i++) {
		auto &o = lines.delayed[i];
		ig->set_color(o.LinesColor);
		if (!o.IsExpired()) {
			lines.used_delayed++;
			if (o.IsVisible) {
				for (auto &l : o.get_lines()) {
					ig->add_vertex(l);
				};
			}
		} else {
			if (o.IsDelayedForOneFrame) {
				o.IsDelayedForOneFrame = false;
				lines.used_delayed++;
				if (o.IsVisible) {
					for (auto &l : o.get_lines()) {
						ig->add_vertex(l);
					};
				}
			}
		}
		o.IsUsedOneTime = true;
	}

	ig->end();
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
	return instances[type].used_instant;
}

size_t GeometryPool::get_used_instances_delayed(InstanceType type) {
	return instances[type].used_delayed;
}

size_t GeometryPool::get_used_instances_total() {
	size_t sum = 0;
	for (auto &i : instances) {
		sum += i.used_instant;
		sum += i.used_delayed;
	}
	return sum;
}

size_t GeometryPool::get_used_lines_total() {
	return lines.used_instant + lines.used_delayed;
}

size_t GeometryPool::get_used_lines_instant() {
	return lines.used_instant;
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
			if (!inst.delayed[i].IsExpired())
				func(&inst.delayed[i]);
		}
	}
}

void GeometryPool::for_each_line(std::function<void(DelayedRendererLine *)> func) {
	for (size_t i = 0; i < lines.used_instant; i++) {
		func(&lines.instant[i]);
	}
	for (size_t i = 0; i < lines.delayed.size(); i++) {
		if (!lines.delayed[i].IsExpired())
			func(&lines.delayed[i]);
	}
}

void GeometryPool::update_visibility(std::vector<Plane> frustum, real_t delta) {
	reset_visible_objects();

	for (auto &t : instances) {
		for (size_t i = 0; i < t.used_instant; i++) {
			auto &o = t.instant[i];
			if (o.IsVisible = MathUtils::is_bounds_partially_inside_convex_shape(o.bounds, frustum))
				t.visible_objects++;
		}
		for (size_t i = 0; i < t.delayed.size(); i++) {
			auto &o = t.delayed[i];
			if (!o.IsExpired()) {
				o.expiration_time -= delta;
				if (o.IsVisible = MathUtils::is_bounds_partially_inside_convex_shape(o.bounds, frustum))
					t.visible_objects++;
			}
		}
	}

	for (size_t i = 0; i < lines.used_instant; i++) {
		auto &o = lines.instant[i];
		if (o.IsVisible = MathUtils::is_bounds_partially_inside_convex_shape(o.bounds, frustum))
			lines.visible_objects++;
	}
	for (size_t i = 0; i < lines.delayed.size(); i++) {
		auto &o = lines.delayed[i];
		if (!o.IsExpired()) {
			o.expiration_time -= delta;
			if (o.IsVisible = MathUtils::is_bounds_partially_inside_convex_shape(o.bounds, frustum))
				lines.visible_objects++;
		}
	}
}

void GeometryPool::rescan_visible_instances() {
	reset_visible_objects();

	for (auto &t : instances) {
		for (size_t i = 0; i < t.used_instant; i++) {
			auto &o = t.instant[i];
			if (o.IsVisible)
				t.visible_objects++;
		}
		for (size_t i = 0; i < t.delayed.size(); i++) {
			auto &o = t.delayed[i];
			if (o.IsVisible && !o.IsExpired())
				t.visible_objects++;
		}
	}

	for (size_t i = 0; i < lines.used_instant; i++) {
		auto &o = lines.instant[i];
		if (o.IsVisible)
			lines.visible_objects++;
	}
	for (size_t i = 0; i < lines.delayed.size(); i++) {
		auto &o = lines.delayed[i];
		if (o.IsVisible && !o.IsExpired())
			lines.visible_objects++;
	}
}

void GeometryPool::add_or_update_instance(InstanceType _type, real_t _exp_time, bool _is_visible, Transform _transform, Color _col, SphereBounds _bounds, std::function<void(DelayedRendererInstance *)> custom_upd) {
	DelayedRendererInstance *inst = instances[_type].get(_exp_time > 0);
	inst->update(_exp_time, _is_visible, _type, _transform, _col, _bounds);
	if (custom_upd)
		custom_upd(inst);
}

void GeometryPool::add_or_update_line(real_t _exp_time, bool _is_visible, std::vector<Vector3> _lines, Color _col, std::function<void(DelayedRendererLine *)> custom_upd) {
	DelayedRendererLine *inst = lines.get(_exp_time > 0);
	inst->update(_exp_time, _is_visible, _lines, _col);
	if (custom_upd)
		custom_upd(inst);
}
