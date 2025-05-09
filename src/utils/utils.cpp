#include "utils.h"

#include <stdarg.h>

GODOT_WARNING_DISABLE()
#include <godot_cpp/classes/dir_access.hpp>
GODOT_WARNING_RESTORE()

using namespace godot;

const godot::Vector3 Vector3_ZERO = godot::Vector3(0, 0, 0);
const godot::Vector3 Vector3_INF = godot::Vector3(INFINITY, INFINITY, INFINITY);
const godot::Vector3 Vector3_ONE = godot::Vector3(1, 1, 1);
const godot::Vector3 Vector3_UP_OF_S = godot::Vector3(1, 1, 0).normalized();
const godot::Vector3 Vector3_UP_OF = godot::Vector3(1, 1, 1).normalized();
const godot::Vector3 Vector3_UP = godot::Vector3(0, 1, 0);
const godot::Vector3 Vector3_DOWN = godot::Vector3(0, -1, 0);
const godot::Vector3 Vector3_LEFT = godot::Vector3(-1, 0, 0);
const godot::Vector3 Vector3_RIGHT = godot::Vector3(1, 0, 0);
const godot::Vector3 Vector3_BACK = godot::Vector3(0, 0, 1);
const godot::Vector3 Vector3_FORWARD = godot::Vector3(0, 0, -1);

const godot::Quaternion Quaternion_IDENTITY = godot::Quaternion();

#if DEBUG_ENABLED
std::queue<Utils::LogData> Utils::log_buffer;
#endif

void Utils::_logv(bool p_err, bool p_force_print, const char *p_format, ...) {
	ZoneScoped;
#if DEBUG_ENABLED

	const int static_buf_size = 512;
	char static_buf[static_buf_size];
	char *buf = static_buf;

	va_list list_copy;
	va_start(list_copy, p_format);

	va_list p_list;
	va_copy(p_list, list_copy);
	int len = vsnprintf(buf, static_buf_size, p_format, p_list);
	va_end(p_list);

	std::string s;

	MSVC_WARNING_DISABLE(6387)

	if (len >= static_buf_size) {
		char *buf_alloc = (char *)malloc((size_t)len + 1);
		vsnprintf(buf_alloc, (size_t)len + 1, p_format, list_copy);
		s = buf_alloc;
		free(buf_alloc);
	} else {
		s = buf;
	}
	va_end(list_copy);

	MSVC_WARNING_RESTORE(6387)

	size_t hsh = std::hash<std::string>()(s);

	if (log_buffer.size() && log_buffer.back().hash == hsh) {
		log_buffer.back().repeat++;
	} else {
		log_buffer.push({ hsh, s, p_err, 1 });
	}

	if (p_force_print || log_buffer.size() > 5000) {
		print_logs();
	}
#endif
}

void Utils::print_logs() {
#if DEBUG_ENABLED
	while (log_buffer.size()) {
		auto &l = log_buffer.front();
		if (l.repeat > 1) {
			if (l.is_error) {
				fprintf(stderr, "[Error] [x%d] %s", l.repeat, l.text.c_str());
			} else {
				printf("[Info] [x%d] %s", l.repeat, l.text.c_str());
				// fflush(stdout);
			}
		} else {
			if (l.is_error) {
				fprintf(stderr, "[Error] %s", l.text.c_str());
			} else {
				printf("[Info] %s", l.text.c_str());
				// fflush(stdout);
			}
		}
		log_buffer.pop();
	}
#endif
}

Node *Utils::find_node_by_class(Node *start_node, const String &class_name) {
	ZoneScoped;
	for (int i = 0; i < start_node->get_child_count(); i++) {
		auto c = start_node->get_child(i);
		if (c->get_class() == class_name)
			return c;
		auto nc = find_node_by_class(c, class_name);
		if (nc)
			return nc;
	}
	return nullptr;
}

