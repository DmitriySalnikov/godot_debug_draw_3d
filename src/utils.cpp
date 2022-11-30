#include "utils.h"

#include "render_instances.h"
#include <stdarg.h>

using namespace godot;

#if _DEBUG
std::queue<Utils::LogData> Utils::log_buffer;
#endif

void Utils::logv(const char *p_format, bool p_err, bool p_force_print, ...) {
#if _DEBUG

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
#if _DEBUG
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
