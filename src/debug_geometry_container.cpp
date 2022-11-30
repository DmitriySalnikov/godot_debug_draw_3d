#include "debug_geometry_container.h"
#include "debug_draw.h"
#include "utils.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/texture.hpp>

using namespace godot;

DebugGeometryContainer::DebugGeometryContainer(class DebugDraw3D *root) {
	owner = root;

	// Create wireframe mesh drawer
	_immediate_mesh.instantiate();
	_immediate_instance = memnew(MeshInstance3D);

	{
		_immediate_instance->set_mesh(_immediate_mesh);

		_immediate_instance->set_name(TEXT(_immediate_mesh));
		_immediate_instance->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
		_immediate_instance->set_gi_mode(GeometryInstance3D::GI_MODE_DISABLED);

		Ref<StandardMaterial3D> mat;
		mat.instantiate();
		mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
		mat->set_flag(StandardMaterial3D::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

		_immediate_instance->set_material_override(mat);
	}

	root->add_child(_immediate_instance);

	// Create node with material and MultiMesh. Add to tree. Create array of instances
	MultiMeshInstance3D *mmi_cubes = CreateMMI(root, InstanceType::CUBES, TEXT(mmi_cubes));
	MultiMeshInstance3D *mmi_cubes_centered = CreateMMI(root, InstanceType::CUBES_CENTERED, TEXT(mmi_cubes_centered));
	MultiMeshInstance3D *mmi_arrowheads = CreateMMI(root, InstanceType::ARROWHEADS, TEXT(mmi_arrowheads));
	MultiMeshInstance3D *mmi_billboard_squares = CreateMMI(root, InstanceType::BILLBOARD_SQUARES, TEXT(mmi_billboard_squares));
	MultiMeshInstance3D *mmi_positions = CreateMMI(root, InstanceType::POSITIONS, TEXT(mmi_positions));
	MultiMeshInstance3D *mmi_spheres = CreateMMI(root, InstanceType::SPHERES, TEXT(mmi_spheres));
	MultiMeshInstance3D *mmi_spheres_hd = CreateMMI(root, InstanceType::SPHERES_HD, TEXT(mmi_spheres_hd));
	MultiMeshInstance3D *mmi_cylinders = CreateMMI(root, InstanceType::CYLINDERS, TEXT(mmi_cylinders));

	set_render_layer_mask(1);

	// Customize parameters
	Ref<StandardMaterial3D> sq_mat = mmi_billboard_squares->get_material_override();
	if (sq_mat.is_valid()) {
		sq_mat->set_billboard_mode(StandardMaterial3D::BillboardMode::BILLBOARD_ENABLED);
		sq_mat->set_flag(StandardMaterial3D::Flags::FLAG_BILLBOARD_KEEP_SCALE, true);
	}

	// Create Meshes

	mmi_cubes->get_multimesh()->set_mesh(CreateMesh(
			Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CubeVertices, C_ARR_SIZE(GeometryGenerator::CubeVertices), GeometryGenerator::CubeIndices, C_ARR_SIZE(GeometryGenerator::CubeIndices)));

	mmi_cubes_centered->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CenteredCubeVertices, C_ARR_SIZE(GeometryGenerator::CenteredCubeVertices), GeometryGenerator::CubeIndices, C_ARR_SIZE(GeometryGenerator::CubeIndices)));

	mmi_arrowheads->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::ArrowheadVertices, C_ARR_SIZE(GeometryGenerator::ArrowheadVertices), GeometryGenerator::ArrowheadIndices, C_ARR_SIZE(GeometryGenerator::ArrowheadIndices)));

	mmi_billboard_squares->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES,
			GeometryGenerator::CenteredSquareVertices, C_ARR_SIZE(GeometryGenerator::CenteredSquareVertices), GeometryGenerator::SquareIndices, C_ARR_SIZE(GeometryGenerator::SquareIndices)));

	mmi_positions->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::PositionVertices, C_ARR_SIZE(GeometryGenerator::PositionVertices), GeometryGenerator::PositionIndices, C_ARR_SIZE(GeometryGenerator::PositionIndices)));

	mmi_spheres->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CreateSphereLines(8, 8, 0.5f, Vector3_ZERO)));

	mmi_spheres_hd->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CreateSphereLines(16, 16, 0.5f, Vector3_ZERO)));

	mmi_cylinders->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CreateCylinderLines(52, 1, 1, Vector3_ZERO, 4)));
}

