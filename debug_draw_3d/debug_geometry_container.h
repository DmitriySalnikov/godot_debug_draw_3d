#pragma once

#include "colors.h"
#include "geometry_generators.h"
#include "math_utils.h"
#include "render_instances.h"

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
	MultiMeshInstance *multi_mesh_instances[InstanceType::ALL];

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
	void set_render_layer_mask(int64_t layers);

	Dictionary get_rendered_primitives_count();
	void create_arrow(Vector3 a, Vector3 b, Color color, real_t arrow_size, bool is_absolute_size, real_t duration = 0);

#pragma region Exposed Draw Functions
	void clear_3d_objects();
#pragma region 3D

#pragma region Spheres
	void draw_sphere(Vector3 position, real_t radius = 0.5f, Color color = Colors::empty_color, real_t duration = 0, bool hd = false);
	void draw_sphere_xf(Transform transform, Color color = Colors::empty_color, real_t duration = 0, bool hd = false);

	void draw_sphere_hd(Vector3 position, real_t radius = 0.5f, Color color = Colors::empty_color, real_t duration = 0);
	void draw_sphere_hd_xf(Transform transform, Color color = Colors::empty_color, real_t duration = 0);

#pragma endregion // Spheres

#pragma region Cylinders
	void draw_cylinder(Transform transform, Color color = Colors::empty_color, real_t duration = 0);
#pragma endregion // Cylinders

#pragma region Boxes
	void draw_box(Transform transform, Color color = Colors::empty_color, bool is_box_centered = true, real_t duration = 0);
	void draw_aabb(AABB aabb, Color color = Colors::empty_color, real_t duration = 0);
	void draw_aabb_ab(Vector3 a, Vector3 b, Color color = Colors::empty_color, real_t duration = 0);
#pragma endregion // Boxes

#pragma region Lines
	void draw_line_hit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, real_t hit_size = 0.25f, Color hit_color = Colors::empty_color, Color after_hit_color = Colors::empty_color, real_t duration = 0);
	void draw_line_hit_offset(Vector3 start, Vector3 end, bool is_hit, real_t unit_offset_of_hit = 0.5f, real_t hit_size = 0.25f, Color hit_color = Colors::empty_color, Color after_hit_color = Colors::empty_color, real_t duration = 0);
#pragma region Normal
	void draw_line(Vector3 a, Vector3 b, Color color = Colors::empty_color, real_t duration = 0);

	void draw_lines(PoolVector3Array lines, Color color = Colors::empty_color, real_t duration = 0);
	void draw_lines_c(std::vector<Vector3> &lines, Color color = Colors::empty_color, real_t duration = 0);

	void draw_ray(Vector3 origin, Vector3 direction, real_t length, Color color = Colors::empty_color, real_t duration = 0);
	void draw_line_path(PoolVector3Array path, Color color = Colors::empty_color, real_t duration = 0);
#pragma endregion // Normal

#pragma region Arrows
	void draw_arrow(Transform transform, Color color = Colors::empty_color, real_t duration = 0);

	void draw_arrow_line(Vector3 a, Vector3 b, Color color = Colors::empty_color, real_t arrow_size = 0.15f, bool is_absolute_size = false, real_t duration = 0);
	void draw_arrow_ray(Vector3 origin, Vector3 direction, real_t length, Color color = Colors::empty_color, real_t arrow_size = 0.15f, bool is_absolute_size = false, real_t duration = 0);
	void draw_arrow_path(PoolVector3Array path, Color color = Colors::empty_color, real_t arrow_size = 0.75f, bool is_absolute_size = true, real_t duration = 0);
#pragma endregion // Arrows
#pragma endregion // Lines

#pragma region Misc
	void draw_billboard_square(Vector3 position, real_t size = 0.2f, Color color = Colors::empty_color, real_t duration = 0);

	void draw_position(Transform transform, Color color = Colors::empty_color, real_t duration = 0);

	void draw_gizmo(Transform transform, bool is_centered = false, real_t duration = 0);

	void draw_grid(Vector3 origin, Vector3 x_size, Vector3 y_size, Vector2 subdivision, Color color = Colors::empty_color, bool is_centered = true, real_t duration = 0);
	void draw_grid_xf(Transform transform, Vector2 subdivision, Color color = Colors::empty_color, bool is_centered = true, real_t duration = 0);

#pragma region Camera Frustum
	void draw_camera_frustum(Camera *camera, Color color = Colors::empty_color, real_t duration = 0);
	void draw_camera_frustum_planes(Array camera_frustum, Color color = Colors::empty_color, real_t duration = 0);
	void draw_camera_frustum_planes_c(Plane planes[6], Color color = Colors::empty_color, real_t duration = 0);
#pragma endregion // Camera Frustum

#pragma endregion // Misc
#pragma endregion // 3D
#pragma endregion // Exposed Draw Functions
};
