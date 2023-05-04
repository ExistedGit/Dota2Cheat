#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

namespace ImGui {

	bool ToggleButton(const char* label, bool* v, const ImVec2& size_arg);

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