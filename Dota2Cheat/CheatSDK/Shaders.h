#pragma once
#include <imgui/imgui.h>

using ImShaderCallback = void(*)(const ImDrawList*, const ImDrawCmd*);
// RAII shader application
struct ShaderApplicator {
	ImDrawList* drawList;

	~ShaderApplicator() {
		if (drawList)
			drawList->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
	}

	ShaderApplicator(ImDrawList* drawList, ImShaderCallback cb, void* callbackData) :drawList(drawList) {
		drawList->AddCallback(cb, callbackData);
	}

	ShaderApplicator(ImDrawList* drawList) :drawList(drawList) {}
};

void GreyscaleShaderCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd);
void NoManaShaderCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd);