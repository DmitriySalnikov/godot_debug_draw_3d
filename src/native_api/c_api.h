#pragma once

#ifdef NATIVE_API_ENABLED

#include "utils/compiler.h"

GODOT_WARNING_DISABLE()
#include <godot_cpp/variant/dictionary.hpp>
GODOT_WARNING_RESTORE()

namespace NATIVE_API {
godot::Dictionary get_functions();
}

#endif