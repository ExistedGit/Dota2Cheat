#pragma once
#include <cstdint>
#include "Function.h"
#include "Memory.h"

class VClass {
public:
	virtual void dummy_fn() = 0; // so that classes have a vtable

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

	Method GetVFunc(ptrdiff_t index) const
	{
		return Method((void*)this, (*(uintptr_t**)this)[index]);
	}

	template<uint32_t index, typename RET = void*, typename ...T>
	RET CallVFunc(T... t) {
		return GetVFunc(index).Call<RET>(t...);
	}
};
