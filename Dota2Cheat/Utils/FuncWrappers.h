#pragma once
#include "../SDK/pch.h"
#include <imgui/imgui.h>

inline ImVec2 WorldToScreen(Vector pos) {
	int x, y;
	Signatures::WorldToScreen(&pos, &x, &y, nullptr);
	return ImVec2{ (float)x, (float)y };
}