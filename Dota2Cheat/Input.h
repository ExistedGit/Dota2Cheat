#pragma once
#include <Windows.h>
// based on GetAsyncKeyState
inline bool IsKeyPressed(int key) {
	return GetAsyncKeyState(key) & 1;
}