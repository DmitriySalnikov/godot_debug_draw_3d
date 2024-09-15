#include "asset_library_update_checker.h"

#ifndef DISABLE_DEBUG_RENDERING
#ifdef DEBUG_ENABLED

#include "utils/utils.h"
#include "version.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/json.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

void _DebugDraw3DAssetLibraryUpdateChecker::_bind_methods() {
#define REG_CLASS_NAME _DebugDraw3DAssetLibraryUpdateChecker

	REG_METHOD(request_completed);
	REG_METHOD(init);

#undef REG_CLASS_NAME
}

void _DebugDraw3DAssetLibraryUpdateChecker::request_completed(String body) {
	ZoneScoped;
	if (http_thread.joinable()) {
		is_thread_closing = true;
		http_thread.join();
	}

	Ref<JSON> json;
	json.instantiate();
	Error err = json->parse(body);
	Variant p = json->get_data();

	if (err != Error::OK || p.get_type() != Variant::Type::DICTIONARY) {
		DEV_PRINT_STD_ERR("%s: Failed to parse the response body.\n", NAMEOF(_DebugDraw3DAssetLibraryUpdateChecker));
		return;
	}

	Dictionary dict = p;
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

void _DebugDraw3DAssetLibraryUpdateChecker::init() {
	ZoneScoped;

	http_thread = std::thread([&]() {
		Ref<HTTPClient> http = nullptr;
		http.instantiate();

		Error err = Error::OK;

		if (http.is_valid() && !is_thread_closing) {
			http->connect_to_host(godot_domain);

			err = http->poll();
			if (err != Error::OK) {
				PRINT_ERROR("{0}: Failed to initialize connection. Error: {1}", NAMEOF(_DebugDraw3DAssetLibraryUpdateChecker), UtilityFunctions::error_string(err));
				return;
			}
		} else {
			return;
		}

		HTTPClient::Status prev_status = HTTPClient::STATUS_DISCONNECTED;
		while (http.is_valid() && !is_thread_closing) {
			err = http->poll();
			if (err != Error::OK) {
				PRINT_ERROR("{0}: Failed to connect. Error: {1}", NAMEOF(_DebugDraw3DAssetLibraryUpdateChecker), UtilityFunctions::error_string(err));
				return;
			}

			HTTPClient::Status status = http->get_status();
			switch (status) {
				case godot::HTTPClient::STATUS_DISCONNECTED:
				case godot::HTTPClient::STATUS_CONNECTION_ERROR:
				case godot::HTTPClient::STATUS_CANT_RESOLVE:
				case godot::HTTPClient::STATUS_CANT_CONNECT:
				case godot::HTTPClient::STATUS_TLS_HANDSHAKE_ERROR:
					PRINT_ERROR("{0}: Connection error: {1}", NAMEOF(_DebugDraw3DAssetLibraryUpdateChecker), status);
					return;
				case godot::HTTPClient::STATUS_RESOLVING:
				case godot::HTTPClient::STATUS_CONNECTING:
				case godot::HTTPClient::STATUS_REQUESTING:
				case godot::HTTPClient::STATUS_BODY:
				default:
					if (status != prev_status) {
						DEV_PRINT_STD("%s: Connecting status: %d\n", NAMEOF(_DebugDraw3DAssetLibraryUpdateChecker), status);
					}
					break;
				case godot::HTTPClient::STATUS_CONNECTED:
					goto connected;
			}
			prev_status = status;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (http.is_null() || is_thread_closing) {
			return;
		}

	connected:

		String request_url = "/asset-library/api/asset/" + String::num_int64(addon_id);
		err = http->request(HTTPClient::METHOD_GET, request_url, PackedStringArray());
		if (err != Error::OK) {
			PRINT_ERROR("{0}: Failed to create a request. Error: {1}", NAMEOF(_DebugDraw3DAssetLibraryUpdateChecker), UtilityFunctions::error_string(err));
			return;
		}

		while (http.is_valid() && !is_thread_closing) {

			err = http->poll();
			if (err != Error::OK) {
				PRINT_ERROR("{0}: Failed to get a response from \"{1}\". Error: {2}", NAMEOF(_DebugDraw3DAssetLibraryUpdateChecker), godot_domain + request_url, UtilityFunctions::error_string(err));
				return;
			}

			HTTPClient::ResponseCode code = (HTTPClient::ResponseCode)http->get_response_code();
			if (code == HTTPClient::ResponseCode::RESPONSE_OK) {
				PackedByteArray res;
				PackedByteArray tmp = http->read_response_body_chunk();
				while (!tmp.is_empty() && !is_thread_closing) {
					res.append_array(tmp);
					if (http->get_status() != HTTPClient::STATUS_BODY) {
						break;
					}
					tmp = http->read_response_body_chunk();
				}

				call_deferred(NAMEOF(request_completed), res.get_string_from_utf8());
				return;
			} else {
				if (code != 0) {
					PRINT_ERROR("{0}: Failed to get a response from \"{1}\". Code: {2}", NAMEOF(_DebugDraw3DAssetLibraryUpdateChecker), godot_domain + request_url, code);
					return;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		DEV_PRINT_STD("%s: Thread finished\n", NAMEOF(_DebugDraw3DAssetLibraryUpdateChecker));
	});
}

_DebugDraw3DAssetLibraryUpdateChecker::_DebugDraw3DAssetLibraryUpdateChecker() {
	addon_id = 1766;
	addon_name = "Debug Draw 3D";
	repository_name = "godot_debug_draw_3d";
	root_settings_section = String(Utils::root_settings_section) + "updates/";
	changes_page = "https://github.com/DmitriySalnikov/godot_debug_draw_3d/releases";

	godot_domain = "https://godotengine.org";
	godot_asset_api = godot_domain + "/asset-library/api/asset/";
	godot_asset_page = godot_domain + "/asset-library/asset/";

	DEFINE_SETTING_READ_ONLY(root_settings_section + "addon_version", DD3D_VERSION_STR, Variant::STRING);
	DEFINE_SETTING_READ_ONLY(root_settings_section + "addon_page", godot_asset_page + String::num_int64(addon_id), Variant::STRING);
	DEFINE_SETTING_AND_GET(bool check_updates, root_settings_section + "check_for_updates", true, Variant::BOOL);

	if (IS_EDITOR_HINT() && check_updates)
		call_deferred(NAMEOF(init));
}

_DebugDraw3DAssetLibraryUpdateChecker::~_DebugDraw3DAssetLibraryUpdateChecker() {
	if (http_thread.joinable()) {
		is_thread_closing = true;
		http_thread.join();
	}
}

#endif
#endif
