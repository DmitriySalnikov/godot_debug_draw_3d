#pragma once
#ifndef DISABLE_DEBUG_RENDERING

#include "render_instances.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/world3d.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DebugDraw3DStats;

class DebugGeometryContainer {
	friend class DebugDraw3D;
	class DebugDraw3D *owner;

	struct MultiMeshStorage {
		RID instance;
		Ref<MultiMesh> mesh;

		~MultiMeshStorage() {
			RenderingServer::get_singleton()->free_rid(instance);
			mesh.unref();
		}
	};
	MultiMeshStorage multi_mesh_storage[(int)InstanceType::MAX] = {};

	struct ImmediateMeshStorage {
		RID instance;
		Ref<ArrayMesh> mesh;
		Ref<ShaderMaterial> material;

		~ImmediateMeshStorage() {
			RenderingServer::get_singleton()->free_rid(instance);
			mesh.unref();
			material.unref();
		}
	};
	ImmediateMeshStorage immediate_mesh_storage;

	GeometryPool geometry_pool;
	Ref<World3D> base_world_viewport;
	int32_t render_layers = 1;
	bool is_frame_rendered = false;
	bool no_depth_test = false;

	void CreateMMI(InstanceType p_type, Ref<ArrayMesh> p_mesh);

public:
	DebugGeometryContainer(class DebugDraw3D *p_root, bool p_no_depth_test);
	~DebugGeometryContainer();

	bool is_no_depth_test() const;

	void set_world(Ref<World3D> p_new_world);
	Ref<World3D> get_world();

	void update_geometry(double p_delta);
	void update_geometry_physics_start(double p_delta);
	void update_geometry_physics_end(double p_delta);

	void set_render_layer_mask(int32_t p_layers);
	int32_t get_render_layer_mask() const;

	void get_render_stats(Ref<DebugDraw3DStats> &p_stats);
	void clear_3d_objects();
};

#endif