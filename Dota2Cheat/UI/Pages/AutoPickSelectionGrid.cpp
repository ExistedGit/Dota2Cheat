#include "AutoPickSelectionGrid.h"

void Pages::AutoPickHeroGrid::InitList() {
	using json = nlohmann::json;
	std::ifstream fin(ctx.cheatFolderPath + "\\assets\\json\\npc_heroes.json");
	if (!fin.is_open()) {
		std::cout << "Failed hero icons initialization: can't open file\n";
		return;
	}
	auto data = json::parse(fin);
	for (auto& [heroName, _] : data["DOTAHeroes"].items()) {
		auto path = ctx.cheatFolderPath + "\\assets\\heroicons\\" + heroName + "_png.png";
		if (std::filesystem::exists(path)) {
			heroNames.push_back(heroName);
			texManager.QueueForLoading(path, "icon_" + heroName.substr(14));
		}
	}
}

void Pages::AutoPickHeroGrid::InitImages() {
	for (auto& heroName : heroNames)
		heroIcons[heroName] = texManager.GetNamedTexture("icon_" + heroName.substr(14));
	Initialized = true;
}

void Pages::AutoPickHeroGrid::Draw() {
	if (!Initialized)
		InitImages();

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
