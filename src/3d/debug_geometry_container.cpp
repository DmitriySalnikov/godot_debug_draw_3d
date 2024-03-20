#include "debug_geometry_container.h"

#ifndef DISABLE_DEBUG_RENDERING
#include "config_3d.h"
#include "config_scope_3d.h"
#include "debug_draw_3d.h"
#include "geometry_generators.h"
#include "stats_3d.h"

#include <array>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/world3d.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

DebugGeometryContainer::DebugGeometryContainer(class DebugDraw3D *root, const bool &add_bevel, const bool &use_icosphere, const bool &use_icosphere_hd) {
	ZoneScoped;
	owner = root;
	RenderingServer *rs = RenderingServer::get_singleton();

	// Create wireframe mesh drawer
	{
		Ref<ArrayMesh> _array_mesh;
		_array_mesh.instantiate();
		RID _immediate_instance = rs->instance_create();

		rs->instance_set_base(_immediate_instance, _array_mesh->get_rid());
		rs->instance_geometry_set_cast_shadows_setting(_immediate_instance, RenderingServer::SHADOW_CASTING_SETTING_OFF);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

		Ref<ShaderMaterial> mat = owner->get_wireframe_material();
		rs->instance_geometry_set_material_override(_immediate_instance, mat->get_rid());

		immediate_mesh_storage.instance = _immediate_instance;
		immediate_mesh_storage.material = mat;
		immediate_mesh_storage.mesh = _array_mesh;
	}

	// Generate geometry and create MMI's in RenderingServer
	{
		auto array_mesh_cube = GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CubeVertexes, GeometryGenerator::CubeIndexes);
		CreateMMI(InstanceType::CUBE, UsingShaderType::Wireframe, NAMEOF(mmi_cubes), array_mesh_cube);

		auto array_mesh_cube_center = GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::CenteredCubeVertexes, GeometryGenerator::CubeIndexes);
		CreateMMI(InstanceType::CUBE_CENTERED, UsingShaderType::Wireframe, NAMEOF(mmi_cubes_centered), array_mesh_cube_center);

		auto array_mesh_arrow_head = GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::ArrowheadVertexes, GeometryGenerator::ArrowheadIndexes);
		CreateMMI(InstanceType::ARROWHEAD, UsingShaderType::Wireframe, NAMEOF(mmi_arrowheads), array_mesh_arrow_head);

		auto array_mesh_pos = GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::PositionVertexes, GeometryGenerator::PositionIndexes);
		CreateMMI(InstanceType::POSITION, UsingShaderType::Wireframe, NAMEOF(mmi_positions), array_mesh_pos);

		auto array_mesh_sphere = use_icosphere ? GeometryGenerator::CreateIcosphereLines(0.5f, 1) : GeometryGenerator::CreateSphereLines(8, 8, 0.5f, 2);
		CreateMMI(InstanceType::SPHERE, UsingShaderType::Wireframe, NAMEOF(mmi_spheres), array_mesh_sphere);

		auto array_mesh_sphere_hd = use_icosphere_hd ? GeometryGenerator::CreateIcosphereLines(0.5f, 2) : GeometryGenerator::CreateSphereLines(16, 16, 0.5f, 2);
		CreateMMI(InstanceType::SPHERE_HD, UsingShaderType::Wireframe, NAMEOF(mmi_spheres_hd), array_mesh_sphere_hd);

		auto array_mesh_cylinder = GeometryGenerator::CreateCylinderLines(16, 1, 1, 2);
		CreateMMI(InstanceType::CYLINDER, UsingShaderType::Wireframe, NAMEOF(mmi_cylinders), array_mesh_cylinder);

		auto array_mesh_cylinder_ab = GeometryGenerator::RotatedMesh(GeometryGenerator::CreateCylinderLines(16, 1, 1, 2), Vector3_RIGHT, Math::deg_to_rad(90.f));
		CreateMMI(InstanceType::CYLINDER_AB, UsingShaderType::Wireframe, NAMEOF(mmi_cylinders), array_mesh_cylinder_ab);

		// VOLUMETRIC

		auto array_mesh_line_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_LINES, GeometryGenerator::LineVertexes), add_bevel);
		CreateMMI(InstanceType::LINE_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_cubes_volumetric), array_mesh_line_volumetric);

		auto array_mesh_cube_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_cube, add_bevel);
		CreateMMI(InstanceType::CUBE_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_cubes_volumetric), array_mesh_cube_volumetric);

		auto array_mesh_cube_centered_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_cube_center, add_bevel);
		CreateMMI(InstanceType::CUBE_CENTERED_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_cubes_centered_volumetric), array_mesh_cube_centered_volumetric);

		auto array_mesh_arrow_head_volumetric = GeometryGenerator::CreateVolumetricArrowHead(.25f, 1.f, 1.f, add_bevel);
		CreateMMI(InstanceType::ARROWHEAD_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_arrowheads_volumetric), array_mesh_arrow_head_volumetric);

		auto array_mesh_pos_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_pos, add_bevel);
		CreateMMI(InstanceType::POSITION_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_positions_volumetric), array_mesh_pos_volumetric);

		auto array_mesh_sphere_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_sphere, false);
		CreateMMI(InstanceType::SPHERE_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_spheres_volumetric), array_mesh_sphere_volumetric);

		auto array_mesh_sphere_hd_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_sphere_hd, false);
		CreateMMI(InstanceType::SPHERE_HD_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_spheres_hd_volumetric), array_mesh_sphere_hd_volumetric);

		auto array_mesh_cylinder_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_cylinder, false);
		CreateMMI(InstanceType::CYLINDER_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_cylinders_volumetric), array_mesh_cylinder_volumetric);

		auto array_mesh_cylinder_ab_volumetric = GeometryGenerator::ConvertWireframeToVolumetric(array_mesh_cylinder_ab, false);
		CreateMMI(InstanceType::CYLINDER_AB_VOLUMETRIC, UsingShaderType::Expandable, NAMEOF(mmi_cylinders_volumetric), array_mesh_cylinder_ab_volumetric);

		// SOLID

		auto array_mesh_billboard = GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES, GeometryGenerator::CenteredSquareVertexes, GeometryGenerator::SquareBackwardsIndexes);
		CreateMMI(InstanceType::BILLBOARD_SQUARE, UsingShaderType::Billboard, NAMEOF(mmi_billboard_squares), array_mesh_billboard);

		auto array_mesh_plane = GeometryGenerator::CreateMeshNative(Mesh::PrimitiveType::PRIMITIVE_TRIANGLES, GeometryGenerator::CenteredSquareVertexes, GeometryGenerator::SquareIndexes);
		CreateMMI(InstanceType::PLANE, UsingShaderType::Solid, NAMEOF(mmi_planes), array_mesh_plane);

		set_render_layer_mask(1);
	}
}

