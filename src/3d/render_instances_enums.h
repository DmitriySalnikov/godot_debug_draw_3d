#pragma once

enum class GeometryType : char {
	Wireframe,
	Volumetric,
	Solid,
};

enum class ConvertableInstanceType : char {
	CUBE,
	CUBE_CENTERED,
	ARROWHEAD,
	POSITION,
	SPHERE,
	CYLINDER,
	CYLINDER_AB,
};

enum class InstanceType : char {
	// Basic wireframe
	CUBE,
	CUBE_CENTERED,
	ARROWHEAD,
	POSITION,
	SPHERE,
	SPHERE_HD,
	CYLINDER,
	CYLINDER_AB,

	// Volumetric from wireframes
	LINE_VOLUMETRIC,
	CUBE_VOLUMETRIC,
	CUBE_CENTERED_VOLUMETRIC,
	ARROWHEAD_VOLUMETRIC,
	POSITION_VOLUMETRIC,
	SPHERE_VOLUMETRIC,
	SPHERE_HD_VOLUMETRIC,
	CYLINDER_VOLUMETRIC,
	CYLINDER_AB_VOLUMETRIC,

	// Solid geometry
	BILLBOARD_SQUARE,
	PLANE,

	MAX,
};

enum class ProcessType : char {
	PROCESS,
	PHYSICS_PROCESS,
	MAX,
};
