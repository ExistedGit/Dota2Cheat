#pragma once
#include "../../SDK/pch.h"
#include <imgui/imgui.h>
#include "../../Config.h"
#include "../../Input.h"
#include "../../Utils/Drawing.h"
#include <regex>

namespace Pages {
	namespace AutoPickHeroGrid {
		std::vector<TextureData> heroIcons;
		bool Initialized = false;

		void InitList() {
			const static std::regex heroRegex("^\\s+\"(npc_dota_hero_\\w+)\".*\n$",
				std::regex_constants::ECMAScript | std::regex_constants::icase);
			auto handle = Interfaces::FileSystem->OpenFile("scripts/npc/npc_heroes.txt", "r");
			if (!handle) {
				std::cout << "Failed hero icons initialization!\n";
				return;
			}

			char buffer[512];
			while (Interfaces::FileSystem->ReadLine(buffer, 512, handle)) {
				std::string line = buffer;
				std::smatch match;
				if (std::regex_match(line, match, heroRegex)){
					std::string heroName = regex_replace(match[0].str(), heroRegex, "$1");
					TextureData data{};
					if (heroName.find("venge") != -1)
					{
						std::cout << heroName << '\n';
					}
					auto path = ctx.cheatFolderPath + "\\assets\\heroicons\\" + heroName + "_png.png";
					texManager.LoadTexture(path.c_str(), data);
					heroIcons.push_back(data);
				}
			}
			Initialized = true;
		}
		void Draw(GLFWwindow* window) {
			if (!Initialized)
				InitList();

			ImGui::Begin("AutoPick");
			for (int i = 0; i < heroIcons.size();++i) {
				auto& icon = heroIcons[i];
				ImGui::ImageButton(icon.glTex, ImVec2(24, 24));
				if (i % 8 != 0)
					ImGui::SameLine();
			}
	
			ImGui::End();
		}
	}
}