DebugGeometryContainer::~DebugGeometryContainer() {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);

	geometry_pool.clear_pool();
}

void DebugGeometryContainer::CreateMMI(InstanceType type, UsingShaderType shader, const String &name, Ref<ArrayMesh> mesh) {
	ZoneScoped;
	RenderingServer *rs = RenderingServer::get_singleton();

	RID mmi = rs->instance_create();

	Ref<MultiMesh> new_mm;
	new_mm.instantiate();
	new_mm->set_name(String::num_int64((int)type));

	new_mm->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);
	new_mm->set_use_colors(true);
	new_mm->set_transform_format(MultiMesh::TRANSFORM_3D);
	new_mm->set_use_custom_data(true);
	new_mm->set_mesh(mesh);

	rs->instance_set_base(mmi, new_mm->get_rid());

	Ref<ShaderMaterial> mat;
	switch (shader) {
		case UsingShaderType::Wireframe:
			mat = owner->get_wireframe_material();
			break;
		case UsingShaderType::Billboard:
			mat = owner->get_billboard_material();
			break;
		case UsingShaderType::Solid:
			mat = owner->get_plane_material();
			break;
		case UsingShaderType::Expandable:
			mat = owner->get_extendable_material();
			break;
	}

	mesh->surface_set_material(0, mat);

	rs->instance_geometry_set_cast_shadows_setting(mmi, RenderingServer::SHADOW_CASTING_SETTING_OFF);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

	multi_mesh_storage[(int)type].instance = mmi;
	multi_mesh_storage[(int)type].mesh = new_mm;
}

