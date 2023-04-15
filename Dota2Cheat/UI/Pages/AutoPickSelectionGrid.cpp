#include "AutoPickSelectionGrid.h"

void Pages::AutoPickHeroGrid::InitList() {
	const static std::regex heroRegex("^\\s+\"(npc_dota_hero_\\w+)\".*\n$",
		std::regex_constants::ECMAScript | std::regex_constants::icase);
	auto handle = Interfaces::FileSystem->OpenFile("scripts/npc/npc_heroes.txt", "r");
	if (!handle) {
		std::cout << "Failed hero icons initialization: can't open file\n";
		return;
	}

	char buffer[512];
	while (Interfaces::FileSystem->ReadLine(buffer, 512, handle)) {
		std::string line = buffer;
		std::smatch match;
		if (std::regex_match(line, match, heroRegex)) {
			std::string heroName = regex_replace(match[0].str(), heroRegex, "$1");
			if (std::filesystem::exists(ctx.cheatFolderPath + "\\assets\\heroicons\\" + heroName + "_png.png"))
				heroNames.push_back(heroName);
		}
	}
}

void Pages::AutoPickHeroGrid::InitImages() {
	for (auto& heroName : heroNames) {
		TextureData data{};
		auto path = ctx.cheatFolderPath + "\\assets\\heroicons\\" + heroName + "_png.png";
		texManager.LoadTextureNamed(path.c_str(), heroIcons[heroName], "icon_" + heroName.substr(14));
	}
	Initialized = true;
}

void Pages::AutoPickHeroGrid::Draw(GLFWwindow* window) {
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
			ImGui::ImageButton(it->first.c_str(), icon.glTex, ImVec2(24, 24)))
			std::cout << it->first << ": " << i << '\n';
		if ((i + 1) % 8 != 0)
			ImGui::SameLine();
	}

	ImGui::End();
}
