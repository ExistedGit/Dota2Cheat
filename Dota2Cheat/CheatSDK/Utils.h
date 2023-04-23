#pragma once
#include "../SDK/pch.h"
#include <imgui/imgui.h>

inline bool IsKeyPressed(int key) {
	return GetAsyncKeyState(key) & 1;
}

inline ImVec2 WorldToScreen(const Vector& pos) {
	int x, y;
	Signatures::WorldToScreen(&pos, &x, &y, nullptr);
	return ImVec2{ (float)x, (float)y };
}