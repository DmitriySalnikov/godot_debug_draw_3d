#pragma once

#include <memory>
#include <mutex>

template <typename TValue>
class CircularBuffer {
	std::unique_ptr<TValue[]> buffer;
	size_t buf_size;
	size_t start;
	size_t end;
	bool is_filled;

public:
	CircularBuffer() :
			buffer(nullptr),
			buf_size(0),
			start(0),
			end(0),
			is_filled(false) {
	}

	CircularBuffer(size_t _buffer_size) :
			buffer(new TValue[_buffer_size]),
			buf_size(_buffer_size),
			start(0),
			end(0),
			is_filled(false) {
	}

	void reset() {
		start = 0;
		end = 0;
		is_filled = 0;
	}

	size_t size() const {
		return is_filled ? buf_size : end - start;
	}

	size_t buffer_size() const {
		return buf_size;
	}

	void add(TValue v) {
		buffer[end] = v;

		end++;
		if (end == buf_size) {
			is_filled = true;
			end = 0;
		}
		if (is_filled) {
			start++;
			if (start == buf_size) {
				start = 0;
			}
		}
	}

	TValue operator[](const size_t &idx) const {
		size_t pos = start + idx;
		if (pos >= buf_size) {
			pos -= buf_size;
		}
		return buffer[pos];
	}
};
