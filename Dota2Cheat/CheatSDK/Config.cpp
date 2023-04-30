#include "Config.h"
#include "../Modules/Hacks/SkinChanger.h"

void Config::ConfigManager::SaveConfig(std::ofstream& stream) {

	using enum ConfigVarType;
	using json = nlohmann::json;

	json data;

	for (auto& [name, var] : vars) {

		auto& entry = *GetJsonEntryFromCfgVar(data, name);
		switch (var.type) {
		case BOOL: entry = *(bool*)var.val; break;
		case FLOAT: entry = *(float*)var.val; break;
		case INT: entry = *(int*)var.val; break;
		case VECTOR2D: {
			Vector2D& vec = *(Vector2D*)var.val;
			entry["x"] = vec.x;
			entry["y"] = vec.y;
			break;
		}
		case VECTOR3D: {
			Vector& vec = *(Vector*)var.val;
			entry["x"] = vec.x;
			entry["y"] = vec.y;
			entry["z"] = vec.z;
			break;
		}

		};
	}
	stream << data.dump(2);
}

void Config::ConfigManager::LoadConfig(std::ifstream& stream) {
	using enum ConfigVarType;
	using json = nlohmann::json;

	json data = json::parse(stream);
	for (auto& [name, var] : vars) {
		auto& entry = *GetJsonEntryFromCfgVar(data, name);
		if (entry.is_null())
			continue;

		switch (var.type) {
		case BOOL: *(bool*)var.val = entry; break;
		case FLOAT: *(float*)var.val = entry; break;
		case INT: *(int*)var.val = entry; break;
		case VECTOR2D: {
			Vector2D& vec = *(Vector2D*)var.val;
			vec.x = entry["x"];
			vec.y = entry["y"];
		}; break;
		case VECTOR3D: {
			Vector& vec = *(Vector*)var.val;
			vec.x = entry["x"];
			vec.y = entry["y"];
			vec.z = entry["z"];
		}; break;

		};
	}

}

void Config::ConfigManager::SaveEquippedItems(std::ofstream& stream) {
	using json = nlohmann::json;
	json data = {};
	for (auto& [itemClass, v] : Modules::SkinChanger.EquippedItems) {
		for (auto& [itemSlot, item] : v) {
			auto& entry = data[std::to_string(item->m_unDefIndex)];
			entry[0] = itemClass;
			entry[1] = itemSlot;
		}
	}
	stream << data.dump(2);
}

void Config::ConfigManager::LoadEquippedItems(std::ifstream& stream) {
	using json = nlohmann::json;
	json data = json::parse(stream);
	if (!data.is_null())
		for (auto& [itemDef, equip] : data.items())
			Modules::SkinChanger.itemsToEquip[stoi(itemDef)] = Hacks::SkinChanger::QueuedEquip{
			.unClass = equip[0],
			.unSlot = equip[1]
		};
}

#define CFG_VAR(type, var, defVal) cfg.AddVar(type, &var, defVal, #var)
void Config::ConfigManager::SetupVars() {
	using namespace Config;
	using enum ConfigManager::ConfigVarType;


	CFG_VAR(BOOL, AbilityESP::Enabled, true);
	CFG_VAR(FLOAT, AbilityESP::UIScale, 1.0f);
	CFG_VAR(BOOL, AbilityESP::ShowAllies, true);
	CFG_VAR(BOOL, AbilityESP::ShowManabars, true);
	CFG_VAR(BOOL, AbilityESP::ShowCooldownDecimals, false);

	CFG_VAR(BOOL, Indicators::Speed, true);
	CFG_VAR(BOOL, Indicators::Kill, true);
	CFG_VAR(FLOAT, Indicators::KillScale, 1.0f);

	CFG_VAR(BOOL, BlinkRevealer, true);

	CFG_VAR(INT, CircleRadius, 1200);
	CFG_VAR(VECTOR3D, CircleRGB, Vector(0, 1, 0));

	CFG_VAR(BOOL, AutoDodge::Enabled, false);

	CFG_VAR(FLOAT, CameraDistance, 1200.0f);

	CFG_VAR(BOOL, ShowEnemyPointSpells, true);
	CFG_VAR(BOOL, ShowLinearProjTrajectory, true);
	CFG_VAR(BOOL, PerfectBlink, false);

	CFG_VAR(BOOL, UIOverhaul::TopBars, true);

	CFG_VAR(BOOL, ModifierRevealer::LinkenSphere, true);
	CFG_VAR(BOOL, ModifierRevealer::TargetedSpells, true);
	CFG_VAR(BOOL, ModifierRevealer::TrueSight, true);

	CFG_VAR(BOOL, TPTracker::Enabled, true);

	CFG_VAR(BOOL, IllusionColoring::Enabled, true);
	CFG_VAR(VECTOR3D, IllusionColoring::Color, Vector(1, 0, 0));

	CFG_VAR(BOOL, ManaAbuse::Enabled, false);

	CFG_VAR(BOOL, AutoAccept::Enabled, true);
	CFG_VAR(INT, AutoAccept::Delay, 1);

	CFG_VAR(BOOL, AutoPickUpRunes, true);
	CFG_VAR(BOOL, AutoPickUpAegis, true);

	CFG_VAR(INT, Changer::RiverListIdx, 0);
	CFG_VAR(INT, Changer::WeatherListIdx, 0);
	CFG_VAR(BOOL, Changer::UnlockDotaPlus, true);
	CFG_VAR(BOOL, Changer::UnlockEmoticons, true);

	CFG_VAR(BOOL, AutoHeal::Enabled, false);
	CFG_VAR(FLOAT, AutoHeal::FaerieFireHPTreshold, 5.f);
	CFG_VAR(FLOAT, AutoHeal::WandHPTreshold, 10.f);
	CFG_VAR(INT, AutoHeal::WandMinCharges, 10);

	CFG_VAR(BOOL, BadCastPrevention, true);
	CFG_VAR(BOOL, LastHitMarker, true);

	CFG_VAR(BOOL, AutoMidas::Enabled, false);
	CFG_VAR(INT, AutoMidas::XPTreshold, 60);

	CFG_VAR(BOOL, RenderAllParticles, true);
}
