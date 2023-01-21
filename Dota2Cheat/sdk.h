#pragma once
#include <cstdint>
#include <iostream>
#include <Windows.h>

inline void PAUSE() {
	std::cout << "PAUSE" << '\n';
	while (!GetAsyncKeyState(VK_NUMPAD3)) {
	}
}


#define ENT_HANDLE uint32_t
#define ENT_HANDLE_MASK 0x7fff
#define ENTID_FROM_HANDLE(H) (H & ENT_HANDLE_MASK)
#define ENT_HANDLE_VALID(H) (H != 0xFFFFFFFF)

#define u64 unsigned long long
#define cc const char*

extern HANDLE CurProcHandle;
extern int CurProcId;

class Fvector {
public:
	static Fvector Zero;
	float x, y, z;
	Fvector(float x, float y, float z) :x(x), y(y), z(z) {

	}
};

class Function {
public:
	void* ptr;
	inline Function(uintptr_t ptr) : ptr((void*)ptr) {

	}
	template<typename ...T>
	inline void* __fastcall operator()(T... t) {
		return (void*)((u64(__fastcall*)(T...))ptr)(t...);
	}
	template<typename V, typename ...T>
	inline V __fastcall Execute(T... t) {
		return (V)((u64(__fastcall*)(T...))ptr)(t...);
	}

};
inline u64 GetAbsoluteAddress(u64 instruction_ptr, int offset, int size)
{
	return instruction_ptr + *(int*)(instruction_ptr + offset) + size;
}
inline Function getvfunc(void* instance, int index)
{
	uintptr_t vtable = *((uintptr_t*)(instance));
	uintptr_t entry = vtable + sizeof(uintptr_t) * index;
	return Function(*(uintptr_t*)entry);
}
template<typename T, typename Z>
inline bool StringsMatch(T a, Z b) {
	return !strcmp((cc)a, (cc)b);
}
template<typename T, typename Z>
inline void MemCopy(T dst, Z src, size_t size) {
	memcpy((void*)dst, (const void*)src, size);
}
inline void Log(const char* str) {
	std::cout << "[LOG] " << str << '\n';
};
class VClass {
public:
	virtual void dummy_fn() = 0; // so that the classes have a vtable
	template<typename T>
	inline T Member(int offset/*, T defaultValue = T{}*/) {
		//if (!offset)
		//	return defaultValue;
		return *(T*)((uintptr_t)this + offset); 
	}
	inline Function GetVFunc(int index)
	{
		uintptr_t vtable = *((uintptr_t*)(this));
		uintptr_t entry = vtable + sizeof(uintptr_t) * index;
		return Function(*(uintptr_t*)entry);
	}
};