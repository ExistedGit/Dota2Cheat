#include "Drawing.h"

// Credit to Wolf49406
ImVec2 WorldToMap( const Vector& EntityPos ) {
	if ( !GameSystems::MinimapRenderer )
		return { 0,0 };
	const auto ScreenSize = glfwGetVideoMode( glfwGetPrimaryMonitor( ) );

	// The border around the actual map panel is around 12px
	auto ActualMinimapSize = static_cast<float>( GameSystems::MinimapRenderer->GetMinimapSize( ).x - 24 );
	auto MinimapPosMin = Vector2D( 12, static_cast<float>( ScreenSize->height - ActualMinimapSize - 12 ) );

	if ( Signatures::IsHUDFlipped( ) ) {
		float offset = ScreenSize->width - ActualMinimapSize;
		MinimapPosMin.x = MinimapPosMin.x + offset;
	}

	Vector2D Scaler = GameSystems::MinimapRenderer->MinimapBounds / ActualMinimapSize * 2;
	auto PosOnMinimap = MinimapPosMin + ( GameSystems::MinimapRenderer->MinimapBounds / Scaler ) - ( Vector2D{ EntityPos.x, EntityPos.y } / Scaler );

	return ImVecFromVec2D( PosOnMinimap );
}


void DrawRect(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color, float thickness) {
	auto DrawList = ImGui::GetForegroundDrawList();
	DrawList->AddRect(
		topLeft, ImVec2(size.x + topLeft.x, size.y + topLeft.y),
		ImGui::GetColorU32(color),
		0,
		0,
		thickness
	);
}

void DrawRectFilled(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color) {
	auto DrawList = ImGui::GetForegroundDrawList();
	DrawList->AddRectFilled(
		topLeft, ImVec2(size.x + topLeft.x, size.y + topLeft.y),
		ImGui::GetColorU32(color)
	);
}

//credits to SMBB from UnknownCheats
//https://www.unknowncheats.me/forum/direct3d/244074-imgui-d3d11-text-drawing.html
float DrawTextForeground(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, const ImVec4& color, bool center, bool outline)
{
	auto black = ImVec4(0, 0, 0, color.w);

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;
	auto DrawList = ImGui::GetForegroundDrawList();

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		if (center)
		{
			if (outline) {
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(black), line.c_str());
			}
			DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			if (outline) {

				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(black), line.c_str());
			}
			DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

bool TextureManager::LoadTexture(const char* filename, ImTextureID& tex)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	tex = (ImTextureID)image_texture;
	
	return true;
}
