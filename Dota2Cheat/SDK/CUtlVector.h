#pragma once

template <class T>
class CUtlVector
{
public:
	CUtlVector() :m_Size(0), m_pElements(nullptr) {}
	
	T& operator[](int i)
	{
		return m_pElements[i];
	}

	[[nodiscard]] int Count() const
	{
		return m_Size;
	}
	int m_Size;
	T* m_pElements;
};