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

DebugGeometryContainer::DebugGeometryContainer(class DebugDraw3D *p_root, bool p_no_depth_test) {
	ZoneScoped;
	DEV_PRINT_STD("New %s created: %s\n", NAMEOF(DebugGeometryContainer), p_no_depth_test ? "NoDepth" : "Normal");
	owner = p_root;
	RenderingServer *rs = RenderingServer::get_singleton();
	no_depth_test = p_no_depth_test;
	geometry_pool.set_no_depth_test_info(no_depth_test);

	// Create wireframe mesh drawer
	{
		Ref<ArrayMesh> _array_mesh;
		_array_mesh.instantiate();
		RID _immediate_instance = rs->instance_create();

		rs->instance_set_base(_immediate_instance, _array_mesh->get_rid());
		rs->instance_geometry_set_cast_shadows_setting(_immediate_instance, RenderingServer::SHADOW_CASTING_SETTING_OFF);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
		rs->instance_geometry_set_flag(_immediate_instance, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

		Ref<ShaderMaterial> mat = owner->get_material_variant(MeshMaterialType::Wireframe, no_depth_test ? MeshMaterialVariant::NoDepth : MeshMaterialVariant::Normal);
		rs->instance_geometry_set_material_override(_immediate_instance, mat->get_rid());

		immediate_mesh_storage.instance = _immediate_instance;
		immediate_mesh_storage.material = mat;
		immediate_mesh_storage.mesh = _array_mesh;
	}

	// Generate geometry and create MMI's in RenderingServer
	{
		auto *meshes = owner->get_shared_meshes();
		int mat_variant = !!no_depth_test;

		CreateMMI(InstanceType::CUBE, meshes[(int)InstanceType::CUBE][mat_variant]);
		CreateMMI(InstanceType::CUBE_CENTERED, meshes[(int)InstanceType::CUBE_CENTERED][mat_variant]);
		CreateMMI(InstanceType::ARROWHEAD, meshes[(int)InstanceType::ARROWHEAD][mat_variant]);
		CreateMMI(InstanceType::POSITION, meshes[(int)InstanceType::POSITION][mat_variant]);
		CreateMMI(InstanceType::SPHERE, meshes[(int)InstanceType::SPHERE][mat_variant]);
		CreateMMI(InstanceType::SPHERE_HD, meshes[(int)InstanceType::SPHERE_HD][mat_variant]);
		CreateMMI(InstanceType::CYLINDER, meshes[(int)InstanceType::CYLINDER][mat_variant]);
		CreateMMI(InstanceType::CYLINDER_AB, meshes[(int)InstanceType::CYLINDER_AB][mat_variant]);

		// VOLUMETRIC

		CreateMMI(InstanceType::LINE_VOLUMETRIC, meshes[(int)InstanceType::LINE_VOLUMETRIC][mat_variant]);
		CreateMMI(InstanceType::CUBE_VOLUMETRIC, meshes[(int)InstanceType::CUBE_VOLUMETRIC][mat_variant]);
		CreateMMI(InstanceType::CUBE_CENTERED_VOLUMETRIC, meshes[(int)InstanceType::CUBE_CENTERED_VOLUMETRIC][mat_variant]);
		CreateMMI(InstanceType::ARROWHEAD_VOLUMETRIC, meshes[(int)InstanceType::ARROWHEAD_VOLUMETRIC][mat_variant]);
		CreateMMI(InstanceType::POSITION_VOLUMETRIC, meshes[(int)InstanceType::POSITION_VOLUMETRIC][mat_variant]);
		CreateMMI(InstanceType::SPHERE_VOLUMETRIC, meshes[(int)InstanceType::SPHERE_VOLUMETRIC][mat_variant]);
		CreateMMI(InstanceType::SPHERE_HD_VOLUMETRIC, meshes[(int)InstanceType::SPHERE_HD_VOLUMETRIC][mat_variant]);
		CreateMMI(InstanceType::CYLINDER_VOLUMETRIC, meshes[(int)InstanceType::CYLINDER_VOLUMETRIC][mat_variant]);
		CreateMMI(InstanceType::CYLINDER_AB_VOLUMETRIC, meshes[(int)InstanceType::CYLINDER_AB_VOLUMETRIC][mat_variant]);

		// SOLID

		CreateMMI(InstanceType::BILLBOARD_SQUARE, meshes[(int)InstanceType::BILLBOARD_SQUARE][mat_variant]);
		CreateMMI(InstanceType::PLANE, meshes[(int)InstanceType::PLANE][mat_variant]);

		set_render_layer_mask(1);
	}
}

DebugGeometryContainer::~DebugGeometryContainer() {
	ZoneScoped;
	DEV_PRINT_STD(NAMEOF(DebugGeometryContainer) " destroyed: %s, World3D (%d)\n", no_depth_test ? "NoDepth" : "Normal", viewport_world.is_valid() ? viewport_world->get_instance_id() : 0);

	LOCK_GUARD(owner->datalock);

	geometry_pool.clear_pool();
}

bool DebugGeometryContainer::is_no_depth_test() const {
	return no_depth_test;
}

void DebugGeometryContainer::CreateMMI(InstanceType p_type, Ref<ArrayMesh> p_mesh) {
	ZoneScoped;
	RenderingServer *rs = RenderingServer::get_singleton();

	RID mmi = rs->instance_create();

	Ref<MultiMesh> new_mm;
	new_mm.instantiate();
	new_mm->set_name(String::num_int64((int)p_type));

	new_mm->set_use_colors(true);
	new_mm->set_transform_format(MultiMesh::TRANSFORM_3D);
	new_mm->set_use_custom_data(true);
	new_mm->set_mesh(p_mesh);

	rs->instance_set_base(mmi, new_mm->get_rid());

	rs->instance_geometry_set_cast_shadows_setting(mmi, RenderingServer::SHADOW_CASTING_SETTING_OFF);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_DYNAMIC_GI, false);
	rs->instance_geometry_set_flag(mmi, RenderingServer::INSTANCE_FLAG_USE_BAKED_LIGHT, false);

	multi_mesh_storage[(int)p_type].instance = mmi;
	multi_mesh_storage[(int)p_type].mesh = new_mm;
}

