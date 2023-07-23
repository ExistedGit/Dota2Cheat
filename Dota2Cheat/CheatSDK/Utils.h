#pragma once
#include "../pch.h"
#include "Globals/include.h"
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_internal.h>

inline bool IsKeyPressed(int key) {
	return GetAsyncKeyState(key) & 1;
}

inline ImVec2 ImVecFromVec2D(const Vector2D& vec) {
	return ImVec2{ vec.x,vec.y };
}

inline void PlayUISoundScript(std::string_view name) {
	void* p;
	Interfaces::SoundOpSystem->StartSoundEvent(&p, name.data(), static_cast<uint32_t>(-1), 1);
}

inline void ShowHUDError(std::string_view caption) {
	Panorama::ErrorMessages->ShowErrorMessage(caption.data());
}

inline void ShowHUDError(std::string_view caption, std::string_view sound) {
	Panorama::ErrorMessages->ShowErrorMessage(caption.data());
	PlayUISoundScript(sound);
}

inline ImVec2 WorldToScreen(const Vector& pos) {
	Vector2D xy{};
	if (!IsValidReadPtr(*GameSystems::RenderGameSystemPtr))
		return ImVecFromVec2D(xy);

	(*GameSystems::RenderGameSystemPtr)->GetVectorInScreenSpace(pos, xy);
	return ImVecFromVec2D(xy);
}

inline bool IsPointOnScreen(const ImVec2& pos) {
	if (!Panorama::DotaHud)
		return false;

	const auto screenSize = Panorama::DotaHud->GetScreenSize();
	ImRect screen{ 0, 0, screenSize.x, screenSize.y };
	return screen.Contains(pos);
}

inline bool IsEntityOnScreen(CBaseEntity* ent) {
	return IsPointOnScreen(WorldToScreen(ent->GetPos()));
}

// Converts a 3D position in Dota's world to a point on the minimap
// Credit to Wolf49406
ImVec2 WorldToMap(const Vector& EntityPos);