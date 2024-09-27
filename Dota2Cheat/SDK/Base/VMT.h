#pragma once
#include <cstdint>
#include <vector>

class VMT
{
public:
	void** vmt;

	explicit VMT(auto* obj)
	{
		vmt = *(void***)obj;
	}

	// Get virtual method
	template <typename T = void*>
	T GetVM(size_t methodIndex)
	{
		return (T)vmt[methodIndex];
	}

	void* operator[](int i) {
		return vmt[i];
	}
};