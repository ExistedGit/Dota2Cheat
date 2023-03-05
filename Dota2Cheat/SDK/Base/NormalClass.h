#pragma once
#include <cstdint>

#define GETTER(type, name, offset) type name() { return Member<type>(offset); }

// Class with no virtual method
class NormalClass {
public:
	template<typename T>
	T Member(int offset/*, T defaultValue = T{}*/) {
		//if (!offset)
		//	return defaultValue;
		return *(T*)((uintptr_t)this + offset);
	}

	// Gets a pointer to a type via the offset but does not dereference it
	template<typename T>
	T* MemberInline(int offset) {
		return (T*)((uintptr_t)this + offset);
	}
};