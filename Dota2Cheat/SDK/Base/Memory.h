#pragma once
#include <cstdint>

template<typename T = uintptr_t>
T GetAbsoluteAddress(uintptr_t instruction_ptr, int offset, int size)
{
	return T(instruction_ptr + *(int*)(instruction_ptr + offset) + size);
}

template<typename T, typename Z>
void MemCopy(T dst, Z src, size_t size) {
	memcpy((void*)dst, (const void*)src, size);
}