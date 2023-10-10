#pragma once
#include <cstdint>
#include <optional>

// Wrapper for a memory address
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

	Address Offset(ptrdiff_t offset) const {
		return Address(ptr + offset);
	}

	template<typename T = Address>
	T GetAbsoluteAddress(ptrdiff_t addrOffset, std::optional<uint32_t> opcodeSize = std::nullopt) const {
		return T(ptr + *(int*)(ptr + addrOffset) + opcodeSize.value_or(addrOffset + sizeof(uint32_t)));
	}

	template<typename T>
	void Set(const T& val) {
		*(T*)ptr = val;
	}

	template<typename T>
	T* As() const {
		return (T*)ptr;
	}

	template<typename T = Address>
	T Dereference() const {
		return (T)(*(uintptr_t*)ptr);
	}

	Address operator*() {
		return *(uintptr_t*)ptr;
	}

};
