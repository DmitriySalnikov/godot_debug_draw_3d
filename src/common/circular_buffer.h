#pragma once

#include <memory>
#include <mutex>

template <typename TValue>
class CircularBuffer {
	std::unique_ptr<TValue[]> buffer;
	size_t buf_size;
	size_t start;
	size_t end;
	bool _is_filled;

public:
	CircularBuffer() :
			buffer(nullptr),
			buf_size(0),
			start(0),
			end(0),
			_is_filled(false) {
	}

	CircularBuffer(size_t p_buffer_size) :
			buffer(new TValue[p_buffer_size]),
			buf_size(p_buffer_size),
			start(0),
			end(0),
			_is_filled(false) {
	}

	CircularBuffer<TValue> &operator=(const CircularBuffer<TValue> &other) {
		if (this == &other)
			return *this;

		reset();
		buf_size = other.buf_size;
		buffer.reset(new TValue[buf_size]);

		return *this;
	}

	void reset() {
		start = 0;
		end = 0;
		_is_filled = 0;
	}

	void resize(size_t p_size) {
		reset();
		buf_size = p_size;
		buffer.reset(new TValue[buf_size]);
	}

	size_t size() const {
		return _is_filled ? buf_size : end - start;
	}

	size_t buffer_size() const {
		return buf_size;
	}

	bool is_filled() const {
		return _is_filled;
	}

	void add(TValue p_v) {
		buffer[end++] = p_v;

		if (end == buf_size) {
			_is_filled = true;
			end = 0;
		}
		if (_is_filled) {
			if (++start == buf_size) {
				start = 0;
			}
		}
	}

	TValue get(const size_t &p_idx) const {
		const size_t pos = start + p_idx;
		return buffer[pos >= buf_size ? pos - buf_size : pos];
	}

	void get_min_max_avg(TValue *p_min, TValue *p_max, TValue *p_avg) {
		if (size()) {
			TValue sum = get(0);
			*p_min = get(0);
			*p_max = get(0);

			for (size_t i = 1; i < size(); i++) {
				TValue v = get(i);
				if (v < *p_min) {
					*p_min = v;
				} else if (v > *p_max) {
					*p_max = v;
				}
				sum += v;
			}
			*p_avg = sum / size();
			return;
		}

		*p_min = *p_max = *p_avg = 0;
		return;
	}
};
