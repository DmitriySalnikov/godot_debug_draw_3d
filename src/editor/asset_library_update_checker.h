#pragma once

#ifndef DISABLE_DEBUG_RENDERING
#ifdef DEBUG_ENABLED

#include "utils/compiler.h"

#include <thread>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/http_client.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

class _DebugDraw3DAssetLibraryUpdateChecker : public RefCounted {
	GDCLASS(_DebugDraw3DAssetLibraryUpdateChecker, RefCounted)

private:
	std::thread http_thread;
	bool is_thread_closing = false;
	int addon_id;
	String addon_name;
	String repository_name;
	String root_settings_section;
	String changes_page;

	String godot_domain;
	String godot_asset_api;
	String godot_asset_page;

protected:
	static void _bind_methods();

public:
	void request_completed(String body);
	void init();

	_DebugDraw3DAssetLibraryUpdateChecker();
	~_DebugDraw3DAssetLibraryUpdateChecker();
};

#endif
#endif
