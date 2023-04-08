#include "debug_geometry_container.h"
#include "debug_draw.h"
#include "debug_draw_config_3d.h"
#include "utils.h"
#include "draw_stats.h"

MSVC_WARNING_DISABLE(4244)
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/world3d.hpp>
MSVC_WARNING_RESTORE(4244)

#include <array>

using namespace godot;

DebugGeometryContainer::DebugGeometryContainer(class DebugDraw *root) {
	owner = root;
	RenderingServer *rs = RS();

	// Create wireframe mesh drawer
	{
		Ref<ArrayMesh> _array_mesh;
		_array_mesh.instantiate();
		RID _immediate_instance = rs->instance_create();

		rs->instance_set_base(_immediate_instance, _array_mesh->get_rid());
		rs->instance_geometry_set_cast_shadows_setting(_immediate_instance, RenderingServer::SHADOW_CASTING_SETTING_OFF);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

		Ref<StandardMaterial3D> mat;
		mat.instantiate();
		mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
		mat->set_flag(StandardMaterial3D::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

		rs->instance_geometry_set_material_override(_immediate_instance, mat->get_rid());

		immediate_mesh_storage.instance = _immediate_instance;
		immediate_mesh_storage.material = mat;
		immediate_mesh_storage.mesh = _array_mesh;
	}

	// Create node with material and MultiMesh. Add to tree. Create array of instances
	{
		CreateMMI(InstanceType::CUBES, TEXT(mmi_cubes), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CubeVertices, GeometryGenerator::CubeIndices));
		CreateMMI(InstanceType::CUBES_CENTERED, TEXT(mmi_cubes_centered), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CenteredCubeVertices, GeometryGenerator::CubeIndices));
		CreateMMI(InstanceType::ARROWHEADS, TEXT(mmi_arrowheads), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::ArrowheadVertices, GeometryGenerator::ArrowheadIndices));
		CreateMMI(InstanceType::BILLBOARD_SQUARES, TEXT(mmi_billboard_squares), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES, GeometryGenerator::CenteredSquareVertices, GeometryGenerator::SquareIndices));
		CreateMMI(InstanceType::POSITIONS, TEXT(mmi_positions), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::PositionVertices, GeometryGenerator::PositionIndices));
		CreateMMI(InstanceType::SPHERES, TEXT(mmi_spheres), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CreateSphereLines(8, 8, 0.5f, Vector3_ZERO)));
		CreateMMI(InstanceType::SPHERES_HD, TEXT(mmi_spheres_hd), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CreateSphereLines(16, 16, 0.5f, Vector3_ZERO)));
		CreateMMI(InstanceType::CYLINDERS, TEXT(mmi_cylinders), CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CreateCylinderLines(52, 1, 1, Vector3_ZERO, 4)));

		set_render_layer_mask(1);
	}
}

DebugGeometryContainer::~DebugGeometryContainer() {
	LOCK_GUARD(datalock);

	geometry_pool.clear_pool();
	geometry_pool.clear_pool();

	RenderingServer *rs = RS();
	rs->free_rid(immediate_mesh_storage.instance);
	immediate_mesh_storage.mesh.unref();
	immediate_mesh_storage.material.unref();

	for (auto &i : multi_mesh_storage) {
		rs->free_rid(i.instance);
		i.mesh.unref();
	}
}

void DebugGeometryContainer::CreateMMI(InstanceType type, const String &name, Ref<ArrayMesh> mesh) {
	RenderingServer *rs = RS();

	RID mmi = rs->instance_create();

	Ref<MultiMesh> new_mm;
	new_mm.instantiate();
	new_mm->set_name(String::num_int64(type));

	new_mm->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);
	new_mm->set_use_colors(true);
	new_mm->set_transform_format(MultiMesh::TRANSFORM_3D);
	new_mm->set_use_custom_data(false);
	new_mm->set_mesh(mesh);

	rs->instance_set_base(mmi, new_mm->get_rid());

	Ref<StandardMaterial3D> new_mat;
	new_mat.instantiate();
	new_mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	new_mat->set_flag(StandardMaterial3D::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

	if (type == InstanceType::BILLBOARD_SQUARES) {
		new_mat->set_billboard_mode(StandardMaterial3D::BillboardMode::BILLBOARD_ENABLED);
		new_mat->set_flag(StandardMaterial3D::Flags::FLAG_BILLBOARD_KEEP_SCALE, true);
	}

	mesh->surface_set_material(0, new_mat);

	rs->instance_geometry_set_cast_shadows_setting(mmi, RenderingServer::SHADOW_CASTING_SETTING_OFF);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

	multi_mesh_storage[type].instance = mmi;
	multi_mesh_storage[type].mesh = new_mm;
}

