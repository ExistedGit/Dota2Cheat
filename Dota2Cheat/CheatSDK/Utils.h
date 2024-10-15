#pragma once
#include "../pch.h"
#include "Globals/include.h"
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_internal.h>

inline bool IsKeyPressed(int key) {
	return GetAsyncKeyState(key) & 1;
}

inline void PlayUISoundScript(std::string_view name) {
	void* p;
	CSoundOpSys::Get()->StartSoundEvent(&p, name.data(), static_cast<uint32_t>(-1), 1);
}

inline void ShowHUDError(std::string_view caption) {
	Panorama::ErrorMessages->ShowErrorMessage(caption.data());
}

inline void ShowHUDError(std::string_view caption, std::string_view sound) {
	ShowHUDError(caption.data());
	PlayUISoundScript(sound);
}

inline ImVec2 WorldToScreen(const Vector& pos) {
	Vector2D xy{};

	if (CRenderGameSystem::Get())
		CRenderGameSystem::Get()->GetVectorInScreenSpace(pos, xy);

	return xy;
}

inline bool IsPointOnScreen(const ImVec2& pos) {
	if (!Panorama::DotaHud)
		return false;

	int screen[2];
	CEngineServiceMgr::Get()->GetEngineSwapChainSize(&screen[0], &screen[1]);
	return pos.x >=0 && pos.y >= 0 && pos.x <= screen[0] && pos.y <= screen[1];
}

inline bool IsEntityOnScreen(CBaseEntity* ent) {
	return IsPointOnScreen(WorldToScreen(ent->GetPos()));
}

// Converts a 3D position in Dota's world to a point on the minimap
ImVec2 WorldToMap(const Vector& EntityPos);