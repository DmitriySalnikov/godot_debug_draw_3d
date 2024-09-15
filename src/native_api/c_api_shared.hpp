#pragma once

#include <sstream>
#include <string>
#include <typeinfo>

#if _MSC_VER
__pragma(warning(disable : 4244 26451 26495));
#endif
#include <godot_cpp/variant/string.hpp>
#if _MSC_VER
__pragma(warning(default : 4244 26451 26495));
#endif

namespace DD3DShared {

template <typename T>
struct FunctionSignature;

template <typename R, typename... Args>
struct FunctionSignature<R (*)(Args...)> {
	static godot::String get() {
		std::ostringstream oss;
		oss << typeid(R).name() << " (";
		((oss << getTypeName<Args>() << ", "), ...);
		return godot::String(oss.str().c_str()).trim_suffix(", ") + ")";
	}

private:
	template <typename U>
	static std::string getTypeName() {
		std::ostringstream oss;
		if constexpr (std::is_const<U>::value) {
			oss << "const ";
		}
		if constexpr (std::is_reference<U>::value) {
			oss << getTypeName<std::remove_reference_t<U> >() << "&";
		} else {
			oss << typeid(U).name();
		}
		return oss.str();
	}
};

template <typename Func>
godot::String get_function_signature(Func func) {
	return FunctionSignature<decltype(func)>::get();
}
} // namespace DD3DShared
