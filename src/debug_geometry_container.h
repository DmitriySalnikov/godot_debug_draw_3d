#pragma once

#include "colors.h"
#include "geometry_generators.h"
#include "math_utils.h"
#include "render_instances.h"

MSVC_WARNING_DISABLE(4244)
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/texture.hpp>
MSVC_WARNING_RESTORE(4244)

#include <map>

using namespace godot;

class DebugDrawStats;

class DebugGeometryContainer {
	class DebugDraw *owner;

	struct MultiMeshStorage {
		RID instance;
		Ref<MultiMesh> mesh;

		~MultiMeshStorage() {
			RS()->free_rid(instance);
			mesh.unref();
		}
	};
	MultiMeshStorage multi_mesh_storage[InstanceType::ALL] = {};

	struct ImmediateMeshStorage {
		RID instance;
		Ref<ArrayMesh> mesh;
		Ref<StandardMaterial3D> material;

		~ImmediateMeshStorage() {
			RS()->free_rid(instance);
			mesh.unref();
			material.unref();
		}
	};
	ImmediateMeshStorage immediate_mesh_storage;

	GeometryPool geometry_pool;
	Node *scene_world_node = nullptr;
	int32_t render_layers = 1;

	void CreateMMI(InstanceType type, const String &name, Ref<ArrayMesh> mesh);

	template <class TVertices, class TIndices = std::array<int, 0>, class TColors = std::array<Color, 0> >
	Ref<ArrayMesh> CreateMesh(Mesh::PrimitiveType type, const TVertices &vertices, const TIndices &indices = {}, const TColors &colors = {}) {
		return CreateMesh(type,
				Utils::convert_to_pool_array<PackedVector3Array>(vertices),
				Utils::convert_to_pool_array<PackedInt32Array>(indices),
				Utils::convert_to_pool_array<PackedColorArray>(colors));
	}

	Ref<ArrayMesh> CreateMesh(Mesh::PrimitiveType type, const PackedVector3Array &vertices, const PackedInt32Array &indices = {}, const PackedColorArray &colors = {});

	std::recursive_mutex datalock;

public:
	DebugGeometryContainer(class DebugDraw *root);
	~DebugGeometryContainer();

	void set_world(Node *new_world);
	Node *get_world();

	void update_geometry(double delta);

	void set_render_layer_mask(int32_t layers);
	int32_t get_render_layer_mask() const;

	Ref<DebugDrawStats> get_render_stats();
	void create_arrow(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration = 0);

#pragma region Exposed Draw Functions
	void clear_3d_objects();
#pragma region 3D

#pragma region Spheres
	void draw_sphere(const Vector3 &position, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0, const bool &hd = false);
	void draw_sphere_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0, const bool &hd = false);

	void draw_sphere_hd(const Vector3 &position, const real_t &radius = 0.5f, const Color &color = Colors::empty_color, const real_t &duration = 0);
	void draw_sphere_hd_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);

#pragma endregion // Spheres

#pragma region Cylinders
	void draw_cylinder(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);
#pragma endregion // Cylinders

#pragma region Boxes
	void draw_box(const Vector3 &position, const Vector3 &size, const Color &color = Colors::empty_color, const bool &is_box_centered = false, const real_t &duration = 0);
	void draw_box_xf(const Transform3D &transform, const Color &color = Colors::empty_color, const bool &is_box_centered = true, const real_t &duration = 0);
	void draw_aabb(const AABB &aabb, const Color &color = Colors::empty_color, const real_t &duration = 0);
	void draw_aabb_ab(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &duration = 0);
#pragma endregion // Boxes

#pragma region Lines
	void draw_line_hit(const Vector3 &start, const Vector3 &end, const Vector3 &hit, const bool &is_hit, const real_t &hit_size = 0.25f, const Color &hit_color = Colors::empty_color, const Color &after_hit_color = Colors::empty_color, const real_t &duration = 0);
	void draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit = 0.5f, const real_t &hit_size = 0.25f, const Color &hit_color = Colors::empty_color, const Color &after_hit_color = Colors::empty_color, const real_t &duration = 0);
#pragma region Normal
	void draw_line(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &duration = 0);

	void draw_lines(const PackedVector3Array &lines, const Color &color = Colors::empty_color, const real_t &duration = 0);
	void draw_lines_c(const std::vector<Vector3> &lines, const Color &color = Colors::empty_color, const real_t &duration = 0);

	void draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color = Colors::empty_color, const real_t &duration = 0);
	void draw_line_path(const PackedVector3Array &path, const Color &color = Colors::empty_color, const real_t &duration = 0);
#pragma endregion // Normal

#pragma region Arrows
	void draw_arrow(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);

	void draw_arrow_line(const Vector3 &a, const Vector3 &b, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.5f, const bool &is_absolute_size = false, const real_t &duration = 0);
	void draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.5f, const bool &is_absolute_size = false, const real_t &duration = 0);
	void draw_arrow_path(const PackedVector3Array &path, const Color &color = Colors::empty_color, const real_t &arrow_size = 0.75f, const bool &is_absolute_size = true, const real_t &duration = 0);
#pragma endregion // Arrows
#pragma region Points

	void draw_point_path(const PackedVector3Array &path, const real_t &size = 0.25f, const Color &points_color = Colors::empty_color, const Color &lines_color = Colors::empty_color, const real_t &duration = 0);
#pragma endregion // Points
#pragma endregion // Lines

#pragma region Misc
	void draw_square(const Vector3 &position, const real_t &size = 0.2f, const Color &color = Colors::empty_color, const real_t &duration = 0);
	void draw_points(const PackedVector3Array &points, const real_t &size = 0.25f, const Color &color = Colors::empty_color, const real_t &duration = 0);

	void draw_position(const Transform3D &transform, const Color &color = Colors::empty_color, const real_t &duration = 0);

	void draw_gizmo(const Transform3D &transform, const Color &color = Colors::empty_color, const bool &is_centered = false, const real_t &duration = 0);

	void draw_grid(const Vector3 &origin, const Vector3 &x_size, const Vector3 &y_size, const Vector2i &subdivision, const Color &color = Colors::empty_color, const bool &is_centered = true, const real_t &duration = 0);
	void draw_grid_xf(const Transform3D &transform, const Vector2i &_subdivision, const Color &color = Colors::empty_color, const bool &is_centered = true, const real_t &duration = 0);

#pragma region Camera Frustum
	void draw_camera_frustum(const Camera3D *camera, const Color &color = Colors::empty_color, const real_t &duration = 0);
	void draw_camera_frustum_planes(const Array &camera_frustum, const Color &color = Colors::empty_color, const real_t &duration = 0);
	void draw_camera_frustum_planes_c(const std::array<Plane, 6> &planes, const Color &color = Colors::empty_color, const real_t &duration = 0);
#pragma endregion // Camera Frustum

#pragma endregion // Misc
#pragma endregion // 3D
#pragma endregion // Exposed Draw Functions
};