void DebugGeometryContainer::set_world(Ref<World3D> new_world) {
	ZoneScoped;
	if (new_world == base_world_viewport) {
		return;
	}

	base_world_viewport = new_world;
	RenderingServer *rs = RenderingServer::get_singleton();
	RID scenario = base_world_viewport.is_valid() ? base_world_viewport->get_scenario() : RID();

	for (auto &s : multi_mesh_storage) {
		rs->instance_set_scenario(s.instance, scenario);
	}
	rs->instance_set_scenario(immediate_mesh_storage.instance, scenario);
}

Ref<World3D> DebugGeometryContainer::get_world() {
	return base_world_viewport;
}

// TODO add mark_dirty for 3d to reduce editor updates if only delayed shapes are displayed.
void DebugGeometryContainer::update_geometry(double delta) {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);

	// cleanup and get available viewports
	std::vector<Viewport *> available_viewports = geometry_pool.get_and_validate_viewports();

	// Do not update geometry if frozen
	if (owner->get_config()->is_freeze_3d_render())
		return;

	if (immediate_mesh_storage.mesh->get_surface_count())
		immediate_mesh_storage.mesh->clear_surfaces();

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		for (auto &item : multi_mesh_storage) {
			if (item.mesh->get_visible_instance_count())
				item.mesh->set_visible_instance_count(0);
		}
		geometry_pool.reset_counter(delta);
		geometry_pool.reset_visible_objects();
		return;
	}

	// Update render layers
	if (render_layers != owner->get_config()->get_geometry_render_layers()) {
		set_render_layer_mask(owner->get_config()->get_geometry_render_layers());
	}

	std::unordered_map<const Viewport *, std::shared_ptr<GeometryPoolCullingData> > culling_data;
	for (const auto &vp_p : available_viewports) {
		// Collect frustums and camera positions
		std::vector<std::array<Plane, 6> > frustum_planes;
		std::vector<Vector3> cameras_positions;
		cameras_positions.reserve(1);

		std::vector<Array> frustum_arrays;
		frustum_arrays.reserve(1);

		auto custom_editor_viewports = owner->get_custom_editor_viewports();
		Camera3D *vp_cam = vp_p->get_camera_3d();

		if ((owner->get_config()->is_force_use_camera_from_scene() || !custom_editor_viewports.size()) && vp_cam) {
			frustum_arrays.push_back(vp_cam->get_frustum());
			cameras_positions.push_back(vp_cam->get_position());
		} else if (custom_editor_viewports.size() > 0) {
			for (auto vp : custom_editor_viewports) {
				if (vp->get_update_mode() == SubViewport::UpdateMode::UPDATE_ALWAYS) {
					auto c = vp->get_camera_3d();
					frustum_arrays.push_back(c->get_frustum());
					cameras_positions.push_back(c->get_position());
				}
			}
		}

		// Convert Array to vector
		if (owner->get_config()->is_use_frustum_culling() && frustum_arrays.size()) {
			frustum_planes.reserve(frustum_arrays.size());

			for (auto &arr : frustum_arrays) {
				if (arr.size() == 6) {
					std::array<Plane, 6> a;
					for (int i = 0; i < arr.size(); i++)
						a[i] = (Plane)arr[i];
					frustum_planes.push_back(a);
				}
			}
		}

		culling_data[vp_p] = std::make_shared<GeometryPoolCullingData>(frustum_planes, /*TODO replace by scoped somehow*/ owner->get_config()->get_culling_distance(), cameras_positions);
	}

	// Update visibility
	geometry_pool.update_visibility(culling_data);

	// Debug bounds of instances and lines
	if (owner->get_config()->is_visible_instance_bounds()) {
		ZoneScopedN("Debug bounds");
		struct sphereDebug {
			Vector3 pos;
			real_t radius;
			Viewport *vp;
		};
		std::vector<sphereDebug> new_instances;

		auto draw_instance_spheres = [&new_instances](Viewport *vp, DelayedRendererInstance *o) {
			if (!o->is_visible || o->is_expired())
				return;
			new_instances.push_back({ o->bounds.position, o->bounds.Radius, vp });
		};

		geometry_pool.for_each_instance(draw_instance_spheres);

		auto cfg = std::make_shared<DebugDraw3DScopeConfig::Data>(owner->scoped_config()->data);

		// Draw custom sphere for 1 frame
		for (auto &i : new_instances) {
			cfg->viewport = i.vp;

			geometry_pool.add_or_update_instance(
					cfg,
					InstanceType::SPHERE,
					0,
					ProcessType::PROCESS,
					Transform3D(Basis().scaled(Vector3_ONE * i.radius * 2), i.pos),
					Colors::debug_bounds,
					SphereBounds(i.pos, i.radius),
					&Colors::empty_color,
					[](auto d) { d->is_used_one_time = true; });
		}

		geometry_pool.for_each_line([this, &cfg](Viewport *vp, DelayedRendererLine *o) {
			if (!o->is_visible || o->is_expired())
				return;

			Vector3 bottom, top, diag;
			MathUtils::get_diagonal_vectors(o->bounds.position, o->bounds.position + o->bounds.size, bottom, top, diag);
			cfg->viewport = vp;

			geometry_pool.add_or_update_instance(
					cfg,
					InstanceType::CUBE,
					0,
					ProcessType::PROCESS,
					Transform3D(Basis().scaled(diag), bottom),
					Colors::debug_bounds,
					SphereBounds(bottom + diag * 0.5f, abs(diag.length() * 0.5f)),
					&Colors::empty_color,
					[](auto d) { d->is_used_one_time = true; });
		});
	}

	// Draw immediate lines
	geometry_pool.fill_lines_data(immediate_mesh_storage.mesh, delta);

	// Update MultiMeshInstances
	static std::array<Ref<MultiMesh> *, (int)InstanceType::MAX> meshes;
	for (int i = 0; i < (int)InstanceType::MAX; i++) {
		meshes[i] = &multi_mesh_storage[i].mesh;
	}

	geometry_pool.fill_instance_data(meshes, delta);

	geometry_pool.scan_visible_instances();
	geometry_pool.update_expiration(delta, ProcessType::PROCESS);
	geometry_pool.reset_counter(delta, ProcessType::PROCESS);

	is_frame_rendered = true;
}