void DebugGeometryContainer::set_world(Ref<World3D> p_new_world) {
	ZoneScoped;
	if (p_new_world == viewport_world) {
		return;
	}

	viewport_world = p_new_world;

	RenderingServer *rs = RenderingServer::get_singleton();
	RID scenario = viewport_world.is_valid() ? viewport_world->get_scenario() : RID();

	for (auto &s : multi_mesh_storage) {
		rs->instance_set_scenario(s.instance, scenario);
	}

	rs->instance_set_scenario(immediate_mesh_storage.instance, scenario);
}

Ref<World3D> DebugGeometryContainer::get_world() {
	return viewport_world;
}

#if defined(REAL_T_IS_DOUBLE) && defined(FIX_PRECISION_ENABLED)
const Vector3 &DebugGeometryContainer::get_center_position() {
	return center_position;
}

void DebugGeometryContainer::update_center_positions() {
	if (center_position.distance_to(new_center_position) < 8192)
		return;

	DEV_PRINT_STD(NAMEOF(DebugGeometryContainer) " Updated instance positions: %s, World3D (%d)\n", no_depth_test ? "NoDepth" : "Normal", viewport_world.is_valid() ? viewport_world->get_instance_id() : 0);

	Vector3 pos_diff = center_position - new_center_position;
	center_position = new_center_position;

	geometry_pool.for_each_instance([&pos_diff](DelayedRendererInstance *i) {
		i->data.origin_x += (float)pos_diff.x;
		i->data.origin_y += (float)pos_diff.y;
		i->data.origin_z += (float)pos_diff.z;
	});

	geometry_pool.for_each_line([&pos_diff](DelayedRendererLine *i) {
		for (size_t l = 0; l < i->lines_count; l++) {
			i->lines[l] += pos_diff;
		}
	});

	RenderingServer *rs = RenderingServer::get_singleton();
	Transform3D xf = Transform3D(Basis(), center_position);
	for (auto &s : multi_mesh_storage) {
		rs->instance_set_transform(s.instance, xf);
	}

	rs->instance_set_transform(immediate_mesh_storage.instance, xf);
}
#endif

void DebugGeometryContainer::update_geometry(double p_delta) {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);

	// cleanup and get available viewports
	std::vector<Viewport *> available_viewports = geometry_pool.get_and_validate_viewports();

	// accumulate a time delta to delete objects in any case after their timers expire.
	geometry_pool.update_expiration_delta(p_delta, ProcessType::PROCESS);

	// Do not update geometry if frozen
	if (owner->get_config()->is_freeze_3d_render())
		return;

	if (immediate_mesh_storage.mesh->get_surface_count()) {
		ZoneScopedN("Clear lines");
		immediate_mesh_storage.mesh->clear_surfaces();
	}

	// Return if nothing to do
	if (!owner->is_debug_enabled()) {
		ZoneScopedN("Reset instances");
		for (auto &item : multi_mesh_storage) {
			if (item.mesh->get_visible_instance_count())
				item.mesh->set_visible_instance_count(0);
		}
		geometry_pool.reset_counter(p_delta);
		geometry_pool.reset_visible_objects();
		return;
	}

	// Update render layers
	if (render_layers != owner->get_config()->get_geometry_render_layers()) {
		set_render_layer_mask(owner->get_config()->get_geometry_render_layers());
	}

