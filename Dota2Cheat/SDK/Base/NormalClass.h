#pragma once
#include <cstdint>

// Class with no virtual methods
class NormalClass {
public:
	template<typename T>
	T& Field(int offset) const {
		if (!IsValidReadPtr((uintptr_t)this + offset))
			throw "VClass::Field access violation";
		return *(T*)((uintptr_t)this + offset);
	}

	template<typename T>
	T Member(int offset/*, T defaultValue = T{}*/) const {
		if (!IsValidReadPtr((uintptr_t)this + offset))
			return T{};
		return *(T*)((uintptr_t)this + offset);
	}

	// Gets a pointer to a type via the offset but does not dereference it
	template<typename T>
	T* MemberInline(int offset) const {
		return (T*)((uintptr_t)this + offset);
	}
};