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
		buf = (char*)malloc((size_t)len + 1);
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

const Color Colors::empty_color = Color(0, 0, 0, 0);
const Color Colors::axis_x = Color(0.9f, 0.03f, 0.08f, 0);
const Color Colors::axis_y = Color(0.24f, 0.67f, 0, 0);
const Color Colors::axis_z = Color(0.02f, 0.26f, 0.9f, 0);
const Color Colors::black = Color(0, 0, 0, 1);
const Color Colors::chartreuse = Color(0.5f, 1, 0, 1);
const Color Colors::crimson = Color(0.86f, 0.08f, 0.24f, 1);
const Color Colors::dark_orange = Color(1, 0.55f, 0, 1);
const Color Colors::dark_salmon = Color(0.91f, 0.59f, 0.48f, 1);
const Color Colors::debug_bounds = Color(1, 0.55f, 0, 1);
const Color Colors::dodgerblue = Color(0.12f, 0.56f, 1, 1);
const Color Colors::forest_green = Color(0.13f, 0.55f, 0.13f, 1);
const Color Colors::gray_bg = Color(0.3f, 0.3f, 0.3f, 0.8f);
const Color Colors::gray_graph_bg = Color(0.2f, 0.2f, 0.2f, 0.6f);
const Color Colors::green = Color(0, 1, 0, 1);
const Color Colors::light_green = Color(0.56f, 0.93f, 0.56f, 1);
const Color Colors::orange = Color(1, 0.65f, 0, 1);
const Color Colors::orange_red = Color(1, 0.27f, 0, 1);
const Color Colors::red = Color(1, 0, 0, 1);
const Color Colors::white = Color(1, 1, 1);
const Color Colors::white_smoke = Color(0.96f, 0.96f, 0.96f, 1);
const Color Colors::yellow = Color(1, 1, 0, 1);