#pragma once
#include <cstdint>
#include "Function.h"
#include "Memory.h"

class VClass {
public:
	virtual void dummy_fn() = 0; // so that classes have a vtable

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

	Function GetVFunc(int index) const
	{
		return Function((*(uintptr_t**)this)[index]);
	}

	template<uint32_t index, typename RET = void*, typename ...T>
	RET CallVFunc(T... t) {
		return GetVFunc(index).Call<RET>(this, t...);
	}
};