DebugGeometryContainer::~DebugGeometryContainer() {
	LOCK_GUARD(datalock);

	geometry_pool.clear_pool();
	geometry_pool.clear_pool();
}

MultiMeshInstance3D *DebugGeometryContainer::CreateMMI(Node *root, InstanceType type, String name) {
	MultiMeshInstance3D *mmi = memnew(MultiMeshInstance3D);
	mmi->set_name(name);
	mmi->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
	mmi->set_gi_mode(GeometryInstance3D::GI_MODE_DISABLED);

	Ref<StandardMaterial3D> new_mat;
	new_mat.instantiate();
	new_mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	new_mat->set_flag(StandardMaterial3D::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

	mmi->set_material_override(new_mat);

	Ref<MultiMesh> new_mm;
	new_mm.instantiate();
	new_mm->set_name(String::num_int64(type));

	new_mm->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);
	new_mm->set_use_colors(true);
	new_mm->set_transform_format(MultiMesh::TRANSFORM_3D);
	new_mm->set_use_custom_data(false);

	mmi->set_multimesh(new_mm);

	root->add_child(mmi);
	multi_mesh_instances[type] = mmi;
	return mmi;
}

Ref<ArrayMesh> DebugGeometryContainer::CreateMesh(Mesh::PrimitiveType type, const Vector3 *vertices, const size_t vertices_size, const int *indices, const size_t indices_size, const Color *colors, const size_t colors_size) {
	PackedVector3Array pool_vertices = Utils::convert_to_pool_array<PackedVector3Array>(vertices, vertices_size);
	PackedInt32Array pool_indices = Utils::convert_to_pool_array<PackedInt32Array>(indices, indices_size);
	PackedColorArray pool_colors = Utils::convert_to_pool_array<PackedColorArray>(colors, colors_size);
	return CreateMesh(type, pool_vertices, pool_indices, pool_colors);
}