#if defined(REAL_T_IS_DOUBLE) && defined(FIX_PRECISION_ENABLED)
#define FIX_DOUBLE_PRECISION_ERRORS
#endif

	std::unordered_map<Viewport *, std::shared_ptr<GeometryPoolCullingData> > culling_data;
	{
		ZoneScopedN("Get frustums");

		for (const auto &vp_p : available_viewports) {
			std::vector<std::array<Plane, 6> > frustum_planes;
			std::vector<AABBMinMax> frustum_boxes;

			std::vector<std::pair<Array, Camera3D *> > frustum_arrays;
			frustum_arrays.reserve(1);

#ifdef DEBUG_ENABLED
			auto custom_editor_viewports = owner->get_custom_editor_viewports();
			bool is_editor_vp = std::find_if(
										custom_editor_viewports.cbegin(),
										custom_editor_viewports.cend(),
										[&vp_p](const auto &it) { return it == vp_p; }) != custom_editor_viewports.cend();

			if (IS_EDITOR_HINT() && is_editor_vp) {
				Camera3D *cam = nullptr;
				Node *root = SCENE_TREE()->get_edited_scene_root();
				if (root) {
					cam = root->get_viewport()->get_camera_3d();
				}

				if (owner->config->is_force_use_camera_from_scene() && cam) {
					frustum_arrays.push_back({ cam->get_frustum(), cam });

#ifdef FIX_DOUBLE_PRECISION_ERRORS
					new_center_position = cam->get_global_position();
#endif
				} else if (custom_editor_viewports.size() > 0) {
					bool is_updated = false;
					for (const auto &evp : custom_editor_viewports) {
						if (evp->get_update_mode() == SubViewport::UpdateMode::UPDATE_ALWAYS) {
							Camera3D *vp_cam = evp->get_camera_3d();
							if (vp_cam) {
								frustum_arrays.push_back({ vp_cam->get_frustum(), vp_cam });

								if (!is_updated) {
									is_updated = true;
#ifdef FIX_DOUBLE_PRECISION_ERRORS
									new_center_position = vp_cam->get_global_position();
#endif
								}
							}
						}
					}
				}
			} else {
#endif
				Camera3D *vp_cam = vp_p->get_camera_3d();
				if (vp_cam) {
					frustum_arrays.push_back({ vp_cam->get_frustum(), vp_cam });

#ifdef FIX_DOUBLE_PRECISION_ERRORS
					new_center_position = vp_cam->get_global_position();
#endif
				}
#ifdef DEBUG_ENABLED
			}
#endif

			// Convert Array to vector
			if (frustum_arrays.size()) {
				for (auto &pair : frustum_arrays) {
					Array &arr = pair.first;
					if (arr.size() == 6) {
						std::array<Plane, 6> a;
						for (int i = 0; i < arr.size(); i++)
							a[i] = (Plane)arr[i];

						MathUtils::scale_frustum_far_plane_distance(a, pair.second->get_global_transform(), owner->get_config()->get_frustum_length_scale());

						if (owner->get_config()->is_use_frustum_culling())
							frustum_planes.push_back(a);

						auto cube = MathUtils::get_frustum_cube(a);
						AABB aabb = MathUtils::calculate_vertex_bounds(cube.data(), cube.size());
						frustum_boxes.push_back(aabb);

#if false
						// Debug camera bounds
						{
							SphereBounds sb = aabb;
							auto cfg = owner->new_scoped_config()->set_thickness(0.1f)->set_hd_sphere(true); //->set_viewport(vp_p);
							owner->draw_sphere(sb.position, sb.radius, Colors::crimson);
							owner->draw_aabb(aabb, Colors::yellow);
						}
#endif
					}
				}
			}

			culling_data[vp_p] = std::make_shared<GeometryPoolCullingData>(frustum_planes, frustum_boxes);
		}
	}

#ifdef FIX_DOUBLE_PRECISION_ERRORS
	update_center_positions();
