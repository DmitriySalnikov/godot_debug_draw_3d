#include "render_instances.h"

void DelayedRenderer::_update(int exp_time, bool is_vis) {
	ms_expiration = exp_time;
	updated_time = TIME_NOW();
	IsUsedOneTime = false;
	IsVisible = is_vis;
}

bool DelayedRenderer::IsExpired() {
	return TIME_TO_MS(TIME_NOW() - updated_time).count() > ms_expiration || IsUsedOneTime;
}

void DelayedRenderer::Returned() {
	IsUsedOneTime = false;
	IsVisible = true;
}

DelayedRendererInstance::DelayedRendererInstance() :
		type(InstanceType::CUBES),
		transform(),
		color(),
		bounds() {
	DEBUG_PRINT("New " TEXT(DelayedRendererInstance) " created");
}

void DelayedRendererInstance::update(int _exp_time, bool _is_visible, InstanceType _type, Transform _transform, Color _col, SphereBounds _bounds) {
	_update(_exp_time, _is_visible);

	type = _type;
	transform = _transform;
	color = _col;
	bounds = _bounds;
}

DelayedRendererLine::DelayedRendererLine() {
	DEBUG_PRINT("New " TEXT(DelayedRendererLine) " created");
}

void DelayedRendererLine::update(int exp_time, bool is_visible, const std::vector<Vector3> &lines, Color col) {
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
