/* register_types.cpp */

/*
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
*/

#ifndef NO_EDITOR
#include "editor_plugin.h"
#endif
#include "data_graphs.h"
#include "debug_draw.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

DebugDraw3D *debug_draw_3d_singleton = nullptr;

/** GDNative Initialize **/
extern "C" void GDN_EXPORT initialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_class<DebugDraw3D>();
	ClassDB::register_class<GraphParameters>();

	debug_draw_3d_singleton = memnew(DebugDraw3D);
	Engine::get_singleton()->register_singleton(TEXT(DebugDraw3D), debug_draw_3d_singleton);
	// TODO EditorPlugin's is not available in GDExtensions...
	/*
#ifdef TOOLS_ENABLED
	ClassDB::register_class<DebugDraw3DEditorPlugin>();
#endif
	*/
}

/** GDNative Uninitialize **/
extern "C" void GDN_EXPORT uninitialize_debug_draw_3d_module(ModuleInitializationLevel p_level) {
	if (debug_draw_3d_singleton) {
		Engine::get_singleton()->unregister_singleton(TEXT(DebugDraw3D));
		memfree(debug_draw_3d_singleton);
		debug_draw_3d_singleton = nullptr;
	}
}

/** GDNative Initialize **/
extern "C" {
GDNativeBool GDN_EXPORT debug_draw_3d_library_init(const GDNativeInterface *p_interface, const GDNativeExtensionClassLibraryPtr p_library, GDNativeInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(initialize_debug_draw_3d_module);
	init_obj.register_terminator(uninitialize_debug_draw_3d_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
	/* TODO remove unused code from godot-cpp
	*
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
	*/
}
}