Ref<ArrayMesh> DebugGeometryContainer::CreateMesh(Mesh::PrimitiveType type, const PackedVector3Array &vertices, const PackedInt32Array &indices, const PackedColorArray &colors) {
	Ref<ArrayMesh> mesh;
	mesh.instantiate();
	Array a;
	a.resize((int)ArrayMesh::ArrayType::ARRAY_MAX);

	a[(int)ArrayMesh::ArrayType::ARRAY_VERTEX] = vertices;
	if (indices.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_INDEX] = indices;
	if (colors.size())
		a[(int)ArrayMesh::ArrayType::ARRAY_COLOR] = colors;

	mesh->add_surface_from_arrays(type, a);

	return mesh;
}

void DebugGeometryContainer::set_world(Node *new_world) {
	scene_world_node = new_world;
	RenderingServer *rs = RS();
	RID scenario;
	Viewport *viewport = Object::cast_to<Viewport>(new_world);
	if (viewport) {
		scenario = viewport->get_world_3d()->get_scenario();
	} else {
		scenario = new_world->get_viewport()->get_world_3d()->get_scenario();
	}

	for (auto &s : multi_mesh_storage) {
		rs->instance_set_scenario(s.instance, scenario);
	}
	rs->instance_set_scenario(immediate_mesh_storage.instance, scenario);
}

Node *DebugGeometryContainer::get_world() {
	return scene_world_node;
}

void DebugGeometryContainer::update_geometry(double delta) {
	LOCK_GUARD(datalock);

	// Don't clear geometry if freezed
	if (owner->get_config_3d()->is_freeze_3d_render())
		return;

	immediate_mesh_storage.mesh->clear_surfaces();

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		for (auto &item : multi_mesh_storage) {
			item.mesh->set_visible_instance_count(0);
		}
		geometry_pool.reset_counter(delta);
		geometry_pool.reset_visible_objects();
		return;
	}

	// Update render layers
	if (render_layers != owner->get_config_3d()->get_geometry_render_layers()) {
		set_render_layer_mask(owner->get_config_3d()->get_geometry_render_layers());
	}

	// TODO: try to get all active cameras inside scene to properly calculate visibilty for SubViewports

	// Get camera frustum
	Camera3D *vp_cam = owner->get_root_node()->get_viewport()->get_camera_3d();
	if (IS_EDITOR_HINT()) {
		auto s_root = SCENE_TREE()->get_edited_scene_root();
		if (s_root) {
			vp_cam = s_root->get_viewport()->get_camera_3d();
		}
	}

	// Collect frustums and camera positions
	std::vector<std::vector<Plane> > frustum_planes;
	std::vector<Vector3> cameras_positions;
	{
		std::vector<SubViewport *> editor_viewports = owner->get_custom_editor_viewports();
		std::vector<Array> frustum_arrays;

		frustum_arrays.reserve(1);
		cameras_positions.reserve(1);
		if ((owner->get_config_3d()->is_force_use_camera_from_scene() || (!editor_viewports.size() && !owner->get_custom_viewport())) && vp_cam) {
			frustum_arrays.push_back(vp_cam->get_frustum());
			cameras_positions.push_back(vp_cam->get_position());
		} else if (owner->get_custom_viewport()) {
			auto c = owner->get_custom_viewport()->get_camera_3d();
			frustum_arrays.push_back(c->get_frustum());
			cameras_positions.push_back(c->get_position());
		} else if (editor_viewports.size() > 0) {
			for (auto vp : editor_viewports) {
				if (vp->get_update_mode() == SubViewport::UpdateMode::UPDATE_ALWAYS) {
					auto c = vp->get_camera_3d();
					frustum_arrays.push_back(c->get_frustum());
					cameras_positions.push_back(c->get_position());
				}
			}
		}

		// Convert frustum to vector
		if (owner->get_config_3d()->is_use_frustum_culling() && frustum_arrays.size()) {
			frustum_planes.reserve(frustum_arrays.size());

			for (auto &arr : frustum_arrays) {
				if (arr.size() == 6) {
					std::vector<Plane> a(6);
					for (int i = 0; i < arr.size(); i++)
						a[i] = (Plane)arr[i];
					frustum_planes.push_back(a);
				}
			}
		}
	}

	// Update visibility
	geometry_pool.update_visibility(
			frustum_planes,
			GeometryPoolDistanceCullingData(owner->get_config_3d()->get_cull_by_distance(), cameras_positions));

	// Debug bounds of instances and lines
	if (owner->get_config_3d()->is_visible_instance_bounds()) {
		std::vector<std::pair<Vector3, real_t> > new_instances;

		auto draw_instance_spheres = [&new_instances](DelayedRendererInstance *o) {
			if (!o->is_visible || o->is_expired())
				return;
			new_instances.push_back({ o->bounds.position, o->bounds.Radius });
		};

		geometry_pool.for_each_instance(draw_instance_spheres);

		// Draw custom sphere for 1 frame
		for (auto &i : new_instances) {
			geometry_pool.add_or_update_instance(
					InstanceType::SPHERES_HD,
					0,
					Transform3D(Basis().scaled(Vector3_ONE * i.second * 2), i.first),
					Colors::debug_bounds,
					SphereBounds(i.first, i.second),
					[](auto d) { d->is_used_one_time = true; });
		}

		geometry_pool.for_each_line([this](DelayedRendererLine *o) {
			if (!o->is_visible || o->is_expired())
				return;

			Vector3 bottom, top, diag;
			MathUtils::get_diagonal_vectors(o->bounds.position, o->bounds.position + o->bounds.size, bottom, top, diag);

			geometry_pool.add_or_update_instance(
					InstanceType::CUBES,
					0,
					Transform3D(Basis().scaled(diag), bottom),
					Colors::debug_bounds,
					SphereBounds(bottom + diag * 0.5f, abs(diag.length() * 0.5f)),
					[](auto d) { d->is_used_one_time = true; });
		});
	}

	// Draw immediate lines
	geometry_pool.fill_lines_data(immediate_mesh_storage.mesh);

	// Update MultiMeshInstances
	std::array<Ref<MultiMesh> *, InstanceType::ALL> meshes;
	for (int i = 0; i < InstanceType::ALL; i++) {
		meshes[i] = &multi_mesh_storage[i].mesh;
	}

	geometry_pool.fill_instance_data(meshes);

	geometry_pool.scan_visible_instances();
	geometry_pool.update_expiration(delta);
	geometry_pool.reset_counter(delta);
}

