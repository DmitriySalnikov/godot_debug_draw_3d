#include "render_instances.h"

#ifndef DISABLE_DEBUG_RENDERING

#include "stats_3d.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/texture.hpp>
#include <godot_cpp/classes/time.hpp>
GODOT_WARNING_RESTORE()

// TODO too big buffer constantly shrinking
template <class T, size_t _TStepSize>
struct TempBigBuffer {
private:
	T m_buffer;
	int m_accesses_before_shrink = (int)InstanceType::ALL * 60; // try to shrink after 60 frames
	int m_shrink_timer;

public:
	TempBigBuffer() {
		m_buffer.resize(_TStepSize);
		m_shrink_timer = 0;
	}

	void prepare_buffer(size_t t_expected_size) {
		if ((size_t)m_buffer.size() < t_expected_size) {
			size_t new_size = (size_t)Math::ceil(t_expected_size / (double)_TStepSize) * _TStepSize;
			DEV_PRINT_STD(NAMEOF(TempBigBuffer) ": extending from %d to %d\n", m_buffer.size(), new_size);
			m_buffer.resize(new_size);
			m_shrink_timer = 0;
		} else {
			if (t_expected_size < m_buffer.size() - _TStepSize) {
				m_shrink_timer++;
				if (m_shrink_timer >= m_accesses_before_shrink) {
					size_t new_size = (size_t)m_buffer.size() - _TStepSize;
					DEV_PRINT_STD(NAMEOF(TempBigBuffer) ": shrinking from %d to %d\n", m_buffer.size(), new_size);
					m_buffer.resize(new_size);
					m_shrink_timer = 0;
				}
			} else {
				m_shrink_timer = 0;
			}
		}
	}

	inline auto ptrw() {
		return m_buffer.ptrw();
	}

	// TODO stupid and slow as it makes a COPY of all memory..
	inline auto slice(int64_t begin, int64_t end = 2147483647) {
		return m_buffer.slice(begin, end);
	}
};

DelayedRendererInstance::DelayedRendererInstance() :
		DelayedRenderer() {
	DEV_PRINT_STD("New " NAMEOF(DelayedRendererInstance) " created\n");
}

void DelayedRendererInstance::update(real_t _exp_time, const InstanceType &_type, const Transform3D &_transform, const Color &_col, const Color &_custom_col, const SphereBounds &_bounds) {
	_update(_exp_time, true);

	type = _type;
	bounds = _bounds;
	data = InstanceTransformAndData3D(_transform, _col, _custom_col);
}

