#include "DebugMenu.h"
#include "../../CheatSDK/Globals/VMTDB.h"
#include "../../Modules/Hacks/SkinChanger.h"

void DebugMenu() {
	static int itemDefId = 6996;
	static bool stateHooks;
	static auto& cl_particle_log_creates = CCVar::Get()->CVars["cl_particle_log_creates"].m_pVar->value.boolean;
	static auto net_showreliable = CCVar::Get()->CVars["net_showreliable"].m_pVar;
	static bool net_showreliable_bool = strcmp(net_showreliable->value.str, "0");

	if (ImGui::Begin("Hooks", &stateHooks, ImGuiWindowFlags_AlwaysAutoResize)) {
		for (auto& hook : hooks::installed) {
			bool v = hook.enabled;

			std::string repr = std::format("{:#X} | {}", (uintptr_t)hook.target, hook.name);
			if (ImGui::Checkbox(repr.c_str(), &v)) {
				hook.SetEnabled(v);
			}
		}
	}
	ImGui::End();

	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Hooks")) stateHooks = !stateHooks;

	if (ImGui::Checkbox("net_showreliable", &net_showreliable_bool))
		net_showreliable->SetStrVal(net_showreliable_bool);

	ImGui::Checkbox("cl_particle_log_creates", &cl_particle_log_creates);

	static std::string uiSoundBuf;
	ImGui::InputText("Sound name", &uiSoundBuf);

	if (ImGui::Button("PLAY SOUND"))
		PlayUISoundScript(uiSoundBuf);

	if (ImGui::Button("Reload SIGNATURES")) {
		LogFI("Loading signatures from {}", d2c.cheatFolderPath + "\\signatures.json");
		SignatureDB::LoadSignaturesFromFile(d2c.cheatFolderPath + "\\signatures.json");
		Signatures::FindSignatures();
	}
	if (ImGui::Button("Reload VMT")) {
		d2c.LoadVMI();
	}

	if (ImGui::Button("Log Entities"))
		LogEntities();

	if (ImGui::Button("Log Inventory")) {
		auto selected = ctx.localPlayer->GetSelectedUnits();
		auto ent = CEntSys::Get()->GetEntity<CDOTABaseNPC>(selected[0]);
		LogInvAndAbilities(ent);
	}

	if (ImGui::Button("Log Modifiers")) {
		auto selected = ctx.localPlayer->GetSelectedUnits();
		auto ent = CEntSys::Get()->GetEntity<CDOTABaseNPC>(selected[0]);
		LogModifiers(ent);
	}

	ImGui::InputInt("ItemDef ID", &itemDefId);
	if (ImGui::Button("Create item"))
		Modules::SkinChanger.QueueAddItem(itemDefId);


	ImGui::End();
}
