#pragma once

#include "circular_buffer.h"

#include <Godot.hpp>

#include <algorithm>
#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#define TEXT(s) #s

#if _DEBUG && _MSC_VER
#ifndef _CRT_STRINGIZE
#define _CRT_STRINGIZE_(x) #x
#define _CRT_STRINGIZE(x) _CRT_STRINGIZE_(x)
#endif

// Expands to macro:
#define EXPAND_MACRO(x) __pragma(message(__FILE__ _CRT_STRINGIZE((__LINE__) \
																 : \nmacro\t) #x " expands to:\n" _CRT_STRINGIZE(x)))
#else
#define EXPAND_MACRO(x)
#endif

#pragma region PRINTING

#if _DEBUG
#define DEBUG_PRINT(text) Godot::print(Variant(text))
#else
#define DEBUG_PRINT(text)
#endif

#define PRINT(text) Godot::print(Variant(text))
#define PRINT_ERROR(text) Godot::print_error(Variant(text), __FUNCTION__, get_file_name_in_repository(__FILE__), __LINE__)
#define PRINT_WARNING(text) Godot::print_warning(Variant(text), __FUNCTION__, get_file_name_in_repository(__FILE__), __LINE__)

namespace godot {
static String get_file_name_in_repository(String name) {
	if (name != "") {
		int idx = name.find("debug_draw_3d");
		if (idx != -1)
			name = name.substr(name.find("debug_draw_3d"), name.length());
	}
	return name;
}
} // namespace godot

#pragma endregion PRINTING

#define IS_EDITOR_HINT() Engine::get_singleton()->is_editor_hint()
#define DEBUG_DRAW() DebugDraw3D::get_singleton()
#define TIME_NOW() std::chrono::steady_clock::now()
#define TIME_TO_MS(time) std::chrono::duration_cast<std::chrono::milliseconds>(time)

#define C_ARR_SIZE(arr) (sizeof(arr) / sizeof(*arr))
#define LOCK_GUARD(_mutex) std::lock_guard<std::mutex> __guard(_mutex)
#define LOCK_GUARD_REC(_mutex) std::lock_guard<std::recursive_mutex> __guard(_mutex)

class Utils {
public:
	// https://stackoverflow.com/a/26221725/8980874
	template <typename... Args>
	static godot::String string_format(const wchar_t *format, Args... args) {
		int size_s = std::swprintf(nullptr, 0, format, args...) + 1; // Extra space for '\0'
		if (size_s <= 0) {
			throw std::runtime_error("Error during formatting.");
		}
		auto size = static_cast<size_t>(size_s);
		std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
		std::swprintf(buf.get(), size, format, args...);
		return godot::String(buf.get());
	}

	template <class TVal, class TFun>
	static void remove_where(std::unordered_set<TVal> *set, TFun checker_bool_val, std::function<void(TVal)> deleter = nullptr) {
		std::unordered_set<TVal> to_remove;
		for (const TVal &t : *set) {
			if (checker_bool_val(t)) {
				to_remove.insert(t);
			}
		}
		for (const TVal &t : to_remove) {
			if (deleter) {
				deleter(t);
			}
			(*set).erase(t);
		}
	}

	template <class TVal, class TFun>
	static int sum(std::unordered_set<TVal> *set, TFun getter_int_val) {
		int res = 0;
		for (const TVal &t : *set) {
			res += getter_int_val(t);
		}
		return res;
	}

	template <class TVal, class TFun>
	inline static std::vector<TVal> order_by(std::unordered_set<TVal> *set, TFun comparator_bool_tval_tval) {
		std::vector<TVal> ordered((*set).begin(), (*set).end());

		std::sort(ordered.begin(), ordered.end(), comparator_bool_tval_tval);
		return ordered;
	}

	template <class TVal, class TFun>
	static bool contains(const std::unordered_set<TVal> *set, TFun contains_bool_tval) {
		for (const TVal &d : *set) {
			if (contains_bool_tval(d))
				return true;
		}
		return false;
	}

	template <class TVal, class TFun>
	static TVal find(const std::unordered_set<TVal> *set, TFun contains_bool_tval) {
		for (const TVal &d : *set) {
			if (contains_bool_tval(d))
				return d;
		}
		return TVal();
	}

	template <class TVal, class TFun>
	static TVal find(const std::vector<TVal> *set, TFun contains_bool_tval) {
		for (const TVal &d : *set) {
			if (contains_bool_tval(d))
				return d;
		}
		return TVal();
	}

	template <class TVal>
	static void get_min_max_avg(const std::shared_ptr<CircularBuffer<TVal> > *arr, TVal *min, TVal *max, TVal *avg) {
		if ((*arr)->size()) {
			TVal sum = (*arr)->get(0);
			*min = (*arr)->get(0);
			*max = (*arr)->get(0);

			for (size_t i = 1; i < (*arr)->size(); i++) {
				TVal v = (*arr)->get(i);
				if (v < *min) {
					*min = v;
				} else if (v > *max) {
					*max = v;
				}
				sum += v;
			}
			*avg = sum / (*arr)->size();
			return;
		}

		*min = *max = *avg = 0;
		return;
	}
};

class Colors {
public:
	const static godot::Color empty_color;
	const static godot::Color black;
	const static godot::Color chartreuse;
	const static godot::Color crimson;
	const static godot::Color dark_orange;
	const static godot::Color dark_salmon;
	const static godot::Color forest_green;
	const static godot::Color gray_bg;
	const static godot::Color gray_graph_bg;
	const static godot::Color green;
	const static godot::Color light_green;
	const static godot::Color orange_red;
	const static godot::Color orange;
	const static godot::Color red;
	const static godot::Color white_smoke;
	const static godot::Color white;
	const static godot::Color yellow;
};
