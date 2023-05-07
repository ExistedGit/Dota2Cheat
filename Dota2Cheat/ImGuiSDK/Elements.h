#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

namespace ImGui {

	bool ToggleButton(const char* label, bool* v, const ImVec2& size_arg);
	bool TextClickable(const char* label);
	inline bool InputUInt64(const char* label, uint64_t* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0) {
		// Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
		const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
		return InputScalar(label, ImGuiDataType_U64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
	}
}

namespace CheatGui {
	FORCEINLINE bool Button(const char* label, const ImVec2& size_arg = ImVec2(0, 0)) {
		const bool clicked = ImGui::Button(label, size_arg);
		if (clicked) {
			void* unk;
			Signatures::PlayUISoundScript(&unk, "General.ButtonClick", 1);
		}
		return clicked;
	}
}