#pragma once
#include <cstdint>

// Function wrapper which can be called with arbitrary arguments
class Function {
public:
	void* ptr;
	Function(uintptr_t ptr) : ptr((void*)ptr) {

	}
	template<typename ...T>
	void* __fastcall operator()(T... t) {
		return (void*)((uintptr_t(__fastcall*)(T...))ptr)(t...);
	}
	template<typename V, typename ...T>
	V __fastcall Execute(T... t) {
		return ((V(__fastcall*)(T...))ptr)(t...);
	}

};