String Utils::get_scene_tree_as_string(Node *start) {
	ZoneScoped;
	String output = "";
	std::function<void(Node *, int)> get_node_tree;
	get_node_tree = [&output, &get_node_tree](Node *node, int depth) {
		for (int i = 0; i < node->get_child_count(); i++) {
			auto c = node->get_child(i);
			output = output + String("\t").repeat(depth) + String::num_int64(i) + ". " + c->get_class() + ":" + c->get_name() + "\n";
			get_node_tree(c, depth + 1);
		}
	};

	get_node_tree(start, 0);
	return output;
}

godot::String Utils::search_file(const godot::String start_path, const godot::String &file_name) {
	auto dir = DirAccess::open(start_path);
	if (dir.is_valid()) {
		dir->list_dir_begin();
		String name = dir->get_next();
		while (name != "") {
			if (dir->current_is_dir()) {
				String res = search_file(start_path.path_join(name), file_name);
				if (!res.is_empty()) {
					return res;
				}
			} else {
				if (name == file_name) {
					return start_path.path_join(name);
				}
			}
			name = dir->get_next();
		}
	} else {
		PRINT_ERROR("An error occurred when trying to access the path '{0}'.", start_path);
	}
	return godot::String();
}

void Utils::get_godot_version(int *major, int *minor, int *patch, int *version_sum) {
	if (major) {
		static int static_godot_major = (int)Engine::get_singleton()->get_version_info()["major"];
		*major = static_godot_major;
	}
	if (minor) {
		static int static_godot_minor = (int)Engine::get_singleton()->get_version_info()["minor"];
		*minor = static_godot_minor;
	}
	if (patch) {
		static int static_godot_patch = (int)Engine::get_singleton()->get_version_info()["patch"];
		*patch = static_godot_patch;
	}
	if (version_sum) {
		static int static_godot_version = (((char)(int)Engine::get_singleton()->get_version_info()["major"]) << 3 * 8) + (((char)(int)Engine::get_singleton()->get_version_info()["minor"]) << 2 * 8) + (((char)(int)Engine::get_singleton()->get_version_info()["patch"]) << 1 * 8);
		*version_sum = static_godot_version;
	}
}

bool Utils::is_current_godot_version_in_range(uint8_t min_major, uint8_t min_minor, uint8_t min_patch, uint8_t max_major, uint8_t max_minor, uint8_t max_patch) {
	ZoneScoped;
	int godot_version;
	get_godot_version(nullptr, nullptr, nullptr, &godot_version);

	int min_version_sum = ((int8_t)min_major << 3 * 8) + ((int8_t)min_minor << 2 * 8) + ((int8_t)min_patch << 1 * 8);
	int max_version_sum = ((int8_t)max_major << 3 * 8) + ((int8_t)max_minor << 2 * 8) + ((int8_t)max_patch << 1 * 8);

	bool min_has_value = min_major;
	bool max_has_value = max_major;

	if (min_has_value && max_has_value) {
		return godot_version >= min_version_sum && godot_version <= max_version_sum;
	} else if (min_has_value) {
		return godot_version >= min_version_sum;
	} else if (max_has_value) {
		return godot_version <= max_version_sum;
	}
	return false;
}

godot::PackedFloat32Array Utils::convert_packed_vector3_to_packed_float(godot::PackedVector3Array &arr) {
	ZoneScoped;

	godot::PackedFloat32Array p;
	long s = sizeof(float);

	if (!arr.is_empty()) {
#if REAL_T_IS_DOUBLE
		if (arr.size()) {
			p.resize(arr.size() * 3);
			auto *w = p.ptrw();
			for (int64_t i = 0; i < arr.size(); i++) {
				const godot::Vector3 &v = arr[i];
				w[i * 3 + 0] = (float)v.x;
				w[i * 3 + 1] = (float)v.y;
				w[i * 3 + 2] = (float)v.z;
			}
		}
#else
		auto *data = arr.ptr();
		if (data) {
			p.resize(arr.size() * sizeof(arr[0]) / s);
			memcpy(p.ptrw(), data, sizeof(arr[0]) * arr.size());
		}
#endif
	}
	return p;
}
