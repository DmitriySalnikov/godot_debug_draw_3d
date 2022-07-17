/* register_types.cpp */

#include "register_types.h"
#include <ConfigFile.hpp>
#include <Directory.hpp>
#include <EditorImportPlugin.hpp>
#include <EditorPlugin.hpp>
#include <File.hpp>
#include <Godot.hpp>
#include <ImageTexture.hpp>
#include <ProjectSettings.hpp>
#include <Resource.hpp>
#include <ResourceFormatSaver.hpp>
#include <ResourceImporter.hpp>
#include <Texture.hpp>

#ifndef NO_EDITOR
#include "qoi_import.h"
#include "qoi_plugin.h"
#endif
#include "qoi_utils.h"
#include "qoi_wrapper.h"

using namespace godot;

/** GDNative Initialize **/
extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	Godot::gdnative_init(o);

	// Custom register and init for only needed classes
	// Works only with my patches
	godot::_TagDB::register_global_type("Image", typeid(Image).hash_code(), typeid(Resource).hash_code());
	godot::_TagDB::register_global_type("Object", typeid(Object).hash_code(), 0);
	godot::_TagDB::register_global_type("Resource", typeid(Resource).hash_code(), typeid(Reference).hash_code());
	godot::_TagDB::register_global_type("ProjectSettings", typeid(ProjectSettings).hash_code(), typeid(Object).hash_code());
	godot::_TagDB::register_global_type("EditorPlugin", typeid(EditorPlugin).hash_code(), typeid(Node).hash_code());
	godot::_TagDB::register_global_type("ResourceImporter", typeid(ResourceImporter).hash_code(), typeid(Reference).hash_code());
	godot::_TagDB::register_global_type("EditorImportPlugin", typeid(EditorImportPlugin).hash_code(), typeid(ResourceImporter).hash_code());
	godot::_TagDB::register_global_type("ConfigFile", typeid(ConfigFile).hash_code(), typeid(Reference).hash_code());
	godot::_TagDB::register_global_type("Reference", typeid(Reference).hash_code(), typeid(Object).hash_code());
	godot::_TagDB::register_global_type("Texture", typeid(Texture).hash_code(), typeid(Resource).hash_code());
	godot::_TagDB::register_global_type("ImageTexture", typeid(ImageTexture).hash_code(), typeid(Texture).hash_code());
	godot::_TagDB::register_global_type("_Directory", typeid(Directory).hash_code(), typeid(Reference).hash_code());
	godot::_TagDB::register_global_type("_File", typeid(File).hash_code(), typeid(Reference).hash_code());

	Image::___init_method_bindings();
	Object::___init_method_bindings();
	Resource::___init_method_bindings();
	ProjectSettings::___init_method_bindings();
	EditorPlugin::___init_method_bindings();
	ResourceImporter::___init_method_bindings();
	EditorImportPlugin::___init_method_bindings();
	ConfigFile::___init_method_bindings();
	Reference::___init_method_bindings();
	Texture::___init_method_bindings();
	ImageTexture::___init_method_bindings();
	Directory::___init_method_bindings();
	File::___init_method_bindings();
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

	register_tool_class<QOI>();
	register_tool_class<QOIUtils>();

#ifndef NO_EDITOR
	register_tool_class<QOIPlugin>();
	register_tool_class<QOIImport>();
#endif
}