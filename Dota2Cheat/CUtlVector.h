#pragma once
#include <vector>
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

	inline std::vector<T> ToStdVector() {
		auto result = std::vector<T>{};
		result.reserve(m_Size);
		for (int i = 0; i  < m_Size; i++) {
			//std::cout << std::hex << &arr[i] << '\n';
			result.push_back(&m_pElements[i]);
		}
		return result;
	}

	inline [[nodiscard]] int Count() const
	{
		return m_Size;
	}
};