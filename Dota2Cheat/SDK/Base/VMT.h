#pragma once
#include <cstdint>
#include <vector>

// original by LWSS
// but VMT hooks are VAC-detected so I had to rework it

inline uint32_t CountVMs(void* Interface)
{
	auto** vmt = reinterpret_cast<uintptr_t**>(Interface);

	uint32_t methodCount = 0;

	while (vmt && (*vmt)[methodCount] && IsValidCodePtr((*vmt)[methodCount]))
		methodCount++;

	return methodCount;
}

class VMT
{
public:
	uintptr_t* vmt;

	explicit VMT(auto* obj)
	{
		vmt = *(uintptr_t**)obj;
	}

	// Get virtual method
	template <typename T = void*>
	T GetVM(size_t methodIndex)
	{
		return (T)vmt[methodIndex];
	}
};