Ref<DebugDrawStats> DebugGeometryContainer::get_render_stats() {
	LOCK_GUARD(datalock);
	return geometry_pool.get_stats();
}

void DebugGeometryContainer::set_render_layer_mask(int32_t layers) {
	LOCK_GUARD(datalock);
	RenderingServer *rs = RS();
	for (auto &mmi : multi_mesh_storage)
		rs->instance_set_layer_mask(mmi.instance, layers);

	rs->instance_set_layer_mask(immediate_mesh_storage.instance, layers);
	render_layers = layers;
}

int32_t DebugGeometryContainer::get_render_layer_mask() const {
	return render_layers;
}

void DebugGeometryContainer::create_arrow(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	LOCK_GUARD(datalock);
	Vector3 dir = (b - a);
	real_t size = (is_absolute_size ? arrow_size : dir.length() * arrow_size) * 2;
	Vector3 pos = b - dir.normalized() * size;

	const Vector3 UP = Vector3(0.0000000001f, 1, 0);
	Transform3D t;
	t.set_look_at(pos, b, UP);
	t.scale(Vector3_ONE * (size));
	t.origin = pos;

	geometry_pool.add_or_update_instance(
			InstanceType::ARROWHEADS,
			duration,
			t,
			color == Colors::empty_color ? Colors::light_green : color,
			SphereBounds(t.origin - t.basis.get_column(2) * 0.5f, MathUtils::ArrowRadiusForSphere * size));
}

#pragma region Draw Functions

void DebugGeometryContainer::clear_3d_objects() {
	for (auto &s : multi_mesh_storage) {
		s.mesh->set_instance_count(0);
	}
	immediate_mesh_storage.mesh->clear_surfaces();

	geometry_pool.clear_pool();
}

#pragma region 3D
#pragma region Spheres

void DebugGeometryContainer::draw_sphere(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration, const bool &hd) {
	Transform3D t(Basis(), position);
	t.basis.scale(Vector3_ONE * (radius * 2));

	draw_sphere_xf(t, color, duration, hd);
}

