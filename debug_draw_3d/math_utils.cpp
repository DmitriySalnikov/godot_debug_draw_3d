#include "math_utils.h"
#include "utils.h"

void MathUtils::get_diagonal_vectors(Vector3 a, Vector3 b, Vector3 &bottom, Vector3 &top, Vector3 &diag) {
	bottom = Vector3::ZERO;
	top = Vector3::ZERO;

	if (a.x > b.x) {
		top.x = a.x;
		bottom.x = b.x;
	} else {
		top.x = b.x;
		bottom.x = a.x;
	}

	if (a.y > b.y) {
		top.y = a.y;
		bottom.y = b.y;
	} else {
		top.y = b.y;
		bottom.y = a.y;
	}

	if (a.z > b.z) {
		top.z = a.z;
		bottom.z = b.z;
	} else {
		top.z = b.z;
		bottom.z = a.z;
	}

	diag = top - bottom;
}

bool MathUtils::is_bounds_partially_inside_convex_shape(AABB bounds, std::vector<Plane> &planes) {
	Vector3 extent = bounds.size * 0.5f;
	Vector3 center = bounds.position + extent;

	for (Plane p : planes)
		if (Vector3(center.x - extent.x * Math::sign(p.normal.x),
					center.y - extent.y * Math::sign(p.normal.y),
					center.z - extent.z * Math::sign(p.normal.z))
						.dot(p.normal) > p.d)
			return false;

	return true;
}

bool MathUtils::is_bounds_partially_inside_convex_shape(AABB bounds, Array &planes) {
	Vector3 extent = bounds.size * 0.5f;
	Vector3 center = bounds.position + extent;

	for (int i = 0; i < planes.size(); i++) {
		Plane p = planes[i];
		if (Vector3(center.x - extent.x * Math::sign(p.normal.x),
					center.y - extent.y * Math::sign(p.normal.y),
					center.z - extent.z * Math::sign(p.normal.z))
						.dot(p.normal) > p.d)
			return false;
	}

	return true;
}

bool MathUtils::is_bounds_partially_inside_convex_shape(SphereBounds sphere, std::vector<Plane> planes) {
	for (Plane p : planes)
		if (p.distance_to(sphere.Position) >= sphere.Radius)
			return false;

	return true;
}

bool MathUtils::is_bounds_partially_inside_convex_shape(SphereBounds sphere, Array planes) {
	for (int i = 0; i < planes.size(); i++) {
		Plane p = planes[i];
		if (p.distance_to(sphere.Position) >= sphere.Radius)
			return false;
	}

	return true;
}

float MathUtils::get_max_value(Vector3 &value) {
	return Math::max(abs(value.x), Math::max(abs(value.y), abs(value.z)));
}