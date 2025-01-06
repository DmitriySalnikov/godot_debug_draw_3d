#ifdef NATIVE_API_ENABLED

#include "native_api/c_api.h"

#include "native_api/c_api_shared.hpp"
#include "utils/utils.h"

#include "2d/debug_draw_2d.h"
#include "3d/config_scope_3d.h"
#include "3d/debug_draw_3d.h"
#include "debug_draw_manager.h"

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
} // namespace NATIVE_API

#endif