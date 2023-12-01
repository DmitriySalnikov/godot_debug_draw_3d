#pragma once
#ifndef DISABLE_DEBUG_RENDERING

#include "render_instances.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/shader_material.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class DebugDrawStats3D;

class DebugGeometryContainer {
	friend class DebugDraw3D;
	class DebugDraw3D *owner;

	enum class UsingShaderType {
		Wireframe,
		Billboard,
		Expandable,
	};

	struct MultiMeshStorage {
		RID instance;
		Ref<MultiMesh> mesh;

		~MultiMeshStorage() {
			RenderingServer::get_singleton()->free_rid(instance);
			mesh.unref();
		}
	};
	MultiMeshStorage multi_mesh_storage[(int)InstanceType::ALL] = {};

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
	Node *scene_world_node = nullptr;
	int32_t render_layers = 1;

	void CreateMMI(InstanceType type, UsingShaderType shader, const String &name, Ref<ArrayMesh> mesh);

public:
	DebugGeometryContainer(class DebugDraw3D *root);
	~DebugGeometryContainer();

	void set_world(Node *new_world);
	Node *get_world();

	void update_geometry(double delta);

	void set_render_layer_mask(int32_t layers);
	int32_t get_render_layer_mask() const;

	void get_render_stats(Ref<DebugDrawStats3D> &stats);
	void clear_3d_objects();
};

#endif