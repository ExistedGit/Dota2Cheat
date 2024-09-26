#pragma once
#include <cstdint>
#include "Function.h"
#include "Memory.h"

class VClass {
	virtual void dummy_fn() = 0; // so that classes have a vtable
public:

	template<typename T>
	T& Field(ptrdiff_t offset) const {
		return *(T*)((uintptr_t)this + offset);
	}

	template<typename T>
	T Member(ptrdiff_t offset) const {
		return *(T*)((uintptr_t)this + offset);
	}

	// Gets a pointer to a type via the offset but does not dereference it
	template<typename T>
	T* MemberInline(ptrdiff_t offset) const {
		return (T*)((uintptr_t)this + offset);
	}

	Method GetVFunc(uint32_t index) const
	{
		return Method((void*)this, (*(uintptr_t**)this)[index]);
	}

	template<uint32_t index, typename R = void*, typename ...Args>
	R CallVFunc(Args... t) {
		return GetVFunc(index).Call<R>(std::forward<Args>(t)...);
	}
};
