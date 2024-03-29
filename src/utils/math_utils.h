#pragma once

#include "compiler.h"

#include <array>
#include <functional>

GODOT_WARNING_DISABLE()
#include <godot_cpp/variant/builtin_types.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

struct AABBMinMax;

class MathUtils {

public:
	static const float Sqrt2;
	static const float CubeRadiusForSphere;
	static const float CylinderRadiusForSphere;
	static const float AxisRadiusForSphere;
	static const float ArrowRadiusForSphere;

	_FORCE_INLINE_ static void get_diagonal_vectors(const Vector3 &p_a, const Vector3 &p_b, Vector3 &r_bottom, Vector3 &r_top, Vector3 &r_diag);
	_FORCE_INLINE_ static bool is_bounds_partially_inside_convex_shape(const AABBMinMax &p_bounds, const std::array<Plane, 6> &p_planes);
	_FORCE_INLINE_ static real_t get_max_value(const Vector3 &p_value);
	_FORCE_INLINE_ static real_t get_max_vector_length(const Vector3 &p_a, const Vector3 &p_b, const Vector3 &p_c);
	_FORCE_INLINE_ static real_t get_max_basis_length(const Basis &p_b);
	_FORCE_INLINE_ static AABB calculate_vertex_bounds(const Vector3 *p_lines, size_t p_count);

	_FORCE_INLINE_ static std::array<Vector3, 8> get_frustum_cube(const std::array<Plane, 6> p_frustum);
	_FORCE_INLINE_ static void scale_frustum_far_plane_distance(std::array<Plane, 6> &p_frustum, const Transform3D &p_camera_xf, const real_t &p_scale);
};

struct SphereBounds {
	Vector3 position;
	real_t radius;

	SphereBounds();
	SphereBounds(const Vector3 &p_pos, const real_t &p_radius);
	SphereBounds(const AABB &p_from);
};

struct AABBMinMax {
	// Sphere/AABB center
	Vector3 center;
	// Sphere radius
	real_t radius;
	// AABB min max
	Vector3 min;
	Vector3 max;

	AABBMinMax();
	AABBMinMax(const AABB &p_from);
	AABBMinMax(const SphereBounds &p_from);

	_FORCE_INLINE_ bool intersects(const AABBMinMax &p_aabb) const;
	_FORCE_INLINE_ void merge_with(const AABBMinMax &p_aabb);
	_FORCE_INLINE_ void reset();

	_FORCE_INLINE_ operator AABB() {
		return AABB(min, max - min);
	}
};

void MathUtils::get_diagonal_vectors(const Vector3 &p_a, const Vector3 &p_b, Vector3 &r_bottom, Vector3 &r_top, Vector3 &r_diag) {
	r_bottom = Vector3();
	r_top = Vector3();

	if (p_a.x > p_b.x) {
		r_top.x = p_a.x;
		r_bottom.x = p_b.x;
	} else {
		r_top.x = p_b.x;
		r_bottom.x = p_a.x;
	}

	if (p_a.y > p_b.y) {
		r_top.y = p_a.y;
		r_bottom.y = p_b.y;
	} else {
		r_top.y = p_b.y;
		r_bottom.y = p_a.y;
	}

	if (p_a.z > p_b.z) {
		r_top.z = p_a.z;
		r_bottom.z = p_b.z;
	} else {
		r_top.z = p_b.z;
		r_bottom.z = p_a.z;
	}

	r_diag = r_top - r_bottom;
}

bool MathUtils::is_bounds_partially_inside_convex_shape(const AABBMinMax &p_sphere, const std::array<Plane, 6> &p_planes) {
	for (int i = 0; i < 6; i++) {
		if (p_sphere.radius < p_planes[i].distance_to(p_sphere.center))
			return false;
	}

	return true;
}

real_t MathUtils::get_max_value(const Vector3 &p_value) {
	return Math::max(abs(p_value.x), Math::max(abs(p_value.y), abs(p_value.z)));
}

