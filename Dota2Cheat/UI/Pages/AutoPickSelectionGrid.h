#pragma once
#include "../../SDK/pch.h"
#include <imgui/imgui.h>
#include "../../Config.h"
#include "../../Input.h"
#include "../../Utils/Drawing.h"
#include <regex>

namespace Pages {
	namespace AutoPickHeroGrid {
		std::vector < std::string> heroNames;
		std::map<std::string, TextureData> heroIcons;
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
				if (std::regex_match(line, match, heroRegex)) {
					std::string heroName = regex_replace(match[0].str(), heroRegex, "$1");
					if (heroName.find("base") != -1)
						continue;

					heroNames.push_back(heroName);
				}
			}
		}
		void InitImages() {
			for (auto& heroName : heroNames) {
				TextureData data{};
				auto path = ctx.cheatFolderPath + "\\assets\\heroicons\\" + heroName + "_png.png";
				texManager.LoadTexture(path.c_str(), data);
				heroIcons[heroName] = data;
			}
			Initialized = true;
		}
		void Draw(GLFWwindow* window) {
			if (!Initialized)
				InitImages();

			ImGui::Begin("AutoPick");
			for (auto it = heroIcons.begin(); it != heroIcons.end(); ++it) {
				auto& icon = it->second;
				if (
					ImGui::ImageButton(icon.glTex, ImVec2(24, 24)))
					std::cout << it->first << '\n';

				int i = std::distance(heroIcons.begin(), it);
				if (i == 0 || i / 8 != 0)
					ImGui::SameLine();
			}

			ImGui::End();
		}
	}
}