DelayedRendererLine::DelayedRendererLine() :
		DelayedRenderer() {
	DEV_PRINT_STD("New " NAMEOF(DelayedRendererLine) " created\n");
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

const std::vector<Vector3> &DelayedRendererLine::get_lines() const {
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

void GeometryPool::fill_instance_data(const std::array<Ref<MultiMesh> *, (int)InstanceType::ALL> &t_meshes) {
	time_spent_to_fill_buffers_of_instances = TIME()->get_ticks_usec();

	for (size_t i = 0; i < t_meshes.size(); i++) {
		auto &mesh = *t_meshes[i];

		// TODO hot delete[]
		PackedFloat32Array a = get_raw_data((InstanceType)i);

		int new_size = (int)(a.size() / INSTANCE_DATA_FLOAT_COUNT);
		if (mesh->get_instance_count() != new_size) {
			mesh->set_instance_count(new_size);
			mesh->set_visible_instance_count(-1);
		}

		if (a.size()) {
			// TODO PR for native pointer!
			// TODO try to change condition to >= and not !=
			// ERROR:
			// Condition "p_buffer.size() != (multimesh->instances * (int)multimesh->stride_cache)" is true.at : RendererRD::MeshStorage::multimesh_set_buffer(servers\rendering\renderer_rd\storage_rd\mesh_storage.cpp : 1764)

			mesh->set_buffer(a);
		}
	}

	time_spent_to_fill_buffers_of_instances = TIME()->get_ticks_usec() - time_spent_to_fill_buffers_of_instances;
}

PackedFloat32Array GeometryPool::get_raw_data(InstanceType _type) {
	auto &inst = instances[(int)_type];

	size_t buffer_size = (inst.used_instant + inst.delayed.size()) * INSTANCE_DATA_FLOAT_COUNT;
	thread_local static auto temp_buffer = TempBigBuffer<PackedFloat32Array, 128 * 1024>();
	temp_buffer.prepare_buffer(buffer_size);

	size_t last_added = 0;
	if (buffer_size > 0) {
		auto w = temp_buffer.ptrw();

		auto write_data = [&last_added, &w](DelayedRendererInstance &o) {
			size_t id = last_added * INSTANCE_DATA_FLOAT_COUNT;

			last_added++;

			// 7500 instances. 1.2-1.3ms with the old approach and 0.8-0.9ms with the current approach
			memcpy(w + id, reinterpret_cast<real_t *>(&o.data), INSTANCE_DATA_FLOAT_COUNT * sizeof(real_t));

			/*w[id + 0] = o.transform.basis.rows[0][0];
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
			w[id + 15] = o.color[3];*/
		};

		for (size_t i = 0; i < inst.used_instant; i++) {
			auto &o = inst.instant[i];
			o.is_used_one_time = true;
			if (o.is_visible) {
				write_data(o);
			}
		}
		inst._prev_used_instant = inst.used_instant;

		inst.used_delayed = 0;
		for (size_t i = 0; i < inst.delayed.size(); i++) {
			auto &o = inst.delayed[i];
			if (!o.is_expired()) {
				inst.used_delayed++;
				o.is_used_one_time = true;
				if (o.is_visible) {
					write_data(o);
				}
			}
		}
	}

	return temp_buffer.slice(0, last_added * INSTANCE_DATA_FLOAT_COUNT);
}

void GeometryPool::fill_lines_data(Ref<ArrayMesh> _ig) {
	time_spent_to_fill_buffers_of_lines = 0;

	if (lines.used_instant == 0 && lines.delayed.size() == 0)
		return;

	size_t spent_timer = TIME()->get_ticks_usec();

	// Avoiding a large number of resizes increased the speed from 1.9-2.0ms to 1.4-1.5ms
	size_t used_vertices = 0;

	// pre calculate buffer size
	for (size_t i = 0; i < lines.used_instant; i++) {
		auto &o = lines.instant[i];
		if (o.is_visible) {
			used_vertices += o.get_lines().size();
		}
	}
	for (size_t i = 0; i < lines.delayed.size(); i++) {
		auto &o = lines.delayed[i];
		if (o.is_visible && !o.is_expired()) {
			used_vertices += o.get_lines().size();
		}
	}

	thread_local static auto verices_buffer = TempBigBuffer<PackedVector3Array, 64 * 1024>();
	thread_local static auto colors_buffer = TempBigBuffer<PackedColorArray, 64 * 1024>();
	verices_buffer.prepare_buffer(used_vertices);
	colors_buffer.prepare_buffer(used_vertices);

	size_t prev_pos = 0;
	auto vertices_write = verices_buffer.ptrw();
	auto colors_write = colors_buffer.ptrw();

	auto append_lines = [&](const DelayedRendererLine &o) {
		size_t lines_size = o.get_lines().size();

		std::copy(o.get_lines().begin(), o.get_lines().end(), vertices_write + prev_pos);
		std::fill(colors_write + prev_pos, colors_write + prev_pos + lines_size, o.color);

		prev_pos += lines_size;
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

	if (used_vertices > 1) {
		Array mesh = Array();
		mesh.resize(ArrayMesh::ArrayType::ARRAY_MAX);
		mesh[ArrayMesh::ArrayType::ARRAY_VERTEX] = verices_buffer.slice(0, used_vertices);
		mesh[ArrayMesh::ArrayType::ARRAY_COLOR] = colors_buffer.slice(0, used_vertices);

		_ig->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_LINES, mesh);
	}

	time_spent_to_fill_buffers_of_lines = TIME()->get_ticks_usec() - spent_timer;
}

void GeometryPool::reset_counter(double _delta) {
	lines.reset_counter(_delta);

	for (int i = 0; i < (int)InstanceType::ALL; i++) {
		instances[i].reset_counter(_delta, i);
	}
}

void GeometryPool::reset_visible_objects() {
	for (auto &i : instances) {
		i.reset_visible_counter();
	}
	lines.reset_visible_counter();
}

Ref<DebugDrawStats3D> GeometryPool::get_stats() const {
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

	Ref<DebugDrawStats3D> stats;
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

void GeometryPool::update_visibility(const std::vector<std::vector<Plane> > &t_frustums, const GeometryPoolDistanceCullingData &t_distance_data) {
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

	// loop over lines
	time_spent_to_cull_instant = TIME()->get_ticks_usec();

	for (size_t i = 0; i < lines.used_instant; i++)
		lines.instant[i].update_visibility(t_frustums, t_distance_data, true);

	time_spent_to_cull_instant = TIME()->get_ticks_usec() - time_spent_to_cull_instant + instant_time;
	time_spent_to_cull_delayed = TIME()->get_ticks_usec();

	for (size_t i = 0; i < lines.delayed.size(); i++)
		lines.delayed[i].update_visibility(t_frustums, t_distance_data, false);

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

void GeometryPool::add_or_update_instance(InstanceType _type, real_t _exp_time, const Transform3D &_transform, const Color &_col, const Color &_custom_col, const SphereBounds &_bounds, const std::function<void(DelayedRendererInstance *)> &_custom_upd) {
	DelayedRendererInstance *inst = instances[(int)_type].get(_exp_time > 0);
	inst->update(_exp_time, _type, _transform, _col, _custom_col, _bounds);
	if (_custom_upd)
		_custom_upd(inst);
}

void GeometryPool::add_or_update_line(real_t _exp_time, const std::vector<Vector3> &_lines, const Color &_col, const std::function<void(DelayedRendererLine *)> _custom_upd) {
	DelayedRendererLine *inst = lines.get(_exp_time > 0);
	inst->update(_exp_time, _lines, _col);
	if (_custom_upd)
		_custom_upd(inst);
}

#endif