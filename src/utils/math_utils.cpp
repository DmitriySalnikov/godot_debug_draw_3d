#include "math_utils.h"

#include <functional>

const float MathUtils::Sqrt2 = Math::sqrt(2.f);
const float MathUtils::CubeRadiusForSphere = 0.8660253882f; // "%.10f" % (Vector3.ONE * 0.5).length()
const float MathUtils::CylinderRadiusForSphere = 0.7071067691f; // "%.10f" % (Vector3(1,1,0) * 0.5).length()
const float MathUtils::AxisRadiusForSphere = 0.5000000000f; // "%.10f" % (Vector3(1,0,0) * 0.5).length()
const float MathUtils::ArrowRadiusForSphere = 0.5153881907f; // "%.10f" % (Vector3(1,0.25,0) * 0.5).length()

void MathUtils::get_diagonal_vectors(const Vector3 &a, const Vector3 &b, Vector3 &bottom, Vector3 &top, Vector3 &diag) {
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

bool MathUtils::is_bounds_partially_inside_convex_shape(const AABBMinMax &sphere, const std::array<Plane, 6> &planes) {
	for (int i = 0; i < planes.size(); i++) {
		if (sphere.radius < planes[i].distance_to(sphere.center))
			return false;
	}

	return true;
}

real_t MathUtils::get_max_value(const Vector3 &value) {
	return Math::max(abs(value.x), Math::max(abs(value.y), abs(value.z)));
}

real_t MathUtils::get_max_vector_length(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
	real_t a_l = a.length();
	real_t b_l = b.length();
	real_t c_l = c.length();
	return Math::max(a_l, Math::max(b_l, c_l));
}

real_t MathUtils::get_max_basis_length(const Basis &b) {
	real_t a_l = b.get_column(0).length();
	real_t b_l = b.get_column(1).length();
	real_t c_l = b.get_column(2).length();
	return Math::max(a_l, Math::max(b_l, c_l));
}

AABB MathUtils::calculate_vertex_bounds(const Vector3 *lines, size_t count) {
	if (count > 0) {
		// Using the original Godot expand_to code to avoid creating new AABB instances
		Vector3 min = lines[0];
		Vector3 max = lines[0];

		for (size_t i = 0; i < count; i++) {
			auto &v = lines[i];
			if (v.x < min.x) {
				min.x = v.x;
			}
			if (v.y < min.y) {
				min.y = v.y;
			}
			if (v.z < min.z) {
				min.z = v.z;
			}

			if (v.x > max.x) {
				max.x = v.x;
			}
			if (v.y > max.y) {
				max.y = v.y;
			}
			if (v.z > max.z) {
				max.z = v.z;
			}
		}

		return AABB(min, max - min);
	} else {
		return AABB();
	}
}

std::array<Vector3, 8> MathUtils::get_frustum_cube(const std::array<Plane, 6> frustum) {
	std::function<Vector3(const Plane &, const Plane &, const Plane &)> intersect_planes = [&](const Plane &a, const Plane &b, const Plane &c) {
		Vector3 intersec_result;
		a.intersect_3(b, c, &intersec_result);
		return intersec_result;
	};

	//  near, far, left, top, right, bottom
	//  0,    1,   2,    3,   4,     5
	return std::array<Vector3, 8>({
			intersect_planes(frustum[0], frustum[3], frustum[2]),
			intersect_planes(frustum[0], frustum[3], frustum[4]),
			intersect_planes(frustum[0], frustum[5], frustum[4]),
			intersect_planes(frustum[0], frustum[5], frustum[2]),

			intersect_planes(frustum[1], frustum[3], frustum[2]),
			intersect_planes(frustum[1], frustum[3], frustum[4]),
			intersect_planes(frustum[1], frustum[5], frustum[4]),
			intersect_planes(frustum[1], frustum[5], frustum[2]),
	});
}
