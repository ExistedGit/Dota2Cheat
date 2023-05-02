#pragma once
#include <cstdint>

// Wrapper for working with memory addresses
class Address {
public:
	uintptr_t ptr;

	Address(uintptr_t ptr) : ptr(ptr) {}
	Address(void* ptr) : ptr((uintptr_t)ptr) {}

	template<typename T>
	operator T() const
	{
		return (T)ptr;
	}

	Address Offset(int offset) const {
		return Address(ptr + offset);
	}

	template<typename T = Address>
	T GetAbsoluteAddress(int addrOffset, int opcodeSize = -1) const {
		if (opcodeSize == -1)
			opcodeSize = addrOffset + sizeof(uint32_t);
		return T(ptr + *(int*)(ptr + addrOffset) + opcodeSize);
	}

	template<typename T>
	T* As() const {
		return (T*)ptr;
	}

};
