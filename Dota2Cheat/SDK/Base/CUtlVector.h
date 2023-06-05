#pragma once
#include <vector>
#include <cstdint>
#include "MemAlloc.h"

// Source engine's own analog to std::vector
template <class T>
class CUtlVector
{
public:
	uint32_t m_Size;
	T* m_pElements;
	uint32_t m_Capacity;

	T& operator[](int i)
	{
		return m_pElements[i];
	}

	T& at(int i) {
		return m_pElements[i];
	}

	T* begin() {
		return m_pElements;
	}

	T* end() {
		return m_pElements + m_Size;
	}

	T& first() {
		assert(m_Size > 0 && "CUtlVector::first(): no elements");
		return m_pElements[0];
	}
	T& last() {
		assert(m_Size > 0 && "CUtlVector::last(): no elements");
		return m_pElements[m_Size - 1];
	}

	[[nodiscard]] std::vector<T> AsStdVector() {
		auto result = std::vector<T>{};
		result.reserve(m_Size);
		for (int i = 0; i < m_Size; i++)
			result.push_back(m_pElements[i]);
		return result;
	}

	void pop_back() {
		m_Size--;
	}

	void remove_by_value(const T& elem) {
		int idx = 0;
		for (; idx < m_Size; idx++)
			if (m_pElements[idx] == elem)
				break;

		if (idx == m_Size)
			return;

		for (int i = idx + 1; i < m_Size; i++)
			m_pElements[i - 1] = m_pElements[i];

		m_Size--;
	}

	void push_back(const T& elem) {
		m_Size++;
		adjust_capacity();
		m_pElements[m_Size - 1] = elem;
	}

	void adjust_capacity() {
		if (m_Size > m_Capacity) {
			if (m_Capacity == 0)
				m_Capacity = 1;
			else
				m_Capacity *= 2;

			m_pElements = m_pElements
				? CMemAlloc::Instance()->ReAlloc(m_pElements, m_Capacity * sizeof(T))
				: CMemAlloc::Instance()->Alloc<T>(m_Size * sizeof(T));
		}
	}

	int Count() const
	{
		return m_Size;
	}
};