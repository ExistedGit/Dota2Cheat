#pragma once
#include <cstdint>

// Class with no virtual methods
class NormalClass {
public:
	template<typename T>
	T Member(int offset/*, T defaultValue = T{}*/) {
		//if (!offset)
		//	return defaultValue;
		return *(T*)((uintptr_t)this + offset);
	}
	template<typename T>
	T& Field(int offset) {
		if (!IsValidReadPtr((uintptr_t)this + offset))
			throw "VClass::Field access violation";
		return *(T*)((uintptr_t)this + offset);
	}
	// Gets a pointer to a type via the offset but does not dereference it
	template<typename T>
	T* MemberInline(int offset) {
		return (T*)((uintptr_t)this + offset);
	}
};