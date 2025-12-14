#ifdef NATIVE_API_ENABLED

//#define DD3D_ENABLE_MISMATCH_CHECKS

#include "native_api/c_api.h"

#include "native_api/c_api_shared.hpp"
#include "utils/utils.h"

// GENERATOR_DD3D_API_INCLUDES

GODOT_WARNING_DISABLE()
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

		// GENERATOR_DD3D_FUNCTIONS_REGISTRATIONS

#undef ADD_FUNC

		result["hash"] = functions.hash();
		result["functions"] = functions;
	}
	return result;
}

void clear_orphaned_refs() {
	ZoneScoped;

#define CLEAR_REFS(_class, _name)                                                                               \
	if (!_name.empty())                                                                                         \
		WARN_PRINT(godot::String(#_class) + ": Not all References were cleared before DebugDraw3D shut down."); \
	for (const auto &i : _name) {                                                                               \
		delete i;                                                                                               \
	}                                                                                                           \
	_name.clear();

	// GENERATOR_DD3D_REFS_CLEAR

#undef CLEAR_REFS
}
} // namespace NATIVE_API

#endif