void DebugGeometryContainer::update_geometry_physics_start(double delta) {
	if (is_frame_rendered) {
		geometry_pool.reset_counter(delta, ProcessType::PHYSICS_PROCESS);
		is_frame_rendered = false;
	}
}

void DebugGeometryContainer::update_geometry_physics_end(double delta) {
	geometry_pool.update_expiration(delta, ProcessType::PHYSICS_PROCESS);
}

void DebugGeometryContainer::get_render_stats(Ref<DebugDraw3DStats> &stats) {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);
	return geometry_pool.set_stats(stats);
}

void DebugGeometryContainer::set_render_layer_mask(int32_t layers) {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);
	if (render_layers != layers) {
		RenderingServer *rs = RenderingServer::get_singleton();
		for (auto &mmi : multi_mesh_storage)
			rs->instance_set_layer_mask(mmi.instance, layers);

		rs->instance_set_layer_mask(immediate_mesh_storage.instance, layers);
		render_layers = layers;
	}
}

int32_t DebugGeometryContainer::get_render_layer_mask() const {
	return render_layers;
}

void DebugGeometryContainer::clear_3d_objects() {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);
	for (auto &s : multi_mesh_storage) {
		s.mesh->set_instance_count(0);
	}
	immediate_mesh_storage.mesh->clear_surfaces();

	geometry_pool.clear_pool();
}

#endif