real_t MathUtils::get_max_vector_length(const Vector3 &p_a, const Vector3 &p_b, const Vector3 &p_c) {
	real_t a_l = p_a.length();
	real_t b_l = p_b.length();
	real_t c_l = p_c.length();
	return Math::max(a_l, Math::max(b_l, c_l));
}

real_t MathUtils::get_max_basis_length(const Basis &p_b) {
	real_t a_l = p_b.get_column(0).length();
	real_t b_l = p_b.get_column(1).length();
	real_t c_l = p_b.get_column(2).length();
	return Math::max(a_l, Math::max(b_l, c_l));
}

AABB MathUtils::calculate_vertex_bounds(const Vector3 *p_lines, size_t p_count) {
	if (p_count > 0) {
		// Using the original Godot expand_to code to avoid creating new AABB instances
		Vector3 min = p_lines[0];
		Vector3 max = p_lines[0];

		for (size_t i = 0; i < p_count; i++) {
			auto &v = p_lines[i];
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

std::array<Vector3, 8> MathUtils::get_frustum_cube(const std::array<Plane, 6> p_frustum) {
	std::function<Vector3(const Plane &, const Plane &, const Plane &)> intersect_planes = [&](const Plane &a, const Plane &b, const Plane &c) {
		Vector3 intersec_result;
		a.intersect_3(b, c, &intersec_result);
		return intersec_result;
	};

	//  near, far, left, top, right, bottom
	//  0,    1,   2,    3,   4,     5
	return std::array<Vector3, 8>({
			intersect_planes(p_frustum[0], p_frustum[3], p_frustum[2]),
			intersect_planes(p_frustum[0], p_frustum[3], p_frustum[4]),
			intersect_planes(p_frustum[0], p_frustum[5], p_frustum[4]),
			intersect_planes(p_frustum[0], p_frustum[5], p_frustum[2]),

			intersect_planes(p_frustum[1], p_frustum[3], p_frustum[2]),
			intersect_planes(p_frustum[1], p_frustum[3], p_frustum[4]),
			intersect_planes(p_frustum[1], p_frustum[5], p_frustum[4]),
			intersect_planes(p_frustum[1], p_frustum[5], p_frustum[2]),
	});
}

_FORCE_INLINE_ void MathUtils::scale_frustum_far_plane_distance(std::array<Plane, 6> &p_frustum, const Transform3D &p_camera_xf, const real_t &p_scale) {
	//  near, far, left, top, right, bottom
	//  0,    1,   2,    3,   4,     5
	real_t near_len = p_frustum[0].distance_to(p_camera_xf.origin);
	real_t far_len = p_frustum[1].distance_to(p_camera_xf.origin);

	if (abs(near_len) > abs(far_len)) {
		std::swap(near_len, far_len);
		p_frustum[1] = Plane(p_frustum[1].normal, p_camera_xf.origin + p_camera_xf.basis.get_column(2) * ((1.f - p_scale) * (far_len + near_len) - near_len));
	} else {
		p_frustum[1] = Plane(p_frustum[1].normal, p_camera_xf.origin + p_camera_xf.basis.get_column(2) * (p_scale * (far_len + near_len) - near_len));
	}
}

bool AABBMinMax::intersects(const AABBMinMax &p_aabb) const {
	return min.x < p_aabb.max.x &&
		   max.x > p_aabb.min.x &&
		   min.y < p_aabb.max.y &&
		   max.y > p_aabb.min.y &&
		   min.z < p_aabb.max.z &&
		   max.z > p_aabb.min.z;
}

_FORCE_INLINE_ void AABBMinMax::merge_with(const AABBMinMax &p_aabb) {
	if (radius) {
		max = max.max(p_aabb.max);
		min = min.min(p_aabb.min);
	} else {
		radius = 1;
		max = p_aabb.max;
		min = p_aabb.min;
	}
}

_FORCE_INLINE_ void AABBMinMax::reset() {
	center = Vector3();
	radius = 0;
	max = Vector3();
	min = Vector3();
}
