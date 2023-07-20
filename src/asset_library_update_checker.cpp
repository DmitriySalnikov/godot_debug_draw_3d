#include "asset_library_update_checker.h"

#ifndef DISABLE_DEBUG_RENDERING
#ifdef DEBUG_ENABLED

#include "version.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/window.hpp>
GODOT_WARNING_RESTORE()

using namespace godot;

void AssetLibraryUpdateChecker::_bind_methods() {
#define REG_CLASS_NAME AssetLibraryUpdateChecker

	REG_METHOD(request_completed);
	REG_METHOD(init);

#undef REG_CLASS_NAME
}

void AssetLibraryUpdateChecker::request_completed(int result, int response_code, PackedStringArray headers, PackedByteArray body) {
	request->queue_free();
	request = nullptr;

	if (response_code != 200)
		return;

	Dictionary dict = JSON::parse_string(body.get_string_from_utf8());
	String ver = dict["version_string"];
	String download_url = dict["download_url"];
	Array split = ver.split(".");

	int ver_num = 0;
	if (split.size() >= 1 && split.size() <= 4) {
		for (int i = 0; i < split.size(); i++) {
			char num = (char)((String)split[i]).to_int();
			ver_num += num << (3 - i) * 8;
		}
	}

	if (ver_num > DD3D_VERSION) {
		PRINT(String("=====\n") +
				"An update for " + addon_name + " is available. Current: " + DD3D_VERSION_STR + " -> New: " + ver + "\n" +
				"It will not be possible to update the addon through the editor, since its files are currently locked.\n" +
				"You need to download the archive from this link (select the URL below, click copy and paste this address into your browser):\n" + download_url + "\n" +
				"Then close the editor and replace the files in your project with new ones from the archive\n" +
				"(In the archive, first go inside the " + repository_name + "-XXXXX... folder and extract its contents).\n" +
				"To disable this alert, go to the Project Settings in the \"" + root_settings_section + "\" section\n"+
				"=====");
	}
}

void AssetLibraryUpdateChecker::init() {
	if (!IS_EDITOR_HINT())
		return;

	request = memnew(HTTPRequest);
	SCENE_ROOT()->add_child(request);

	request->connect("request_completed", Callable(this, NAMEOF(request_completed)));
	request->request("https://godotengine.org/asset-library/api/asset/1766");
}

AssetLibraryUpdateChecker::AssetLibraryUpdateChecker() {
	addon_name = "Debug Draw 3D";
	repository_name = "godot_debug_draw_3d";
	root_settings_section = "debug_draw_3d/settings/";

	DEFINE_SETTING_READ_ONLY(root_settings_section + "addon_version", DD3D_VERSION_STR, Variant::STRING);
	DEFINE_SETTING_AND_GET(bool check_updates, root_settings_section + "check_for_updates", true, Variant::BOOL);

	if (check_updates)
		call_deferred(NAMEOF(init));
}

#endif
#endif