#pragma once
#include <cstdint>

template <class T>
class CUtlVector
{
public:
	uintptr_t m_Size;
	T* m_pElements;
	uintptr_t m_Capacity;

	inline T& operator[](int i)
	{
		return m_pElements[i];
	}
	inline T& at(int i) {
		return m_pElements[i];
	}
	inline T& first() {
		return m_pElements[0];
	}
	inline T& last() {
		return m_pElements[m_Size - 1];
	}


	inline [[nodiscard]] int Count() const
	{
		return m_Size;
	}
};