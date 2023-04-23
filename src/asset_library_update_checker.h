#pragma once

#ifdef DEBUG_ENABLED

#include "utils.h"

MSVC_WARNING_DISABLE(4244)
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/http_request.hpp>
MSVC_WARNING_RESTORE(4244)

using namespace godot;

class AssetLibraryUpdateChecker : public RefCounted {
	GDCLASS(AssetLibraryUpdateChecker, RefCounted)

private:
	HTTPRequest *request = nullptr;

protected:
	static void _bind_methods();

public:
	void request_completed(int result, int response_code, PackedStringArray headers, PackedByteArray body);
	void init();

	AssetLibraryUpdateChecker();
};

#endif