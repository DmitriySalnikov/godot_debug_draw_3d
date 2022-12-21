#include "math_utils.h"

const float MathUtils::CubeRadiusForSphere = 0.8660253882f; // "%.10f" % (Vector3.ONE * 0.5).length()
const float MathUtils::CylinderRadiusForSphere = 0.5590170026f; // "%.10f" % (Vector3(1,0.5,0) * 0.5).length()
const float MathUtils::AxisRadiusForSphere = 0.5000000000f; // "%.10f" % (Vector3(1,0,0) * 0.5).length()
const float MathUtils::ArrowRadiusForSphere = 0.5153881907f; // "%.10f" % (Vector3(1,0.25,0) * 0.5).length()

void MathUtils::get_diagonal_vectors(Vector3 a, Vector3 b, Vector3 &bottom, Vector3 &top, Vector3 &diag) {
	bottom = Vector3();
	top = Vector3();

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

bool MathUtils::is_bounds_partially_inside_convex_shape(SphereBounds sphere, std::vector<Plane> planes) {
	for (Plane p : planes)
		if (p.distance_to(sphere.Position) >= sphere.Radius)
			return false;

	return true;
}

real_t MathUtils::get_max_value(Vector3 &value) {
	return Math::max(abs(value.x), Math::max(abs(value.y), abs(value.z)));
}

real_t MathUtils::get_max_vector_length(Vector3 &a, Vector3 &b, Vector3 &c) {
	real_t a_l = a.length();
	real_t b_l = b.length();
	real_t c_l = c.length();
	return Math::max(a_l, Math::max(b_l, c_l));
}

real_t MathUtils::get_max_basis_length(Basis &b) {
	real_t a_l = b.get_column(0).length();
	real_t b_l = b.get_column(1).length();
	real_t c_l = b.get_column(2).length();
	return Math::max(a_l, Math::max(b_l, c_l));
}