void DebugGeometryContainer::draw_sphere_xf(const Transform3D &transform, const Color &color, const real_t &duration, const bool &hd) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_instance(
			hd ? InstanceType::SPHERES_HD : InstanceType::SPHERES,
			duration,
			transform,
			color == Colors::empty_color ? Colors::chartreuse : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * 0.51f));
}

void DebugGeometryContainer::draw_sphere_hd(const Vector3 &position, const real_t &radius, const Color &color, const real_t &duration) {
	draw_sphere(position, radius, color, duration, true);
}

void DebugGeometryContainer::draw_sphere_hd_xf(const Transform3D &transform, const Color &color, const real_t &duration) {
	draw_sphere_xf(transform, color, duration, true);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugGeometryContainer::draw_cylinder(const Transform3D &transform, const Color &color, const real_t &duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_instance(
			InstanceType::CYLINDERS,
			duration,
			transform,
			color == Colors::empty_color ? Colors::forest_green : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CylinderRadiusForSphere));
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugGeometryContainer::draw_box(const Vector3 &position, const Vector3 &size, const Color &color, const bool &is_box_centered, const real_t &duration) {
	draw_box_xf(Transform3D(Basis().scaled(size), position), color, is_box_centered, duration);
}

void DebugGeometryContainer::draw_box_xf(const Transform3D &transform, const Color &color, const bool &is_box_centered, const real_t &duration) {
	LOCK_GUARD(datalock);

	/// It's possible to use less space to contain box, but this method works better in more cases
	SphereBounds sb(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CubeRadiusForSphere);

	if (!is_box_centered) {
		sb.position = transform.origin + transform.basis.get_scale() * 0.5f;
	}
	geometry_pool.add_or_update_instance(
			is_box_centered ? InstanceType::CUBES_CENTERED : InstanceType::CUBES,
			duration,
			transform,
			color == Colors::empty_color ? Colors::forest_green : color,
			sb);
}

void DebugGeometryContainer::draw_aabb(const AABB &aabb, const Color &color, const real_t &duration) {
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(aabb.position, aabb.position + aabb.size, bottom, top, diag);
	draw_box(bottom, diag, color, false, duration);
}

void DebugGeometryContainer::draw_aabb_ab(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(a, b, bottom, top, diag);
	draw_box(bottom, diag, color, false, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugGeometryContainer::draw_line_hit(const Vector3 &start, const Vector3 &end, const Vector3 &hit, const bool &is_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	LOCK_GUARD(datalock);
	if (is_hit) {
		geometry_pool.add_or_update_line(duration, { start, hit }, hit_color == Colors::empty_color ? owner->get_config_3d()->get_line_hit_color() : hit_color);
		geometry_pool.add_or_update_line(duration, { hit, end }, after_hit_color == Colors::empty_color ? owner->get_config_3d()->get_line_after_hit_color() : after_hit_color);

		geometry_pool.add_or_update_instance(
				InstanceType::BILLBOARD_SQUARES,
				duration,
				Transform3D(Basis().scaled(Vector3_ONE * hit_size), hit),
				hit_color == Colors::empty_color ? owner->get_config_3d()->get_line_hit_color() : hit_color,
				SphereBounds(hit, MathUtils::CubeRadiusForSphere * hit_size));
	} else {
		geometry_pool.add_or_update_line(duration, { start, end }, hit_color == Colors::empty_color ? owner->get_config_3d()->get_line_hit_color() : hit_color);
	}
}

void DebugGeometryContainer::draw_line_hit_offset(const Vector3 &start, const Vector3 &end, const bool &is_hit, const real_t &unit_offset_of_hit, const real_t &hit_size, const Color &hit_color, const Color &after_hit_color, const real_t &duration) {
	LOCK_GUARD(datalock);
	if (is_hit && unit_offset_of_hit >= 0 && unit_offset_of_hit <= 1) {
		draw_line_hit(start, end, ((end - start).normalized() * start.distance_to(end) * unit_offset_of_hit + start), is_hit, hit_size, hit_color, after_hit_color, duration);
	} else {
		draw_line_hit(start, end, {}, is_hit, hit_size, hit_color, after_hit_color, duration);
	}
}

#pragma region Normal

void DebugGeometryContainer::draw_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, { a, b }, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_lines(const PackedVector3Array &lines, const Color &color, const real_t &duration) {
	LOCK_GUARD(datalock);

	if (lines.size() % 2 != 0) {
		PRINT_ERROR("The size of the lines array must be even. " + String::num_int64(lines.size()) + " is not even.");
		return;
	}

	std::vector<Vector3> l(lines.size());
	{
		memcpy(l.data(), lines.ptr(), (size_t)lines.size() * sizeof(Vector3));
	}

	geometry_pool.add_or_update_line(duration, l, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_lines_c(const std::vector<Vector3> &lines, const Color &color, const real_t &duration) {
	LOCK_GUARD(datalock);

	if (lines.size() % 2 != 0) {
		PRINT_ERROR("The size of the lines array must be even. " + String::num_int64(lines.size()) + " is not even.");
		return;
	}

	geometry_pool.add_or_update_line(duration, lines, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, { origin, origin + direction * length }, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_line_path(const PackedVector3Array &path, const Color &color, const real_t &duration) {
	if (path.size() < 2) {
		PRINT_ERROR("Line path must contains at least 2 points. " + String::num_int64(path.size()) + " is not enough.");
		return;
	}

	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, GeometryGenerator::CreateLinesFromPath(path), color == Colors::empty_color ? Colors::light_green : color);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugGeometryContainer::draw_arrow(const Transform3D &transform, const Color &color, const real_t &duration) {
	LOCK_GUARD(datalock);

	geometry_pool.add_or_update_instance(
			InstanceType::ARROWHEADS,
			duration,
			transform,
			color == Colors::empty_color ? Colors::light_green : color,
			SphereBounds(transform.origin - transform.basis.get_column(2) * 0.5f, MathUtils::ArrowRadiusForSphere * MathUtils::get_max_basis_length(transform.basis)));
}

void DebugGeometryContainer::draw_arrow_line(const Vector3 &a, const Vector3 &b, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, { a, b }, color == Colors::empty_color ? Colors::light_green : color);

	create_arrow(a, b, color, arrow_size, is_absolute_size, duration);
}

void DebugGeometryContainer::draw_arrow_ray(const Vector3 &origin, const Vector3 &direction, const real_t &length, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	draw_arrow_line(origin, origin + direction * length, color, arrow_size, is_absolute_size, duration);
}

void DebugGeometryContainer::draw_arrow_path(const PackedVector3Array &path, const Color &color, const real_t &arrow_size, const bool &is_absolute_size, const real_t &duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, GeometryGenerator::CreateLinesFromPath(path), color == Colors::empty_color ? Colors::light_green : color);

	for (int i = 0; i < path.size() - 1; i++) {
		create_arrow(path[i], path[i + 1], color, arrow_size, is_absolute_size, duration);
	}
}

#pragma endregion // Arrows
#pragma region Points

void DebugGeometryContainer::draw_point_path(const PackedVector3Array &path, const real_t &size, const Color &points_color, const Color &lines_color, const real_t &duration) {
	draw_points(path, size, points_color == Colors::empty_color ? Colors::red : points_color, duration);
	draw_line_path(path, lines_color == Colors::empty_color ? Colors::green : lines_color, duration);
}

#pragma endregion // Points
#pragma endregion // Lines

#pragma region Misc

void DebugGeometryContainer::draw_square(const Vector3 &position, const real_t &size, const Color &color, const real_t &duration) {
	Transform3D t(Basis(), position);
	t.basis.scale(Vector3_ONE * size);

	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_instance(
			InstanceType::BILLBOARD_SQUARES,
			duration,
			t,
			color == Colors::empty_color ? Colors::red : color,
			SphereBounds(position, MathUtils::CubeRadiusForSphere * size));
}

void DebugGeometryContainer::draw_points(const PackedVector3Array &points, const real_t &size, const Color &color, const real_t &duration) {
	for (int i = 0; i < points.size(); i++) {
		draw_square(points[i], size, color, duration);
	}
}

void DebugGeometryContainer::draw_position(const Transform3D &transform, const Color &color, const real_t &duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_instance(
			InstanceType::POSITIONS,
			duration,
			transform,
			color == Colors::empty_color ? Colors::crimson : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::AxisRadiusForSphere));
}

void DebugGeometryContainer::draw_gizmo(const Transform3D &transform, const Color &color, const bool &is_centered, const real_t &duration) {
	LOCK_GUARD(datalock);

	bool is_color_empty = color == Colors::empty_color;
#define COLOR(axis) is_color_empty ? Colors::axis_##axis : color
#define MINUS(axis) transform.origin - transform.basis.get_column(axis)
#define PLUS(axis) transform.origin + transform.basis.get_column(axis)

	if (is_centered) {
		draw_arrow_line(MINUS(0 /** 0.5f*/), PLUS(0 /** 0.5f*/), COLOR(x), 0.1f, true, duration);
		draw_arrow_line(MINUS(1 /** 0.5f*/), PLUS(1 /** 0.5f*/), COLOR(y), 0.1f, true, duration);
		draw_arrow_line(MINUS(2 /** 0.5f*/), PLUS(2 /** 0.5f*/), COLOR(z), 0.1f, true, duration);
	} else {
		draw_arrow_line(transform.origin, PLUS(0), COLOR(x), 0.15f, true, duration);
		draw_arrow_line(transform.origin, PLUS(1), COLOR(y), 0.15f, true, duration);
		draw_arrow_line(transform.origin, PLUS(2), COLOR(z), 0.15f, true, duration);
	}

#undef COLOR
#undef MINUS
#undef PLUS
}

void DebugGeometryContainer::draw_grid(const Vector3 &origin, const Vector3 &x_size, const Vector3 &y_size, const Vector2i &subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
	draw_grid_xf(Transform3D(Basis(x_size, y_size.cross(x_size).normalized(), y_size), origin),
			subdivision, color, is_centered, duration);
}

void DebugGeometryContainer::draw_grid_xf(const Transform3D &transform, const Vector2i &_subdivision, const Color &color, const bool &is_centered, const real_t &duration) {
#define MAX_SUBDIVISIONS 1024 * 1024
	ERR_FAIL_COND(_subdivision.x > MAX_SUBDIVISIONS);
	ERR_FAIL_COND(_subdivision.y > MAX_SUBDIVISIONS);
	LOCK_GUARD(datalock);

	Vector2i subdivision = _subdivision.abs();
	subdivision = Vector2i(Math::clamp(subdivision.x, 1, MAX_SUBDIVISIONS), Math::clamp(subdivision.y, 1, MAX_SUBDIVISIONS));
	Vector3 x_axis = transform.basis.get_column(0);
	Vector3 z_axis = transform.basis.get_column(2);
	Vector3 x_d = x_axis / (real_t)subdivision.x;
	Vector3 z_d = z_axis / (real_t)subdivision.y;

	Vector3 origin = is_centered ?
							 transform.origin - x_d * (real_t)subdivision.x * 0.5 - z_d * (real_t)subdivision.y * 0.5 :
							 transform.origin;

	std::vector<Vector3> lines;
	for (int x = 0; x < subdivision.x + 1; x++) {
		lines.push_back(origin + x_d * (real_t)x);
		lines.push_back(origin + x_d * (real_t)x + z_axis);
	}

	for (int y = 0; y < subdivision.y + 1; y++) {
		lines.push_back(origin + z_d * (real_t)y);
		lines.push_back(origin + z_d * (real_t)y + x_axis);
	}

	draw_lines_c(lines, color == Colors::empty_color ? Colors::white : color, duration);
}

#pragma region Camera Frustum

void DebugGeometryContainer::draw_camera_frustum(const Camera3D *camera, const Color &color, const real_t &duration) {
	ERR_FAIL_COND(!camera);
	draw_camera_frustum_planes(camera->get_frustum(), color, duration);
}

void DebugGeometryContainer::draw_camera_frustum_planes(const Array &camera_frustum, const Color &color, const real_t &duration) {
	std::array<Plane, 6> planes = { Plane() };

	if (camera_frustum.size() == 6) {
		for (int i = 0; i < camera_frustum.size(); i++) {
			planes[i] = camera_frustum[i];
		}
	} else {
		PRINT_ERROR("Camera frustum requires an array of 6 planes. Recieved " + String::num_int64(camera_frustum.size()));
	}

	draw_camera_frustum_planes_c(planes, color, duration);
}

void DebugGeometryContainer::draw_camera_frustum_planes_c(const std::array<Plane, 6> &planes, const Color &color, const real_t &duration) {
	auto lines = GeometryGenerator::CreateCameraFrustumLines(planes);

	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, lines, color == Colors::empty_color ? Colors::red : color);
}

#pragma endregion // Camera Frustum

#pragma endregion // Misc
#pragma endregion // 3D
#pragma endregion // Draw Functions
