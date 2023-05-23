#pragma once
#include <vector>
#include <cstdint>

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
		assert(m_Size > 0 && "CUtlVector::first(): no elements");
		return m_pElements[m_Size - 1];
	}

	[[nodiscard]] std::vector<T> AsStdVector() {
		auto result = std::vector<T>{};
		result.reserve(m_Size);
		for (int i = 0; i < m_Size; i++)
			result.push_back(m_pElements[i]);
		return result;
	}

	int Count() const
	{
		return m_Size;
	}
};