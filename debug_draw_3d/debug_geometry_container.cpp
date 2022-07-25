#include "debug_geometry_container.h"
#include "debug_draw.h"

#include <Engine.hpp>
#include <Texture.hpp>

using namespace godot;

#define GET_INSTANCES(type) mmi_with_values[type].second

DebugGeometryContainer::DebugGeometryContainer(class DebugDraw3D *root) {
	owner = root;

	// Create wireframe mesh drawer
	_immediateGeometry = ImmediateGeometry::_new();
	{
		_immediateGeometry->set_name(TEXT(_immediateGeometry));
		_immediateGeometry->set_cast_shadows_setting(GeometryInstance::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
		_immediateGeometry->set_flag(GeometryInstance::Flags::FLAG_USE_BAKED_LIGHT, false);

		Ref<SpatialMaterial> mat;
		mat.instance();
		mat->set_flag(SpatialMaterial::Flags::FLAG_UNSHADED, true);
		mat->set_flag(SpatialMaterial::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

		_immediateGeometry->set_material_override(mat);
	}

	root->add_child(_immediateGeometry);

	// Create node with material and MultiMesh. Add to tree. Create array of instances
	MultiMeshInstance *mmi_cubes = CreateMMI(root, InstanceType::CUBES, TEXT(mmi_cubes));
	MultiMeshInstance *mmi_cubes_centered = CreateMMI(root, InstanceType::CUBES_CENTERED, TEXT(mmi_cubes_centered));
	MultiMeshInstance *mmi_arrowheads = CreateMMI(root, InstanceType::ARROWHEADS, TEXT(mmi_arrowheads));
	MultiMeshInstance *mmi_billboard_squares = CreateMMI(root, InstanceType::BILLBOARD_SQUARES, TEXT(mmi_billboard_squares));
	MultiMeshInstance *mmi_positions = CreateMMI(root, InstanceType::POSITIONS, TEXT(mmi_positions));
	MultiMeshInstance *mmi_spheres = CreateMMI(root, InstanceType::SPHERES, TEXT(mmi_spheres));
	MultiMeshInstance *mmi_cylinders = CreateMMI(root, InstanceType::CYLINDERS, TEXT(mmi_cylinders));

	// Customize parameters
	Ref<SpatialMaterial> sq_mat = mmi_billboard_squares->get_material_override();
	if (sq_mat.is_valid()) {
		sq_mat->set_billboard_mode(SpatialMaterial::BillboardMode::BILLBOARD_ENABLED);
		sq_mat->set_flag(SpatialMaterial::Flags::FLAG_BILLBOARD_KEEP_SCALE, true);
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
			GeometryGenerator::CreateSphereLines(6, 6, 0.5f, Vector3::ZERO)));

	mmi_cylinders->get_multimesh()->set_mesh(CreateMesh(Mesh::PrimitiveType::PRIMITIVE_LINES,
			GeometryGenerator::CreateCylinderLines(52, 0.5f, 1, Vector3::ZERO, 4)));
}

DebugGeometryContainer::~DebugGeometryContainer() {
	LOCK_GUARD_REC(datalock);

	pool_wireframe_renderers.clear_pool();
	pool_instance_renderers.clear_pool();
}

MultiMeshInstance *DebugGeometryContainer::CreateMMI(Node *root, InstanceType type, String name) {
	MultiMeshInstance *mmi = MultiMeshInstance::_new();
	mmi->set_name(name);
	mmi->set_cast_shadows_setting(GeometryInstance::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
	mmi->set_flag(GeometryInstance::Flags::FLAG_USE_BAKED_LIGHT, false);

	Ref<SpatialMaterial> new_mat;
	new_mat.instance();
	new_mat->set_flag(SpatialMaterial::Flags::FLAG_UNSHADED, true);
	new_mat->set_flag(SpatialMaterial::Flags::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

	mmi->set_material_override(new_mat);

	Ref<MultiMesh> new_mm;
	new_mm.instance();
	new_mm->set_name(String::num_int64(type));

	new_mm->set_color_format(MultiMesh::ColorFormat::COLOR_FLOAT);
	new_mm->set_custom_data_format(MultiMesh::CustomDataFormat::CUSTOM_DATA_NONE);
	new_mm->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);

	mmi->set_multimesh(new_mm);

	root->add_child(mmi);
	mmi_with_values[type] = mmi;
	return mmi;
}

Ref<ArrayMesh> DebugGeometryContainer::CreateMesh(Mesh::PrimitiveType type, const Vector3 *vertices, const size_t vertices_size, const int *indices, const size_t indices_size, const Color *colors, const size_t colors_size) {
	PoolVector3Array pool_vertices;
	PoolIntArray pool_indices;
	PoolColorArray pool_colors;

	if (vertices && vertices_size > 0) {
		pool_vertices.resize((int)vertices_size);
		memcpy(pool_vertices.write().ptr(), vertices, sizeof(Vector3) * vertices_size);
	}

	if (indices && indices_size > 0) {
		pool_indices.resize((int)indices_size);
		memcpy(pool_indices.write().ptr(), indices, sizeof(int) * indices_size);
	}

	if (colors && colors_size > 0) {
		pool_colors.resize((int)colors_size);
		memcpy(pool_colors.write().ptr(), colors, sizeof(Color) * colors_size);
	}

	return CreateMesh(type, pool_vertices, pool_indices, pool_colors);
}

Ref<ArrayMesh> DebugGeometryContainer::CreateMesh(Mesh::PrimitiveType type, const PoolVector3Array &vertices, const PoolIntArray &indices, const PoolColorArray &colors) {
	Ref<ArrayMesh> mesh;
	mesh.instance();
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

void DebugGeometryContainer::UpdateGeometry() {
	LOCK_GUARD_REC(datalock);

	// Don't clear geometry if freezed
	if (owner->is_freeze_3d_render())
		return;

	_immediateGeometry->clear();

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		for (auto item : mmi_with_values) {
			item->get_multimesh()->set_visible_instance_count(0);
		}
		pool_instance_renderers.reset_counter();
		pool_wireframe_renderers.reset_counter();
		return;
	}

	// Get camera frustum
	Camera *vp_cam = owner->get_viewport()->get_camera();

	Array frustum_array = !owner->get_custom_viewport() || owner->is_force_use_camera_from_scene() ?
								  vp_cam->get_frustum() :
									(owner->get_custom_viewport() ? owner->get_custom_viewport()->get_camera()->get_frustum() : Array());

	// Convert frustum to vector
	std::vector<Plane> f(6);
	if (frustum_array.size()) {
		for (int i = 0; i < frustum_array.size(); i++)
			f[i] = ((Plane)frustum_array[i]);
	} else {
		f.resize(0);
	}

	// Check visibility of all objects

	size_t visible_wireframes_count = 0;
	size_t visible_instance_count[InstanceType::ALL] = { 0 };

	// Update visibility
	if (owner->is_use_frustum_culling() && f.size()) {
		pool_wireframe_renderers.for_each([&f, &visible_wireframes_count](DelayedRendererLine *o) {
			if (o->IsVisible = MathUtils::is_bounds_partially_inside_convex_shape(o->bounds, f))
				visible_wireframes_count++;
		});

		pool_instance_renderers.for_each([&f, &visible_instance_count](DelayedRendererInstance *o) {
			if (o->IsVisible = MathUtils::is_bounds_partially_inside_convex_shape(o->bounds, f))
				visible_instance_count[o->type]++;
		});
	}

	// Draw immediate lines
	_immediateGeometry->begin(Mesh::PrimitiveType::PRIMITIVE_LINES);
	pool_wireframe_renderers.for_each([this](DelayedRendererLine *o) {
		o->IsUsedOneTime = true;

		if (o->IsVisible) {
			_immediateGeometry->set_color(o->LinesColor);
			for (auto &l : o->get_lines()) {
				_immediateGeometry->add_vertex(l);
			};
		}
	});
	_immediateGeometry->end();

	// Debug bounds of instances and lines
	if (false) {
		_immediateGeometry->begin(Mesh::PrimitiveType::PRIMITIVE_LINES);

		pool_wireframe_renderers.for_each([this](DelayedRendererLine *o) {
			if (!o->IsVisible)
				return;

			auto _lines = GeometryGenerator::CreateCubeLines(o->bounds.position, Quat::IDENTITY, o->bounds.size, false, true);

			// TODO DRAW BOXES!!!
			_immediateGeometry->set_color(Color(1, 0.65f, 0, 1));
			for (int i = 0; i < _lines.size(); i++) {
				_immediateGeometry->add_vertex(_lines[i]);
			};
		});
		_immediateGeometry->end();

		// using Planes for 1 Vector3 and real_t..
		std::vector<Plane> new_instances(pool_instance_renderers.get_used_objects());

		auto draw_instance_spheres = [&new_instances](DelayedRendererInstance *o) {
			if (!o->IsVisible)
				return;
			new_instances.push_back({ o->bounds.Position, o->bounds.Radius });
		};

		pool_instance_renderers.for_each(draw_instance_spheres);

		// Draw custom sphere for 1 frame
		for (auto &i : new_instances) {
			Transform t(Basis(), i.normal);
			t.scaled(Vector3::ONE * i.d * 2);

			auto inst = pool_instance_renderers.get();
			inst->update(0, true, InstanceType::SPHERES, t, Color(1, 0.55f, 0, 1), SphereBounds(i.normal, i.d));
			inst->IsUsedOneTime = true;
		}
	}

	// Update MultiMeshInstances

	Ref<MultiMesh> mms[InstanceType::ALL];
	for (int i = 0; i < InstanceType::ALL; i++) {
		mms[i] = mmi_with_values[i]->get_multimesh();
		mms[i]->set_instance_count(visible_instance_count[i]);
		mms[i]->set_visible_instance_count(-1);
	}

	size_t mms_idxs[InstanceType::ALL] = { 0 };
	auto draw_instance_render = [&mms, &mms_idxs](DelayedRendererInstance *o) {
		o->IsUsedOneTime = true;
		if (o->IsVisible) {
			mms[o->type]->set_instance_transform(mms_idxs[o->type], o->transform);
			mms[o->type]->set_instance_color(mms_idxs[o->type], o->color);
			mms_idxs[o->type]++;
		}
	};

	pool_instance_renderers.for_each(draw_instance_render);

	pool_instance_renderers.reset_counter();
	pool_wireframe_renderers.reset_counter();
}

Dictionary DebugGeometryContainer::get_rendered_primitives_count() {
	return Dictionary::make(
			"instances", pool_instance_renderers.get_used_objects(),
			"wireframes", pool_wireframe_renderers.get_used_objects(),
			"total", pool_instance_renderers.get_used_objects() + pool_wireframe_renderers.get_used_objects());
}

#pragma region Draw Functions

void DebugGeometryContainer::clear_3d_objects() {
	for (auto s : mmi_with_values) {
		s->get_multimesh()->set_instance_count(0);
	}
	_immediateGeometry->clear();
}

#pragma region 3D
#pragma region Spheres

void DebugGeometryContainer::draw_sphere(Vector3 position, float radius, Color color, float duration) {
	Transform t(Basis(), position);
	t.basis.scale(Vector3::ONE * (radius * 2));

	draw_sphere_xf(t, color, duration);
}

void DebugGeometryContainer::draw_sphere_xf(Transform transform, Color color, float duration) {
	LOCK_GUARD_REC(datalock);
	auto inst = pool_instance_renderers.get();
	inst->update(int(duration * 1000), true, InstanceType::SPHERES, transform, color == Colors::empty_color ? Colors::chartreuse : color, SphereBounds(transform.origin, abs(transform.basis.get_scale().length() * 0.5f)));
}

#pragma endregion // Spheres
#pragma region Cylinders

void DebugGeometryContainer::draw_cylinder(Vector3 position, Quat rotation, float radius, float height, Color color, float duration) {
	Transform t(rotation, position);
	t.basis.scale(Vector3(radius * 2, height, radius * 2));

	draw_cylinder_xf(t, color, duration);
}

void DebugGeometryContainer::draw_cylinder_xf(Transform transform, Color color, float duration) {
	LOCK_GUARD_REC(datalock);
	auto inst = pool_instance_renderers.get();
	inst->update(int(duration * 1000), true, InstanceType::CYLINDERS, transform, color == Colors::empty_color ? Colors::forest_green : color, SphereBounds(transform.origin, abs(transform.basis.get_scale().length() * 0.5f)));
}

#pragma endregion // Cylinders
#pragma region Boxes

void DebugGeometryContainer::draw_box(Vector3 position, Quat rotation, Vector3 size, Color color, bool is_box_centered, float duration) {
	Transform t(rotation, position);
	t.basis.scale(size);

	draw_box_xf(t, color, duration, is_box_centered);
}

void DebugGeometryContainer::draw_box_xf(Transform transform, Color color, bool is_box_centered, float duration) {
	LOCK_GUARD_REC(datalock);

	auto inst = pool_instance_renderers.get();
	SphereBounds sb(transform.origin, abs(transform.basis.get_scale().length() * 0.5f));
	if (!is_box_centered) {
		sb.Position = transform.origin + transform.basis.get_scale() * 0.5f;
	}
	inst->update(int(duration * 1000), true, is_box_centered ? InstanceType::CUBES_CENTERED : InstanceType::CUBES, transform, color == Colors::empty_color ? Colors::forest_green : color, sb);
}

void DebugGeometryContainer::draw_aabb(AABB aabb, Color color, float duration) {
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(aabb.position, aabb.position + aabb.size, bottom, top, diag);
	draw_box(bottom, Quat(), diag, color, duration);
}

void DebugGeometryContainer::draw_aabb_ab(Vector3 a, Vector3 b, Color color, float duration) {
	Vector3 bottom, top, diag;
	MathUtils::get_diagonal_vectors(a, b, bottom, top, diag);
	draw_box(bottom, Quat(), diag, color, duration);
}

#pragma endregion // Boxes
#pragma region Lines

void DebugGeometryContainer::draw_line_3d_hit(Vector3 a, Vector3 b, bool is_hit, float unitOffsetOfHit, float hitSize, Color hit_color, Color after_hit_color, float duration) {
}

#pragma region Normal

void DebugGeometryContainer::draw_line_3d(Vector3 a, Vector3 b, Color color, float duration) {
}

void DebugGeometryContainer::draw_ray_3d(Vector3 origin, Vector3 direction, float length, Color color, float duration) {
}

void DebugGeometryContainer::draw_line_path_3d(PoolVector3Array path, Color color, float duration) {
}

void DebugGeometryContainer::draw_line_path_3d_arr(Array path, Color color, float duration) {
}

#pragma endregion // Normal
#pragma region Arrows

void DebugGeometryContainer::draw_arrow_line_3d(Vector3 a, Vector3 b, Color color, float arrow_size, bool absolute_size, float duration) {
}

void DebugGeometryContainer::draw_arrow_ray_3d(Vector3 origin, Vector3 direction, float length, Color color, float arrow_size, bool absolute_size, float duration) {
}

void DebugGeometryContainer::draw_arrow_path_3d(PoolVector3Array path, Color color, float arrow_size, bool absolute_size, float duration) {
}

void DebugGeometryContainer::draw_arrow_path_3d_arr(Array path, Color color, float arrow_size, bool absolute_size, float duration) {
}

#pragma endregion // Arrows
#pragma endregion // Lines

#pragma region Misc

void DebugGeometryContainer::draw_billboard_square(Vector3 position, float size, Color color, float duration) {
	Transform t(Basis(), position);
	t.basis.scale(Vector3::ONE * size);

	LOCK_GUARD_REC(datalock);
	auto inst = pool_instance_renderers.get();
	inst->update(int(duration * 1000), true, InstanceType::BILLBOARD_SQUARES, t, color == Colors::empty_color ? Colors::red : color, SphereBounds(position, GeometryGenerator::CubeDiaganolLengthForSphere * size));
}

#pragma region Camera Frustum

void DebugGeometryContainer::draw_camera_frustum(Camera *camera, Color color, float duration) {
}

void DebugGeometryContainer::draw_camera_frustum_planes(Array camera_frustum, Color color, float duration) {
}

void DebugGeometryContainer::draw_camera_frustum_planes_c(Plane planes[], Color color, float duration) {
}

#pragma endregion // Camera Frustum

void DebugGeometryContainer::draw_position_3d(Vector3 position, Quat rotation, Vector3 scale, Color color, float duration) {
}

void DebugGeometryContainer::draw_position_3d_xf(Transform transform, Color color, float duration) {
}

#pragma endregion // Misc
#pragma endregion // 3D
#pragma endregion // Draw Functions