Ref<ArrayMesh> DebugGeometryContainer::CreateMesh(Mesh::PrimitiveType type, const std::vector<Vector3> vertices, const std::vector<int> indices, const std::vector<Color> colors) {
	PackedVector3Array pool_vertices = Utils::convert_to_pool_array<PackedVector3Array>(vertices);
	PackedInt32Array pool_indices = Utils::convert_to_pool_array<PackedInt32Array>(indices);
	PackedColorArray pool_colors = Utils::convert_to_pool_array<PackedColorArray>(colors);
	return CreateMesh(type, pool_vertices, pool_indices, pool_colors);
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

void DebugGeometryContainer::update_geometry(real_t delta) {
	LOCK_GUARD(datalock);

	// Don't clear geometry if freezed
	if (owner->is_freeze_3d_render())
		return;

	_immediate_mesh->clear_surfaces();

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		for (auto item : multi_mesh_instances) {
			item->get_multimesh()->set_visible_instance_count(0);
		}
		geometry_pool.reset_counter(delta);
		geometry_pool.reset_visible_objects();
		return;
	}

	// TODO try to get all active cameras inside scene to properly calculate visibilty

	// Get camera frustum
	Camera3D *vp_cam = owner->get_viewport()->get_camera_3d();

	std::vector<std::vector<Plane> > f;
	if (owner->is_use_frustum_culling()) {
		std::vector<Viewport *> editor_viewports = owner->get_custom_editor_viewport();
		std::vector<Array> frustum_arrays;

		frustum_arrays.reserve(1);
		if ((owner->is_force_use_camera_from_scene() || (!editor_viewports.size() && !owner->get_custom_viewport())) && vp_cam) {
			frustum_arrays.push_back(vp_cam->get_frustum());
		} else if (owner->get_custom_viewport()) {
			frustum_arrays.push_back(owner->get_custom_viewport()->get_camera_3d()->get_frustum());
		} else if (editor_viewports.size() > 0) {
			for (auto vp : editor_viewports) {
				// TODO idk where is update mode. Mb UPDATE_ALWAYS is default now.
				//if (vp->get_update_mode() == Viewport::UpdateMode::UPDATE_ALWAYS) {
					frustum_arrays.push_back(vp->get_camera_3d()->get_frustum());
				//}
			}
		} else {
			PRINT_WARNING("No camera found to perform frustum culling for DebugDraw");
		}

		// Convert frustum to vector
		f.reserve(1);

		for (auto &arr : frustum_arrays) {
			if (arr.size() == 6) {
				std::vector<Plane> a(6);
				for (int i = 0; i < arr.size(); i++)
					a[i] = (Plane)arr[i];
				f.push_back(a);
			}
		}
	}

	// Update visibility
	geometry_pool.update_visibility(f);

	// Debug bounds of instances and lines
	if (owner->is_visible_instance_bounds()) {
		// std::vector<Plane> new_instances(geometry_pool.get_used_instances());
		std::vector<std::pair<Vector3, real_t> > new_instances;

		auto draw_instance_spheres = [&new_instances](DelayedRendererInstance *o) {
			if (!o->is_visible || o->is_expired())
				return;
			new_instances.push_back({ o->bounds.Position, o->bounds.Radius });
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
	geometry_pool.fill_lines_data(_immediate_mesh);

	// Update MultiMeshInstances
	for (int i = 0; i < InstanceType::ALL; i++) {
		Ref<MultiMesh> mms = multi_mesh_instances[i]->get_multimesh();
		mms->set_visible_instance_count(-1);

		auto a = geometry_pool.get_raw_data((InstanceType)i);
		mms->set_instance_count(a.size() / INSTANCE_DATA_FLOAT_COUNT);
		if (a.size()) {
			mms->set_buffer(a);
		}
	}

	geometry_pool.scan_visible_instances();
	geometry_pool.update_expiration(delta);
	geometry_pool.reset_counter(delta);
}

#include <godot_cpp/variant/dictionary.hpp>

Dictionary DebugGeometryContainer::get_render_stats() {
	LOCK_GUARD(datalock);
	
	return Utils::make_dict(
			"instances", (int64_t)geometry_pool.get_used_instances_total(),
			"visible_instances", (int64_t)geometry_pool.get_visible_instances(),
			"wireframes", (int64_t)geometry_pool.get_used_lines_total(),
			"visible_wireframes", (int64_t)geometry_pool.get_visible_lines(),
			"total", (int64_t)geometry_pool.get_used_instances_total() + geometry_pool.get_used_lines_total(),
			"total_visible", (int64_t)geometry_pool.get_visible_instances() + geometry_pool.get_visible_lines());
}

void DebugGeometryContainer::set_render_layer_mask(int64_t layers) {
	for (auto &mmi : multi_mesh_instances)
		mmi->set_layer_mask(layers);

	_immediate_instance->set_layer_mask(layers);
}

void DebugGeometryContainer::create_arrow(Vector3 a, Vector3 b, Color color, real_t arrow_size, bool absolute_size, real_t duration) {
	LOCK_GUARD(datalock);
	Vector3 dir = (b - a);
	real_t size = (absolute_size ? arrow_size : dir.length() * arrow_size) * 2;
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
			SphereBounds(t.origin - t.basis.rows[2] * 0.5f, MathUtils::ArrowRadiusForSphere * size));
}

#pragma region Draw Functions

void DebugGeometryContainer::clear_3d_objects() {
	for (auto s : multi_mesh_instances) {
		s->get_multimesh()->set_instance_count(0);
	}
	_immediate_mesh->clear_surfaces();

	geometry_pool.clear_pool();
}

#pragma region 3D
#pragma region Spheres

void DebugGeometryContainer::draw_sphere(Vector3 position, real_t radius, Color color, real_t duration, bool hd) {
	Transform3D t(Basis(), position);
	t.basis.scale(Vector3_ONE * (radius * 2));

	draw_sphere_xf(t, color, duration, hd);
}

void DebugGeometryContainer::draw_sphere_xf(Transform3D transform, Color color, real_t duration, bool hd) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_instance(
			hd ? InstanceType::SPHERES_HD : InstanceType::SPHERES,
			duration,
			transform,
			color == Colors::empty_color ? Colors::chartreuse : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * 0.51f));
}

