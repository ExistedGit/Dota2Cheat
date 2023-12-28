#pragma once
#include "VClass.h"

// Valve's memory allocation manager
struct CMemAlloc : public VClass {
	static CMemAlloc* Instance() {
		static CMemAlloc* g_pMemAlloc = *Memory::GetExport<CMemAlloc**>("tier0.dll", "g_pMemAlloc");
		return g_pMemAlloc;
	}

	template<typename T>
	T* AllocArray(size_t arrSize) {
		return CallVFunc<1, T*>(sizeof(T) * arrSize);
	}

	template<typename T>
	T* AllocInit() {
		T obj{};
		auto mem = CallVFunc<1, T*>(sizeof(T));
		memcpy(mem, &obj, sizeof(T));
		return mem;
	}

	template<typename T>
	T* AllocCopy(const T* obj) {
		auto mem = CallVFunc<1, T*>(sizeof(T));
		memcpy(mem, &obj, sizeof(T));
		return mem;
	}

	template<typename T>
	T* Alloc() {
		return CallVFunc<1, T*>(sizeof(T));
	}

	template<typename T = void>
	T* Alloc(size_t size) {
		return CallVFunc<1, T*>(size);
	}

	template<typename T = void>
	T* ReAlloc(const T* p, size_t size) {
		return CallVFunc<2, T*>(p, size);
	}

	void Free(const void* p) {
		return CallVFunc<3, void>(p);
	}

	size_t GetSize(const void* p) {
		return CallVFunc<21, size_t>(p);
	}
};