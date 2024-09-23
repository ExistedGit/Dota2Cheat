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

	template<typename ...Args>
	void* __fastcall operator()(Args&&... t) {
		return ((void* (__fastcall*)(Args...))ptr)(std::forward<Args>(t)...);
	}

	// Used to specify the return type(e. g. in case of a floating-point value)
	template<typename R, typename ...Args>
	R __fastcall Call(Args&&... t) {
		return ((R(__fastcall*)(Args...))ptr)(std::forward<Args>(t)...);
	}
};

// A function with a bound "this" parameter.
struct Method {
public:
	void* ptr;
	void* thisptr;

	Method(void* thisptr, uintptr_t ptr) : ptr((void*)ptr), thisptr(thisptr) {}
	Method(void* thisptr, void* ptr) : ptr(ptr), thisptr(thisptr) {}

	template<typename T>
	operator T* ()
	{
		return (T*)ptr;
	}

	template<typename ...Args>
	void* __fastcall operator()(Args&&... t) {
		return ((void* (__fastcall*)(void*, Args...))ptr)(thisptr, std::forward<Args>(t)...);
	}

	// Used to specify the return type(e. g. in case of a floating-point value)
	template<typename R, typename ...Args>
	R __fastcall Call(Args&&... t) {
		return ((R(__fastcall*)(void*, Args...))ptr)(thisptr, std::forward<Args>(t)...);
	}
};