void DebugGeometryContainer::draw_sphere_hd(Vector3 position, real_t radius, Color color, real_t duration) {
	draw_sphere(position, radius, color, duration, true);
}

void DebugGeometryContainer::draw_sphere_hd_xf(Transform3D transform, Color color, real_t duration) {
	draw_sphere_xf(transform, color, duration, true);
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugGeometryContainer::draw_cylinder(Transform3D transform, Color color, real_t duration) {
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

void DebugGeometryContainer::draw_box(Vector3 position, Vector3 size, Color color, bool is_box_centered, real_t duration) {
	draw_box_xf(Transform3D(Basis().scaled(size), position), color, is_box_centered, duration);
}

void DebugGeometryContainer::draw_box_xf(Transform3D transform, Color color, bool is_box_centered, real_t duration) {
	LOCK_GUARD(datalock);

	/// It's possible to use less space to contain box, but this method works better in more cases
	SphereBounds sb(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::CubeRadiusForSphere);

	if (!is_box_centered) {
		sb.Position = transform.origin + transform.basis.get_scale() * 0.5f;
	}
	geometry_pool.add_or_update_instance(
			is_box_centered ? InstanceType::CUBES_CENTERED : InstanceType::CUBES,
			duration,
			transform,
			color == Colors::empty_color ? Colors::forest_green : color,
			sb);
}

void DebugGeometryContainer::draw_aabb(AABB aabb, Color color, real_t duration) {
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(aabb.position, aabb.position + aabb.size, bottom, top, diag);
	draw_box(bottom, diag, color, false, duration);
}

void DebugGeometryContainer::draw_aabb_ab(Vector3 a, Vector3 b, Color color, real_t duration) {
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(a, b, bottom, top, diag);
	draw_box(bottom, diag, color, false, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugGeometryContainer::draw_line_hit(Vector3 start, Vector3 end, Vector3 hit, bool is_hit, real_t hit_size, Color hit_color, Color after_hit_color, real_t duration) {
	LOCK_GUARD(datalock);
	if (is_hit) {
		geometry_pool.add_or_update_line(duration, { start, hit }, hit_color == Colors::empty_color ? owner->get_line_hit_color() : hit_color);
		geometry_pool.add_or_update_line(duration, { hit, end }, after_hit_color == Colors::empty_color ? owner->get_line_after_hit_color() : after_hit_color);

		geometry_pool.add_or_update_instance(
				InstanceType::BILLBOARD_SQUARES,
				duration,
				Transform3D(Basis().scaled(Vector3_ONE * hit_size), hit),
				hit_color == Colors::empty_color ? owner->get_line_hit_color() : hit_color,
				SphereBounds(hit, MathUtils::CubeRadiusForSphere * hit_size));
	} else {
		geometry_pool.add_or_update_line(duration, { start, end }, hit_color == Colors::empty_color ? owner->get_line_hit_color() : hit_color);
	}
}

void DebugGeometryContainer::draw_line_hit_offset(Vector3 start, Vector3 end, bool is_hit, real_t unit_offset_of_hit, real_t hit_size, Color hit_color, Color after_hit_color, real_t duration) {
	LOCK_GUARD(datalock);
	if (is_hit && unit_offset_of_hit >= 0 && unit_offset_of_hit <= 1) {
		draw_line_hit(start, end, ((end - start).normalized() * start.distance_to(end) * unit_offset_of_hit + start), is_hit, hit_size, hit_color, after_hit_color, duration);
	} else {
		draw_line_hit(start, end, {}, is_hit, hit_size, hit_color, after_hit_color, duration);
	}
}

#pragma region Normal

void DebugGeometryContainer::draw_line(Vector3 a, Vector3 b, Color color, real_t duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, { a, b }, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_lines(PackedVector3Array lines, Color color, real_t duration) {
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

void DebugGeometryContainer::draw_lines_c(std::vector<Vector3> &lines, Color color, real_t duration) {
	LOCK_GUARD(datalock);

	if (lines.size() % 2 != 0) {
		PRINT_ERROR("The size of the lines array must be even. " + String::num_int64(lines.size()) + " is not even.");
		return;
	}

	geometry_pool.add_or_update_line(duration, lines, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_ray(Vector3 origin, Vector3 direction, real_t length, Color color, real_t duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, { origin, origin + direction * length }, color == Colors::empty_color ? Colors::red : color);
}

void DebugGeometryContainer::draw_line_path(PackedVector3Array path, Color color, real_t duration) {
	if (path.size() < 2) {
		PRINT_ERROR("Line path must contains at least 2 points. " + String::num_int64(path.size()) + " is not enough.");
		return;
	}

	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, GeometryGenerator::CreateLinesFromPath(path), color == Colors::empty_color ? Colors::light_green : color);
}

#pragma endregion // Normal
#pragma region Arrows

void DebugGeometryContainer::draw_arrow(Transform3D transform, Color color, real_t duration) {
	LOCK_GUARD(datalock);

	geometry_pool.add_or_update_instance(
			InstanceType::ARROWHEADS,
			duration,
			transform,
			color == Colors::empty_color ? Colors::light_green : color,
			SphereBounds(transform.origin - transform.basis.rows[2] * 0.5f, MathUtils::ArrowRadiusForSphere * MathUtils::get_max_basis_length(transform.basis)));
}

void DebugGeometryContainer::draw_arrow_line(Vector3 a, Vector3 b, Color color, real_t arrow_size, bool is_absolute_size, real_t duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, { a, b }, color == Colors::empty_color ? Colors::light_green : color);

	create_arrow(a, b, color, arrow_size, is_absolute_size, duration);
}

void DebugGeometryContainer::draw_arrow_ray(Vector3 origin, Vector3 direction, real_t length, Color color, real_t arrow_size, bool is_absolute_size, real_t duration) {
	draw_arrow_line(origin, origin + direction * length, color, arrow_size, is_absolute_size, duration);
}

void DebugGeometryContainer::draw_arrow_path(PackedVector3Array path, Color color, real_t arrow_size, bool is_absolute_size, real_t duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, GeometryGenerator::CreateLinesFromPath(path), color == Colors::empty_color ? Colors::light_green : color);

	for (int i = 0; i < path.size() - 1; i++) {
		create_arrow(path[i], path[i + 1], color, arrow_size, is_absolute_size, duration);
	}
}

#pragma endregion // Arrows
#pragma region Points

void DebugGeometryContainer::draw_point_path(PackedVector3Array path, real_t size, Color points_color, Color lines_color, real_t duration) {
	draw_points(path, size, points_color == Colors::empty_color ? Colors::red : points_color, duration);
	draw_line_path(path, lines_color == Colors::empty_color ? Colors::green : lines_color, duration);
}

#pragma endregion // Points
#pragma endregion // Lines

#pragma region Misc

void DebugGeometryContainer::draw_square(Vector3 position, real_t size, Color color, real_t duration) {
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

void DebugGeometryContainer::draw_points(PackedVector3Array points, real_t size, Color color, real_t duration) {
	for (int i = 0; i < points.size(); i++) {
		draw_square(points[i], size, color, duration);
	}
}

#pragma region Camera Frustum

void DebugGeometryContainer::draw_camera_frustum(Camera3D *camera, Color color, real_t duration) {
	draw_camera_frustum_planes(camera->get_frustum(), color, duration);
}

void DebugGeometryContainer::draw_camera_frustum_planes(Array camera_frustum, Color color, real_t duration) {
	Plane planes[6] = { Plane() };

	if (camera_frustum.size() == 6) {
		for (int i = 0; i < camera_frustum.size(); i++) {
			planes[i] = camera_frustum[i];
		}
	} else {
		PRINT_ERROR("Camera frustum requires an array of 6 planes. Recieved " + String::num_int64(camera_frustum.size()));
	}

	draw_camera_frustum_planes_c(planes, color, duration);
}

void DebugGeometryContainer::draw_camera_frustum_planes_c(Plane planes[6], Color color, real_t duration) {
	auto lines = GeometryGenerator::CreateCameraFrustumLines(planes);

	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_line(duration, lines, color == Colors::empty_color ? Colors::red : color);
}

#pragma endregion // Camera Frustum

void DebugGeometryContainer::draw_position(Transform3D transform, Color color, real_t duration) {
	LOCK_GUARD(datalock);
	geometry_pool.add_or_update_instance(
			InstanceType::POSITIONS,
			duration,
			transform,
			color == Colors::empty_color ? Colors::crimson : color,
			SphereBounds(transform.origin, MathUtils::get_max_basis_length(transform.basis) * MathUtils::AxisRadiusForSphere));
}

void DebugGeometryContainer::draw_gizmo(Transform3D transform, Color color, bool is_centered, real_t duration) {
	LOCK_GUARD(datalock);

	bool is_color_empty = color == Colors::empty_color;
#define COLOR(axis) is_color_empty ? Colors::axis_##axis : color
#define MINUS(axis) transform.origin - transform.basis axis
#define PLUS(axis) transform.origin + transform.basis axis

	if (is_centered) {
		draw_arrow_line(MINUS([0] /** 0.5f*/), PLUS([0] /** 0.5f*/), COLOR(x), 0.1f, true, duration);
		draw_arrow_line(MINUS([1] /** 0.5f*/), PLUS([1] /** 0.5f*/), COLOR(y), 0.1f, true, duration);
		draw_arrow_line(MINUS([2] /** 0.5f*/), PLUS([2] /** 0.5f*/), COLOR(z), 0.1f, true, duration);
	} else {
		draw_arrow_line(transform.origin, PLUS([0]), COLOR(x), 0.15f, true, duration);
		draw_arrow_line(transform.origin, PLUS([1]), COLOR(y), 0.15f, true, duration);
		draw_arrow_line(transform.origin, PLUS([2]), COLOR(z), 0.15f, true, duration);
	}

#undef COLOR
#undef MINUS
#undef PLUS
}

void DebugGeometryContainer::draw_grid(Vector3 origin, Vector3 x_size, Vector3 y_size, Vector2i subdivision, Color color, bool is_centered, real_t duration) {
	Basis b;
	// TODO need testing
	b.set_column(0, x_size);
	b.set_column(1, y_size.cross(x_size).normalized());
	b.set_column(2, y_size);
	draw_grid_xf(Transform3D(b, origin), subdivision, color, is_centered, duration);
}

void DebugGeometryContainer::draw_grid_xf(Transform3D transform, Vector2i subdivision, Color color, bool is_centered, real_t duration) {
	LOCK_GUARD(datalock);

	subdivision = subdivision.abs();
	subdivision = Vector2((real_t)Math::clamp(subdivision.x, 1, INT32_MAX), (real_t)Math::clamp(subdivision.y, 1, INT32_MAX));
	Vector3 x_d = transform.basis.rows[0] / subdivision.x;
	Vector3 z_d = transform.basis.rows[2] / subdivision.y;

	Vector3 origin = is_centered ?
							 transform.origin - x_d * subdivision.x * 0.5 - z_d * subdivision.y * 0.5 :
							 transform.origin;

	std::vector<Vector3> lines;
	for (int x = 0; x < subdivision.x + 1; x++) {
		lines.push_back(origin + x_d * (real_t)x);
		lines.push_back(origin + x_d * (real_t)x + transform.basis.rows[2]);
	}

	for (int y = 0; y < subdivision.y + 1; y++) {
		lines.push_back(origin + z_d * (real_t)y);
		lines.push_back(origin + z_d * (real_t)y + transform.basis.rows[0]);
	}

	draw_lines_c(lines, color == Colors::empty_color ? Colors::white : color, duration);
}

#pragma endregion // Misc
#pragma endregion // 3D
#pragma endregion // Draw Functions
