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

	// Base class for others
	godot::_TagDB::register_global_type("Object", typeid(Object).hash_code(), 0);
	Object::___init_method_bindings();

	// Custom register and init for only needed classes
	// Works only with my patches

#define CUSTOM_CLASS(_class, _base, _name)                                                             \
	godot::_TagDB::register_global_type(_name, typeid(_class).hash_code(), typeid(_base).hash_code()); \
	_class::___init_method_bindings();

#define TEXT(t) #t
#define REGULAR_CLASS(_class, _base) CUSTOM_CLASS(_class, _base, #_class)
#define REGULAR_CLASS_DASH(_class, _base) CUSTOM_CLASS(_class, _base, TEXT(_##_class))

	REGULAR_CLASS(ArrayMesh, Mesh);
	REGULAR_CLASS(Camera, Spatial);
	REGULAR_CLASS(CanvasItem, Node);
	REGULAR_CLASS(CanvasLayer, Node);
	REGULAR_CLASS(Control, CanvasItem);
	REGULAR_CLASS(EditorPlugin, Node);
	REGULAR_CLASS(Engine, Object);
	REGULAR_CLASS(Font, Object);
	REGULAR_CLASS(GeometryInstance, VisualInstance);
	REGULAR_CLASS(ImmediateGeometry, GeometryInstance);
	REGULAR_CLASS(MainLoop, Object);
	REGULAR_CLASS(Material, Resource);
	REGULAR_CLASS(Mesh, Resource);
	REGULAR_CLASS(MultiMesh, Resource);
	REGULAR_CLASS(MultiMeshInstance, GeometryInstance);
	REGULAR_CLASS(Node, Object);
	REGULAR_CLASS(Node2D, CanvasItem);
	REGULAR_CLASS(Reference, Object);
	REGULAR_CLASS(Resource, Reference);
	REGULAR_CLASS(SceneTree, MainLoop);
	REGULAR_CLASS(SpatialMaterial, Material);
	REGULAR_CLASS(Texture, Resource);
	REGULAR_CLASS(Viewport, Node);
	REGULAR_CLASS(VisualInstance, CullInstance);

#undef CUSTOM_CLASS
#undef TEXT
#undef REGULAR_CLASS
#undef REGULAR_CLASS_DASH
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