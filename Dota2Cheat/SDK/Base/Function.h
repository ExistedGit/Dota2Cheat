#pragma once
#include <cstdint>

// Function wrapper which can be called with arbitrary arguments
// Made by Liberalist
struct Function {
	void* ptr;

	Function() : ptr(nullptr) {}
	Function(uintptr_t ptr) : ptr((void*)ptr) {}
	Function(void* ptr) : ptr(ptr) {}

	template<typename T>
	operator T* ()
	{
		return (T*)ptr;
	}

	template<typename ...T>
	void* __fastcall operator()(T... t) {
		return ((void* (__fastcall*)(T...))ptr)(t...);
	}
	// Used to specify the return type(e. g. in case of a floating-point value)
	template<typename V, typename ...T>
	V __fastcall Call(T... t) {
		return ((V(__fastcall*)(T...))ptr)(t...);
	}

};

// A function with a bound "this" parameter.
struct Method {
public:
	void* ptr;
	void* thisptr;

	Method() : ptr(nullptr) {}
	Method(void* thisptr, uintptr_t ptr) : ptr((void*)ptr), thisptr(thisptr) {}
	Method(void* thisptr, void* ptr) : ptr(ptr), thisptr(thisptr) {}

	template<typename T>
	operator T* ()
	{
		return (T*)ptr;
	}

	template<typename ...T>
	void* __fastcall operator()(T... t) {
		return ((void* (__fastcall*)(void*, T...))ptr)(thisptr, t...);
	}

	// Used to specify the return type(e. g. in case of a floating-point value)
	template<typename V, typename ...T>
	V __fastcall Call(T... t) {
		return ((V(__fastcall*)(void*, T...))ptr)(thisptr, t...);
	}
};