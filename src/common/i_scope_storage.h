#pragma once

#include "utils/compiler.h"

#include <functional>
#include <memory>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/ref.hpp>
GODOT_WARNING_RESTORE()
using namespace godot;

template <class TCfgStorage, class TCfgStorageData>
class IScopeStorage {
private:
#ifndef DISABLE_DEBUG_RENDERING
	virtual void _register_scoped_config(uint64_t thread_id, uint64_t guard_id, TCfgStorage *cfg) = 0;
	virtual void _unregister_scoped_config(uint64_t thread_id, uint64_t guard_id) = 0;
	virtual void _clear_scoped_configs() = 0;

	virtual const std::shared_ptr<TCfgStorageData> scoped_config_for_current_thread() = 0;
#endif

public:
	typedef std::function<void(uint64_t, uint64_t)> unregister_func;

	virtual Ref<TCfgStorage> scoped_config() = 0;
};
