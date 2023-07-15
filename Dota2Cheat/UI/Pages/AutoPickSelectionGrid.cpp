#include "AutoPickSelectionGrid.h"
#include "../../CheatSDK/Systems/TextureManager.h"

struct KeyValues {
	static KeyValues* MakeKV(const char* name) {
		auto kv = CMemAlloc::Instance()->Alloc<KeyValues>(0x14);
		auto ctor = Memory::GetExport("tier0.dll", "??0KeyValues@@QEAA@PEBD@Z");
		ctor(kv, name);
		return kv;
	}
	bool LoadFromFile(const char* path) {
		auto func = Memory::GetExport("tier0.dll", "?LoadFromFile@KeyValues@@QEAA_NPEAVIFileSystem@@PEBD1P6A_N1PEAX@Z21@Z");
		return func.Call<bool>(this, Interfaces::FileSystem, path, "GAME", nullptr, nullptr, nullptr);
	}
	KeyValues* GetFirstSubKey() {
		auto func = Memory::GetExport("tier0.dll", "?GetFirstSubKey@KeyValues@@QEBAPEAV1@XZ");
		return func.Call<KeyValues*>(this);
	}
	KeyValues* GetNextKey() {
		auto func = Memory::GetExport("tier0.dll", "?GetNextKey@KeyValues@@QEBAPEAV1@XZ");
		return func.Call<KeyValues*>(this);
	}
	const char* GetName() {
		auto func = Memory::GetExport("tier0.dll", "?GetName@KeyValues@@QEBAPEBDXZ");
		return func.Call<const char*>(this);
	}
	void Destroy() {
		auto func = Memory::GetExport("tier0.dll", "??1KeyValues@@QEAA@XZ");
		func(this);
	}
};

void Pages::AutoPickHeroGrid::InitList() {
	auto npc_heroes = KeyValues::MakeKV("Shit");
	npc_heroes->LoadFromFile("scripts/npc/npc_heroes.txt");
	for (auto node = npc_heroes->GetFirstSubKey(); node; node = node->GetNextKey()) {
		if (!node->GetFirstSubKey()) // not considering non-hero nodes
			continue;

		std::string heroName = node->GetName();
		auto path = d2c.cheatFolderPath + "\\assets\\heroicons\\" + heroName + "_png.png";
		if (std::filesystem::exists(path)) {
			heroNames.push_back(heroName);
			texManager.LoadTextureNamed(path, &heroIcons[heroName], "icon_" + heroName.substr(14));
		}
	}
	npc_heroes->Destroy();
	CMemAlloc::Instance()->Free(npc_heroes);
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