#endif
#undef FIX_DOUBLE_PRECISION_ERRORS

	// Debug bounds of instances and lines
	if (owner->get_config()->is_visible_instance_bounds()) {
		ZoneScopedN("Debug bounds");
		struct sphereDebug {
			Vector3 pos;
			real_t radius;
		};

		Viewport *vp;
		if (available_viewports.size()) {
			vp = *available_viewports.begin();
			auto cfg = std::make_shared<DebugDraw3DScopeConfig::Data>(owner->scoped_config()->data);
			cfg->thickness = 0;

			std::vector<AABBMinMax> new_instances;
			geometry_pool.for_each_instance([&new_instances](DelayedRendererInstance *o) {
				if (!o->is_visible || o->is_expired())
					return;
				new_instances.push_back(o->bounds);
			});

			// Draw custom sphere for 1 frame
			for (auto &i : new_instances) {
				cfg->dcd.viewport = vp;
				Vector3 diag = i.max - i.min;
				Vector3 center = i.center;
				real_t radius = i.radius;

				geometry_pool.add_or_update_instance(
						cfg,
						InstanceType::SPHERE,
						0,
						ProcessType::PROCESS,
						Transform3D(Basis().scaled(VEC3_ONE(radius) * 2), center),
						Colors::debug_sphere_bounds,
						SphereBounds(center, radius));

				geometry_pool.add_or_update_instance(
						cfg,
						InstanceType::CUBE_CENTERED,
						0,
						ProcessType::PROCESS,
						Transform3D(Basis().scaled(diag), center),
						Colors::debug_rough_box_bounds,
						SphereBounds(center, radius));
			}

			geometry_pool.for_each_line([this, &cfg, &vp](DelayedRendererLine *o) {
				if (!o->is_visible || o->is_expired())
					return;

				Vector3 diag = o->bounds.max - o->bounds.min;
				Vector3 center = o->bounds.center;
				real_t radius = o->bounds.radius;

				cfg->dcd.viewport = vp;
				geometry_pool.add_or_update_instance(
						cfg,
						InstanceType::CUBE_CENTERED,
						0,
						ProcessType::PROCESS,
						Transform3D(Basis().scaled(diag), center),
						Colors::debug_box_bounds,
						SphereBounds(center, radius),
						&Colors::empty_color);

				geometry_pool.add_or_update_instance(
						cfg,
						InstanceType::SPHERE,
						0,
						ProcessType::PROCESS,
						Transform3D(Basis().scaled(VEC3_ONE(radius) * 2), center),
						Colors::debug_sphere_bounds,
						SphereBounds(center, radius));
			});

			for (const auto &culling_data : culling_data) {
				for (const auto &frustum : culling_data.second->m_frustums) {
					size_t s = GeometryGenerator::CubeIndexes.size();
					std::unique_ptr<Vector3[]> l(new Vector3[s]);
					GeometryGenerator::CreateCameraFrustumLinesWireframe(frustum, l.get());

					geometry_pool.add_or_update_line(
							cfg,
							0,
							ProcessType::PROCESS,
							std::move(l),
							s,
							Colors::red,
							MathUtils::calculate_vertex_bounds(l.get(), s));
				}
			}
		}
	}

	std::vector<Ref<MultiMesh> *> meshes((int)InstanceType::MAX);
	for (int i = 0; i < (int)InstanceType::MAX; i++) {
		meshes[i] = &multi_mesh_storage[i].mesh;
	}

	geometry_pool.reset_visible_objects();
	geometry_pool.fill_mesh_data(meshes, immediate_mesh_storage.mesh, culling_data);

	geometry_pool.reset_counter(p_delta, ProcessType::PROCESS);

	is_frame_rendered = true;
}

void DebugGeometryContainer::update_geometry_physics_start(double p_delta) {
	if (is_frame_rendered) {
		geometry_pool.reset_counter(p_delta, ProcessType::PHYSICS_PROCESS);
		is_frame_rendered = false;
	}
}

void DebugGeometryContainer::update_geometry_physics_end(double p_delta) {
	geometry_pool.update_expiration_delta(p_delta, ProcessType::PHYSICS_PROCESS);
}

void DebugGeometryContainer::get_render_stats(Ref<DebugDraw3DStats> &p_stats) {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);
	return geometry_pool.set_stats(p_stats);
}

void DebugGeometryContainer::set_render_layer_mask(int32_t p_layers) {
	ZoneScoped;
	LOCK_GUARD(owner->datalock);
	if (render_layers != p_layers) {
		RenderingServer *rs = RenderingServer::get_singleton();
		for (auto &mmi : multi_mesh_storage)
			rs->instance_set_layer_mask(mmi.instance, p_layers);

		rs->instance_set_layer_mask(immediate_mesh_storage.instance, p_layers);
		render_layers = p_layers;
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