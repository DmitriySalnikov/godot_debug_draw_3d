#include "math_utils.h"

const float MathUtils::Sqrt2 = Math::sqrt(2.f);
const float MathUtils::CubeRadiusForSphere = 0.8660253882f; // "%.10f" % (Vector3.ONE * 0.5).length()
const float MathUtils::CylinderRadiusForSphere = 0.7071067691f; // "%.10f" % (Vector3(1,1,0) * 0.5).length()
const float MathUtils::AxisRadiusForSphere = 0.5000000000f; // "%.10f" % (Vector3(1,0,0) * 0.5).length()
const float MathUtils::ArrowRadiusForSphere = 0.5153881907f; // "%.10f" % (Vector3(1,0.25,0) * 0.5).length()

SphereBounds::SphereBounds() :
		position(),
		radius() {}

SphereBounds::SphereBounds(const Vector3 &p_pos, const real_t &p_radius) :
		position(p_pos),
		radius(p_radius) {}

SphereBounds::SphereBounds(const AABB &p_from) :
		position(p_from.get_center()),
		radius(p_from.size.length() * 0.5f) {}

AABBMinMax::AABBMinMax() :
		center(),
		radius(),
		min(),
		max() {}

AABBMinMax::AABBMinMax(const AABB &p_from) :
		radius(p_from.size.length() * .5f),
		min(p_from.position),
		max(p_from.position + p_from.size) {
	center = p_from.position + p_from.size * 0.5f;
}

AABBMinMax::AABBMinMax(const SphereBounds &p_from) :
		center(p_from.position),
		radius(p_from.radius) {
	Vector3 half = Vector3(p_from.radius, p_from.radius, p_from.radius);
	min = p_from.position - half;
	max = p_from.position + half;
}
