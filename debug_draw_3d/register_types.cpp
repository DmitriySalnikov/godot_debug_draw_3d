/* register_types.cpp */

#include <ArrayMesh.hpp>
#include <Camera.hpp>
#include <CanvasItem.hpp>
#include <CanvasLayer.hpp>
#include <Control.hpp>
#include <EditorPlugin.hpp>
#include <Engine.hpp>
#include <Font.hpp>
#include <GeometryInstance.hpp>
#include <Godot.hpp>
#include <ImmediateGeometry.hpp>
#include <MainLoop.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <MultiMesh.hpp>
#include <MultiMeshInstance.hpp>
#include <Node.hpp>
#include <Node2D.hpp>
#include <Object.hpp>
#include <Reference.hpp>
#include <Resource.hpp>
#include <SceneTree.hpp>
#include <SpatialMaterial.hpp>
#include <Texture.hpp>
#include <Viewport.hpp>
#include <VisualInstance.hpp>

#ifndef NO_EDITOR
#include "editor_plugin.h"
#endif
#include "data_graphs.h"
#include "debug_draw.h"

using namespace godot;

/** GDNative Initialize **/
extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	Godot::gdnative_init(o);

	// Custom register and init for only needed classes
	// Works only with my patches

	godot::_TagDB::register_global_type("ArrayMesh", typeid(ArrayMesh).hash_code(), typeid(Mesh).hash_code());
	godot::_TagDB::register_global_type("Camera", typeid(Camera).hash_code(), typeid(Spatial).hash_code());
	godot::_TagDB::register_global_type("CanvasItem", typeid(CanvasItem).hash_code(), typeid(Node).hash_code());
	godot::_TagDB::register_global_type("CanvasLayer", typeid(CanvasLayer).hash_code(), typeid(Node).hash_code());
	godot::_TagDB::register_global_type("Control", typeid(Control).hash_code(), typeid(CanvasItem).hash_code());
	godot::_TagDB::register_global_type("EditorPlugin", typeid(EditorPlugin).hash_code(), typeid(Node).hash_code());
	godot::_TagDB::register_global_type("Engine", typeid(Engine).hash_code(), typeid(Object).hash_code());
	godot::_TagDB::register_global_type("Font", typeid(Font).hash_code(), typeid(Object).hash_code());
	godot::_TagDB::register_global_type("GeometryInstance", typeid(GeometryInstance).hash_code(), typeid(VisualInstance).hash_code());
	godot::_TagDB::register_global_type("ImmediateGeometry", typeid(ImmediateGeometry).hash_code(), typeid(GeometryInstance).hash_code());
	godot::_TagDB::register_global_type("MainLoop", typeid(MainLoop).hash_code(), typeid(Object).hash_code());
	godot::_TagDB::register_global_type("Material", typeid(Material).hash_code(), typeid(Resource).hash_code());
	godot::_TagDB::register_global_type("Mesh", typeid(Mesh).hash_code(), typeid(Resource).hash_code());
	godot::_TagDB::register_global_type("MultiMesh", typeid(MultiMesh).hash_code(), typeid(Resource).hash_code());
	godot::_TagDB::register_global_type("MultiMeshInstance", typeid(MultiMeshInstance).hash_code(), typeid(GeometryInstance).hash_code());
	godot::_TagDB::register_global_type("Node", typeid(Node).hash_code(), typeid(Object).hash_code());
	godot::_TagDB::register_global_type("Node2D", typeid(Node2D).hash_code(), typeid(CanvasItem).hash_code());
	godot::_TagDB::register_global_type("Object", typeid(Object).hash_code(), 0);
	godot::_TagDB::register_global_type("Reference", typeid(Reference).hash_code(), typeid(Object).hash_code());
	godot::_TagDB::register_global_type("Resource", typeid(Resource).hash_code(), typeid(Reference).hash_code());
	godot::_TagDB::register_global_type("SceneTree", typeid(SceneTree).hash_code(), typeid(MainLoop).hash_code());
	godot::_TagDB::register_global_type("SpatialMaterial", typeid(SpatialMaterial).hash_code(), typeid(Material).hash_code());
	godot::_TagDB::register_global_type("Texture", typeid(Texture).hash_code(), typeid(Resource).hash_code());
	godot::_TagDB::register_global_type("Viewport", typeid(Viewport).hash_code(), typeid(Node).hash_code());
	godot::_TagDB::register_global_type("VisualInstance", typeid(VisualInstance).hash_code(), typeid(CullInstance).hash_code());

	ArrayMesh::___init_method_bindings();
	Camera::___init_method_bindings();
	CanvasItem::___init_method_bindings();
	CanvasLayer::___init_method_bindings();
	Control::___init_method_bindings();
	EditorPlugin::___init_method_bindings();
	Engine::___init_method_bindings();
	Font::___init_method_bindings();
	GeometryInstance::___init_method_bindings();
	ImmediateGeometry::___init_method_bindings();
	MainLoop::___init_method_bindings();
	Material::___init_method_bindings();
	Mesh::___init_method_bindings();
	MultiMesh::___init_method_bindings();
	MultiMeshInstance::___init_method_bindings();
	Node::___init_method_bindings();
	Object::___init_method_bindings();
	Reference::___init_method_bindings();
	Resource::___init_method_bindings();
	SceneTree::___init_method_bindings();
	SpatialMaterial::___init_method_bindings();
	Texture::___init_method_bindings();
	Viewport::___init_method_bindings();
	VisualInstance::___init_method_bindings();
}

/** GDNative Terminate **/
extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	Godot::gdnative_terminate(o);
}

/** GDNative Singleton **/
extern "C" void GDN_EXPORT godot_gdnative_singleton() {
}

/** NativeScript Initialize **/
extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	Godot::nativescript_init(handle);

	register_tool_class<DebugDraw3D>();
	register_tool_class<GraphParameters>();

#ifndef NO_EDITOR
	register_tool_class<DebugDraw3DEditorPlugin>();
#endif
}