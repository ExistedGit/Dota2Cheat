#pragma once
#include <cstdint>
#include "Function.h"
#include "Memory.h"

inline Function getvfunc(void* instance, int index)
{
	uintptr_t vtable = *((uintptr_t*)(instance));
	uintptr_t entry = vtable + sizeof(uintptr_t) * index;
	return Function(*(uintptr_t*)entry);
}

class VClass {
public:
	virtual void dummy_fn() = 0; // so that the classes have a vtable

	template<typename T>
	T& Field(int offset) {
		if (!IsValidReadPtr((uintptr_t)this + offset))
			throw std::exception_ptr("VClass::Field access violation");
		return *(T*)((uintptr_t)this + offset);
	}

	template<typename T>
	T Member(int offset/*, T defaultValue = T{}*/) {
		if (!IsValidReadPtr((uintptr_t)this + offset))
			return T{};
		return *(T*)((uintptr_t)this + offset);
	}

	// Gets a pointer to a type via the offset but does not dereference it
	template<typename T>
	T* MemberInline(int offset) {
		return (T*)((uintptr_t)this + offset);
	}

	Function GetVFunc(int index)
	{
		uintptr_t vtable = *((uintptr_t*)(this));
		uintptr_t entry = vtable + sizeof(uintptr_t) * index;
		return Function(*(uintptr_t*)entry);
	}

	template<uint32_t index, typename RET = void*, typename ...T>
	RET CallVFunc(T... t) {
		return GetVFunc(index).Execute<RET>(this, t...);
	}
};
