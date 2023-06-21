#pragma once
#include "../SDK/pch.h"
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "DrawData.h"

inline bool IsKeyPressed(int key) {
	return GetAsyncKeyState(key) & 1;
}

inline ImVec2 ImVecFromVec2D(const Vector2D& vec) {
	return ImVec2{ vec.x,vec.y };
}

inline void PlayUISoundScript(const std::string_view& name) {
	void* p;
	Interfaces::SoundOpSystem->StartSoundEvent(&p, name.data(), static_cast<uint32_t>(-1), 1);
}

inline ImVec2 WorldToScreen(const Vector& pos) {
	Vector2D xy{};
	if (!GameSystems::RenderGameSystem)
		return ImVecFromVec2D(xy);

	GameSystems::RenderGameSystem->GetVectorInScreenSpace(pos, xy);
	return ImVecFromVec2D(xy);
}

inline bool IsPointOnScreen(const ImVec2& pos) {
	if (!GameSystems::DotaHud)
		return false;

	const auto screenSize = GameSystems::DotaHud->GetScreenSize();
	ImRect screen{ 0, 0, screenSize.x, screenSize.y };
	return screen.Contains(pos);
}

inline bool IsEntityOnScreen(CBaseEntity* ent) {
	return IsPointOnScreen(WorldToScreen(ent->GetPos()));
}

// Converts a 3D position in Dota's world to a point on the minimap
// Credit to Wolf49406
ImVec2 WorldToMap(const Vector& EntityPos);