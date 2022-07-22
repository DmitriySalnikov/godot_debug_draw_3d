#include "render_instances.h"

bool DelayedRenderer::IsExpired() {
	return TIME_TO_MS(TIME_NOW() - updated_time).count() > ms_expiration && IsUsedOneTime;
}

void DelayedRenderer::Returned() {
	IsUsedOneTime = false;
	IsVisible = true;
}

void DelayedRendererLine::set_lines(std::vector<Vector3> lines) {
	_lines = lines;
	Bounds = CalculateBoundsBasedOnLines(_lines);
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