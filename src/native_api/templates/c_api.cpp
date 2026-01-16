#ifdef NATIVE_API_ENABLED

//#define DD3D_ENABLE_MISMATCH_CHECKS

#include "native_api/c_api.h"

#include "native_api/c_api_shared.hpp"
#include "utils/utils.h"

// GENERATOR_DD3D_API_INCLUDES

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/os.hpp>
// GENERATOR_DD3D_GODOT_API_INCLUDES
GODOT_WARNING_RESTORE()

using namespace godot;

namespace NATIVE_API {

// GENERATOR_DD3D_FUNCTIONS

Dictionary get_functions() {
	ZoneScoped;

	static godot::Dictionary result;
	if (result.is_empty()) {
		Dictionary functions;
#ifdef DD3D_ENABLE_MISMATCH_CHECKS
#define ADD_FUNC(_name) functions[#_name] = Utils::make_dict("ptr", (int64_t)&_name, "signature", DD3DShared::FunctionSignature<decltype(&_name)>::get())
#else
#define ADD_FUNC(_name) functions[#_name] = Utils::make_dict("ptr", (int64_t)&_name)
#endif

		Dictionary classes;
#define ADD_CLASS(_name) classes[#_name] = Utils::make_dict("size", static_cast<int32_t>(sizeof(_name)))

		// GENERATOR_DD3D_REGISTRATIONS

#undef ADD_FUNC
#undef ADD_CLASS

		result["hash"] = functions.hash();
		result["functions"] = functions;
		result["classes"] = classes;
	}
	return result;
}

void clear_orphaned_refs() {
	ZoneScoped;

	// VMs with managed memory and garbage collector may not release resources until the program is completed.
	// Therefore, these warnings will be displayed almost always.
	bool warning_is_printed = false;
#define CLEAR_REFS(_class, _name)                                                                                                                                              \
	if (godot::OS::get_singleton()->is_stdout_verbose() && !_name.empty()) {                                                                                                   \
		WARN_PRINT("Verbose Output: " + godot::String(#_class) + ": Not all References (" + String::num_int64(_name.size()) + ") were cleared before DebugDraw3D shut down."); \
		warning_is_printed = true;                                                                                                                                             \
	}                                                                                                                                                                          \
	for (const auto &i : _name) {                                                                                                                                              \
		delete i;                                                                                                                                                              \
	}                                                                                                                                                                          \
	_name.clear();

	// GENERATOR_DD3D_REFS_CLEAR

	if (warning_is_printed) {
		WARN_PRINT("Verbose Output DD3D: If you are using a language with a garbage collector, then most likely everything is fine.");
	}

#undef CLEAR_REFS
}
} // namespace NATIVE_API

#endif