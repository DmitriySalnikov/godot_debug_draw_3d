#pragma once

#include "geometry_generators.h"
#include "math_utils.h"
#include "render_instances.h"
#include "utils.h"

#include <ArrayMesh.hpp>
#include <Camera.hpp>
#include <CanvasItem.hpp>
#include <Font.hpp>
#include <Godot.hpp>
#include <ImmediateGeometry.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <MultiMesh.hpp>
#include <MultiMeshInstance.hpp>
#include <Reference.hpp>
#include <SpatialMaterial.hpp>

#include <map>

using namespace godot;

class DebugGeometryContainer {
	class DebugDraw3D *owner;
	MultiMeshInstance *mmi_with_values[InstanceType::ALL];

	ImmediateGeometry *_immediateGeometry = nullptr;
	GeometryPool geometry_pool;

	MultiMeshInstance *CreateMMI(Node *root, InstanceType type, String name);
	Ref<ArrayMesh> CreateMesh(Mesh::PrimitiveType type, const Vector3 *vertices, const size_t vertices_size, const int *indices = nullptr, const size_t indices_size = 0, const Color *colors = nullptr, const size_t colors_size = 0);
	Ref<ArrayMesh> CreateMesh(Mesh::PrimitiveType type, const std::vector<Vector3> vertices, const std::vector<int> indices = {}, const std::vector<Color> colors = {});
	Ref<ArrayMesh> CreateMesh(Mesh::PrimitiveType type, const PoolVector3Array &vertices, const PoolIntArray &indices = {}, const PoolColorArray &colors = {});

	std::recursive_mutex datalock;

public:
	DebugGeometryContainer(class DebugDraw3D *root);
	~DebugGeometryContainer();

	void update_geometry(real_t delta);

	Dictionary get_rendered_primitives_count();
	void create_arrow(Vector3 a, Vector3 b, Color color, real_t arrow_size, bool is_absolute_size, float duration = 0);

#pragma region Exposed Draw Functions
	void clear_3d_objects();
#pragma region 3D

#pragma region Spheres
	void draw_sphere(Vector3 position, real_t radius = 1, Color color = Colors::empty_color, float duration = 0, bool hd = false);
	void draw_sphere_xf(Transform transform, Color color = Colors::empty_color, float duration = 0, bool hd = false);
	
	void draw_sphere_hd(Vector3 position, float radius = 0.5f, Color color = Colors::empty_color, float duration = 0);
	void draw_sphere_hd_xf(Transform transform, Color color = Colors::empty_color, float duration = 0);

#pragma endregion // Spheres

#pragma region Cylinders
	void draw_cylinder(Vector3 position, Quat rotation = Quat(), real_t radius = 1, real_t height = 1, Color color = Colors::empty_color, float duration = 0);
	void draw_cylinder_xf(Transform transform, Color color = Colors::empty_color, float duration = 0);
#pragma endregion // Cylinders

#pragma region Boxes
	void draw_box(Vector3 position, Quat rotation = Quat(), Vector3 size = Vector3::ONE, Color color = Colors::empty_color, bool is_box_centered = true, float duration = 0);
	void draw_box_xf(Transform transform, Color color = Colors::empty_color, bool is_box_centered = true, float duration = 0);
	void draw_aabb(AABB aabb, Color color = Colors::empty_color, float duration = 0);
	void draw_aabb_ab(Vector3 a, Vector3 b, Color color = Colors::empty_color, float duration = 0);
#pragma endregion // Boxes

#pragma region Lines
	void draw_line_3d_hit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, float hit_size = 0.25f, Color hit_color = Colors::empty_color, Color after_hit_color = Colors::empty_color, float duration = 0);
	void draw_line_3d_hit_offset(Vector3 start, Vector3 end, bool is_hit, float unit_offset_of_hit = 0.5f, float hit_size = 0.25f, Color hit_color = Colors::empty_color, Color after_hit_color = Colors::empty_color, float duration = 0);
#pragma region Normal
	void draw_line_3d(Vector3 a, Vector3 b, Color color = Colors::empty_color, float duration = 0);
	void draw_lines_3d(PoolVector3Array lines, Color color = Colors::empty_color, float duration = 0);
	void draw_ray_3d(Vector3 origin, Vector3 direction, float length, Color color = Colors::empty_color, float duration = 0);
	void draw_line_path_3d(PoolVector3Array path, Color color = Colors::empty_color, float duration = 0);
#pragma endregion // Normal

#pragma region Arrows
	void draw_arrow_line_3d(Vector3 a, Vector3 b, Color color = Colors::empty_color, float arrow_size = 0.15f, bool is_absolute_size = false, float duration = 0);
	void draw_arrow_ray_3d(Vector3 origin, Vector3 direction, float length, Color color = Colors::empty_color, float arrow_size = 0.15f, bool is_absolute_size = false, float duration = 0);
	void draw_arrow_path_3d(PoolVector3Array path, Color color = Colors::empty_color, float arrow_size = 0.75f, bool is_absolute_size = true, float duration = 0);
#pragma endregion // Arrows
#pragma endregion // Lines

#pragma region Misc
	void draw_billboard_square(Vector3 position, float size = 0.2f, Color color = Colors::empty_color, float duration = 0);
#pragma region Camera Frustum
	void draw_camera_frustum(Camera *camera, Color color = Colors::empty_color, float duration = 0);
	void draw_camera_frustum_planes(Array camera_frustum, Color color = Colors::empty_color, float duration = 0);
	void draw_camera_frustum_planes_c(Plane planes[6], Color color = Colors::empty_color, float duration = 0);

#pragma endregion // Camera Frustum
	void draw_position_3d(Vector3 position, Quat rotation = Quat(), Vector3 scale = Vector3::ONE, Color color = Colors::empty_color, float duration = 0);
	void draw_position_3d_xf(Transform transform, Color color = Colors::empty_color, float duration = 0);

	void draw_gizmo_3d(Vector3 position, Quat rotation, Vector3 scale, bool is_centered = false, float duration = 0);
	void draw_gizmo_3d_xf(Transform transform, bool is_centered = false, float duration = 0);

#pragma endregion // Misc
#pragma endregion // 3D
#pragma endregion // Exposed Draw Functions
};
