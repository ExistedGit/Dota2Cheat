#include "AutoPickSelectionGrid.h"
#include "../../CheatSDK/Systems/TextureManager.h"

void Pages::AutoPickHeroGrid::InitList() {
	auto npc_heroes = KeyValues::MakeKV("npc_heroes");
	npc_heroes->LoadFromFile("scripts/npc/npc_heroes.txt");

	for (auto node : npc_heroes) {
		if (!node->GetFirstSubKey()) // not considering non-hero nodes
			continue;

		std::string heroName = node->GetName();
		heroNames.push_back(heroName);
		heroIcons[heroName] = assets.heroIcons.Load(heroName + "_png.png");
	}
	npc_heroes->Destroy();
	CMemAlloc::Get()->Free(npc_heroes);
}

void Pages::AutoPickHeroGrid::Draw() {
	ImGui::Begin("AutoPick/AutoBan menu");
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Button] = ImVec4(0, 0x8b / 255.0f, 0, 0.62f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0x9b / 255.0f, 0, 0.79f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0, 0xaa / 255.0f, 0, 1.00f);

	if (ImGui::ToggleButton("PICK", &pickToggled, ImVec2(65, 125 / 3)))
		banToggled = false;
	ImGui::SameLine();
	colors[ImGuiCol_Button] = ImVec4(0x8b / 255.0f, 0, 0, 0.62f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0xab / 255.0f, 0, 0, 0.79f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0xff / 255.0f, 0, 0, 1.00f);

	if (ImGui::ToggleButton("BAN", &banToggled, ImVec2(65, 125 / 3)))
		pickToggled = false;

	ImGui::StyleColorsClassic();

	for (auto it = heroIcons.begin(); it != heroIcons.end(); ++it) {
		auto& icon = it->second;
		int i = std::distance(heroIcons.begin(), it);
		if (
			ImGui::ImageButton(it->first.c_str(), icon, ImVec2(24, 24)))
			std::cout << it->first << ": " << i << '\n';
		if ((i + 1) % 8 != 0)
			ImGui::SameLine();
	}

	ImGui::End();
}
