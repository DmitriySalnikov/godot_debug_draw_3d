#pragma once

GODOT_WARNING_DISABLE()
#include <godot_cpp/variant/builtin_types.hpp>
GODOT_WARNING_RESTORE()

// TODO too big buffer constantly shrinking
template <class T, size_t _TStepSize, size_t _CallsToShrink>
struct GDContainerBigRawBuffer {
private:
	T m_buffer;
	int m_shrink_timer;

public:
	GDContainerBigRawBuffer() {
		m_buffer.resize(_TStepSize);
		m_shrink_timer = 0;
	}

	void prepare_buffer(size_t t_expected_size) {
		if ((size_t)m_buffer.size() < t_expected_size) {
			size_t new_size = (size_t)Math::ceil(t_expected_size / (double)_TStepSize) * _TStepSize;
			DEV_PRINT_STD(NAMEOF(GDContainerBigRawBuffer) ": extending from %d to %d\n", m_buffer.size(), new_size);
			m_buffer.resize(new_size);
			m_shrink_timer = 0;
		} else {
			if (t_expected_size < m_buffer.size() - _TStepSize) {
				m_shrink_timer++;
				if (m_shrink_timer >= _CallsToShrink) {
					size_t new_size = (size_t)m_buffer.size() - _TStepSize;
					DEV_PRINT_STD(NAMEOF(GDContainerBigRawBuffer) ": shrinking from %d to %d\n", m_buffer.size(), new_size);
					m_buffer.resize(new_size);
					m_shrink_timer = 0;
				}
			} else {
				m_shrink_timer = 0;
			}
		}
	}

	inline auto ptrw() {
		return m_buffer.ptrw();
	}

	inline auto slice(int64_t begin, int64_t end = 2147483647) {
		return m_buffer.slice(begin, end);
	}
};
