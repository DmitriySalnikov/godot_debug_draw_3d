#pragma once

#include "compiler.h"

#include <array>

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

	static void get_diagonal_vectors(const Vector3 &a, const Vector3 &b, Vector3 &bottom, Vector3 &top, Vector3 &diag);
	static bool is_bounds_partially_inside_convex_shape(const AABBMinMax &bounds, const std::array<Plane, 6> &planes);
	static real_t get_max_value(const Vector3 &value);
	static real_t get_max_vector_length(const Vector3 &a, const Vector3 &b, const Vector3 &c);
	static real_t get_max_basis_length(const Basis &b);
	static AABB calculate_vertex_bounds(const Vector3 *lines, size_t count);
	static std::array<Vector3, 8> get_frustum_cube(const std::array<Plane, 6> frustum);
};

class SphereBounds {
public:
	Vector3 position;
	real_t radius;

	SphereBounds() :
			position(),
			radius() {}

	SphereBounds(const Vector3 &_pos, const real_t &_radius) :
			position(_pos),
			radius(_radius) {}

	SphereBounds(const AABB &from) :
			position(from.get_center()),
			radius(from.size.length() * 0.5f) {}
};

struct AABBMinMax {
	// AABB extent and Sphere/AABB center
	Vector3 center;
	Vector3 extent;
	// AABB min max
	Vector3 min;
	Vector3 max;
	// Sphere radius
	real_t radius;

	AABBMinMax() :
			center(),
			extent(),
			min(),
			max(),
			radius() {}

	AABBMinMax(const AABB &from) :
			extent(from.size * 0.5f),
			min(from.position),
			max(from.position + from.size),
			radius(from.size.length() * 0.5f) {
		center = from.position + extent;
	}

	AABBMinMax(const SphereBounds &from) :
			center(from.position),
			extent(Vector3(from.radius, from.radius, from.radius)),
			radius(from.radius) {
		min = from.position - extent;
		max = from.position + extent;
	}

	inline bool intersects(const AABBMinMax &p_aabb) const {
		if (min.x > p_aabb.max.x) {
			return false;
		}
		if (max.x < p_aabb.min.x) {
			return false;
		}
		if (min.y > p_aabb.max.y) {
			return false;
		}
		if (max.y < p_aabb.min.y) {
			return false;
		}
		if (min.z > p_aabb.max.z) {
			return false;
		}
		if (max.z < p_aabb.min.z) {
			return false;
		}

		return true;
	}
};
