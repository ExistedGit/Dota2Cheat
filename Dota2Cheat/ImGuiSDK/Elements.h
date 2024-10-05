#pragma once
#include <imgui/imgui.h>
#include <vector>
#include <map>
#include "../CheatSDK/Utils.h"

namespace ImGui {

	bool ToggleButton(const char* label, bool* v, const ImVec2& size_arg);
	bool TextClickable(const char* label);
}

namespace CheatGui {
	inline static bool Vector_ArrayGetter(void* data, int idx, const char** out_text)
	{
		auto items = (const std::vector<std::string>*const)data;
		if (out_text)
			*out_text = items->at(idx).data();
		return true;
	}

	inline bool Combo(const char* label, int* current_item, const std::vector<std::string>& items, int popup_max_height_in_items = -1) {
		const bool value_changed = ImGui::Combo(label, current_item, Vector_ArrayGetter, (void*)&items, items.size(), popup_max_height_in_items);
		return value_changed;
	}

	// Creates an ImGui button that uses Dota's button sounds
	FORCEINLINE bool Button(const char* label, const ImVec2& size_arg = ImVec2(0, 0)) {
		const bool clicked = ImGui::Button(label, size_arg);
		if (clicked)
			PlayUISoundScript("General.ButtonClick");
		
		return clicked;
	}
}