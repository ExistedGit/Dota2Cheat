#pragma once
#include <cstdint>

// Class with no virtual methods
class NormalClass {
public:
	template<typename T>
	T& Field(ptrdiff_t offset) const {
		if (!IsValidReadPtr((uintptr_t)this + offset))
			throw "VClass::Field access violation";
		return *(T*)((uintptr_t)this + offset);
	}

	template<typename T>
	T Member(ptrdiff_t offset/*, T defaultValue = T{}*/) const {
		if (!IsValidReadPtr((uintptr_t)this + offset))
			return T{};
		return *(T*)((uintptr_t)this + offset);
	}

	// Gets a pointer to a type via the offset but does not dereference it
	template<typename T>
	T* MemberInline(ptrdiff_t offset) const {
		return (T*)((uintptr_t)this + offset);
	}
};