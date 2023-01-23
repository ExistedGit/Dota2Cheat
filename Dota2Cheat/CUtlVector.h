#pragma once
#include <cstdint>

template <class T>
class CUtlVector
{
public:
	uintptr_t m_Size;
	T* m_pElements;
	uintptr_t m_Capacity;

	T& operator[](int i)
	{
		return m_pElements[i];
	}

	[[nodiscard]] int Count() const
	{
		return m_Size;
	}
};