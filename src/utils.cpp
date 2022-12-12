#include "utils.h"

#include "render_instances.h"
#include <stdarg.h>

using namespace godot;

#if DEBUG_ENABLED
std::queue<Utils::LogData> Utils::log_buffer;
#endif

void Utils::logv(const char *p_format, bool p_err, bool p_force_print, ...) {
#if DEBUG_ENABLED

	const int static_buf_size = 512;
	char static_buf[static_buf_size];
	char *buf = static_buf;

	va_list list_copy;
	va_start(list_copy, (const char *)p_force_print);

	va_list p_list;
	va_copy(p_list, list_copy);
	int len = vsnprintf(buf, static_buf_size, p_format, p_list);
	va_end(p_list);

	if (len >= static_buf_size) {
		buf = (char *)malloc((size_t)len + 1);
		vsnprintf(buf, (size_t)len + 1, p_format, list_copy);
	}
	va_end(list_copy);

	std::string s = buf;

	if (len >= static_buf_size) {
		free(buf);
	}

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
				fprintf(stderr, "[x%d] %s", l.repeat, l.text.c_str());
			} else {
				printf("[x%d] %s", l.repeat, l.text.c_str());
				// fflush(stdout);
			}
		} else {
			if (l.is_error) {
				fprintf(stderr, l.text.c_str());
			} else {
				printf(l.text.c_str());
				// fflush(stdout);
			}
		}
		log_buffer.pop();
	}
#endif
}

Node *Utils::find_node_by_class(Node *start_node, String *class_name) {
	for (int i = 0; i < start_node->get_child_count(); i++) {
		auto c = start_node->get_child(i);
		if (c->get_class() == *class_name)
			return c;
		auto nc = find_node_by_class(c, class_name);
		if (nc)
			return nc;
	}
	return nullptr;
}

godot::String Utils::get_scene_tree_as_string(godot::Node *start) {
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
};