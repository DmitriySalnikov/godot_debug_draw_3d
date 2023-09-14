#include "asset_library_update_checker.h"

#ifndef DISABLE_DEBUG_RENDERING
#ifdef DEBUG_ENABLED

#include "utils/utils.h"
#include "version.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
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
	String addon_godot_version = dict["godot_version"];

	int ver_code = 0;
	{
		Array split = ver.split(".");
		if (split.size() >= 1 && split.size() <= 4) {
			for (int i = 0; i < split.size(); i++) {
				String str = (String)split[i];
				if (str.is_valid_int()) {
					char num = (char)str.to_int();
					ver_code += num << (3 - i) * 8;
				} else {
					break;
				}
			}
		}
	}

	int addon_ver_code = 0;
	int godot_ver_code = 0;
	String godot_ver_str = "";
	{
		Dictionary godot_ver = Engine::get_singleton()->get_version_info();
		godot_ver_code = (((char)(int)godot_ver["major"]) << 3 * 8) + (((char)(int)godot_ver["minor"]) << 2 * 8) + (((char)(int)godot_ver["patch"]) << 1 * 8);
		godot_ver_str = godot_ver["major"].stringify() + "." + godot_ver["minor"].stringify() + "." + godot_ver["patch"].stringify();

		Array godot_split = addon_godot_version.split(".");
		if (godot_split.size() >= 1 && godot_split.size() <= 4) {
			for (int i = 0; i < godot_split.size(); i++) {
				String str = (String)godot_split[i];
				if (str.is_valid_int()) {
					char num = (char)str.to_int();
					addon_ver_code += num << (3 - i) * 8;
				} else {
					break;
				}
			}
		}
	}

	bool is_godot_version_incompatible = addon_ver_code > godot_ver_code;

	if (ver_code > DD3D_VERSION) {
		String warning = String("=====\n") +
						 "An update for [b]" + addon_name + "[/b] is available. Current: [i]" + DD3D_VERSION_STR + "[/i] -> New: [i]" + ver + "[/i]\n";

		if (is_godot_version_incompatible) {
			warning += "This version of Godot ([i]" + godot_ver_str + "[/i]) is not compatible with the new version of the addon (requires [i]" + addon_godot_version + "+[/i])!\n";
		} else {
			warning += "It will not be possible to update the addon through the editor, since its files are currently locked.\n";
			warning += "You need to download the archive from this link:\n[url]" + download_url + "[/url]\n";
			warning += "Then close the editor and replace the files in your project with new ones from the archive ";
			warning += "(extract the content of the [i]" + repository_name + "-*****[/i] folder).\n";
		}

		warning += "To disable this alert, go to the [b]Project Settings[/b] in the \"[u]" + root_settings_section + "[/u]\" section.\n";
		warning += "Addon page: [url]" + godot_asset_page + String::num_int64(addon_id) + "[/url]\n";
		warning += "Changes: [url]" + changes_page + "[/url]\n";
		warning += "=====";
		godot::UtilityFunctions::print_rich(godot::Variant(warning));
	}
}

void AssetLibraryUpdateChecker::init() {
	if (!IS_EDITOR_HINT())
		return;

	request = memnew(HTTPRequest);
	SCENE_ROOT()->add_child(request);

	request->connect("request_completed", Callable(this, NAMEOF(request_completed)));
	request->request(godot_asset_api + String::num_int64(addon_id));
}

AssetLibraryUpdateChecker::AssetLibraryUpdateChecker() {
	addon_id = 1766;
	addon_name = "Debug Draw 3D";
	repository_name = "godot_debug_draw_3d";
	root_settings_section = "debug_draw_3d/settings/";
	changes_page = "https://github.com/DmitriySalnikov/godot_debug_draw_3d/releases";

	godot_asset_api = "https://godotengine.org/asset-library/api/asset/";
	godot_asset_page = "https://godotengine.org/asset-library/asset/";

	DEFINE_SETTING_READ_ONLY(root_settings_section + "addon_version", DD3D_VERSION_STR, Variant::STRING);
	DEFINE_SETTING_READ_ONLY(root_settings_section + "addon_page", godot_asset_page + String::num_int64(addon_id), Variant::STRING);
	DEFINE_SETTING_AND_GET(bool check_updates, root_settings_section + "check_for_updates", true, Variant::BOOL);

	if (check_updates)
		call_deferred(NAMEOF(init));
}

AssetLibraryUpdateChecker::~AssetLibraryUpdateChecker() {
	if (UtilityFunctions::is_instance_valid(request))
		request->queue_free();
	request = nullptr;
}

#endif
#endif