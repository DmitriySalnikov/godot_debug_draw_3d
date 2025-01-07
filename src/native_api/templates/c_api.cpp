#ifdef NATIVE_API_ENABLED

#include "native_api/c_api.h"

#include "native_api/c_api_shared.hpp"
#include "utils/utils.h"

// GENERATOR_DD3D_API_INCLUDES

using namespace godot;

namespace NATIVE_API {

// GENERATOR_DD3D_FUNCTIONS_DEFINES

Dictionary get_functions() {
	ZoneScoped;

	static Dictionary result;
	if (result.is_empty()) {
		Dictionary functions;
#define ADD_FUNC(_name) functions[#_name] = Utils::make_dict("ptr", (int64_t) & _name, "signature", DD3DShared::get_function_signature(&_name))

		// GENERATOR_DD3D_FUNCTIONS_REGISTERS

#undef ADD_FUNC

		result["hash"] = functions.hash();
		result["functions"] = functions;
	}
	return result;
}

void clear_orphaned_refs() {
	ZoneScoped;
	
#define CLEAR_REFS(_class, _name)                                                              \
	if (!_name.empty())                                                                        \
		WARN_PRINT(#_class ": Not all References were cleared before DebugDraw3D shut down."); \
	for (const auto &i : _name) {                                                              \
		delete i;                                                                              \
	}                                                                                          \
	_name.clear();

	// GENERATOR_DD3D_REFS_CLEAR

#undef CLEAR_REFS
}
} // namespace NATIVE_API

#endif