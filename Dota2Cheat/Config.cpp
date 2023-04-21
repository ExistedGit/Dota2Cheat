#include "Config.h"
#include "Modules/Hacks/SkinChanger.h"

void Config::ConfigManager::SaveConfig(std::ofstream& stream) {
	using enum ConfigVarType;
	using json = nlohmann::json;

	json data;
	for (auto& [name, var] : vars) {
		switch (var.type) {
		case BOOL: data[name] = *(bool*)var.val; break;
		case FLOAT: data[name] = *(float*)var.val; break;
		case INT: data[name] = *(int*)var.val; break;
		case VECTOR: {
			Vector& vec = *(Vector*)var.val;
			data[name]["x"] = vec.x;
			data[name]["y"] = vec.y;
			data[name]["z"] = vec.z;
		}; break;

		};
	}
	stream << data.dump(2);
}

void Config::ConfigManager::LoadConfig(std::ifstream& stream) {
	using enum ConfigVarType;
	using json = nlohmann::json;

	json data = json::parse(stream);
	for (auto& [name, var] : vars) {
		if (data[name].is_null())
			continue;

		switch (var.type) {
		case BOOL: *(bool*)var.val = data[name]; break;
		case FLOAT: *(float*)var.val = data[name]; break;
		case INT: *(int*)var.val = data[name]; break;
		case VECTOR: {
			Vector& vec = *(Vector*)var.val;
			vec.x = data[name]["x"];
			vec.y = data[name]["y"];
			vec.z = data[name]["z"];
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

void Config::ConfigManager::SetupVars() {
	using namespace Config;
	using enum ConfigManager::ConfigVarType;

	cfg.AddVar(BOOL, &AbilityESP::Enabled, true, "AbilityESP.Enabled");
	cfg.AddVar(FLOAT, &AbilityESP::UIScale, 1.0f, "AbilityESP.UIScale");
	cfg.AddVar(BOOL, &AbilityESP::ShowAllies, true, "AbilityESP.ShowAllies");
	cfg.AddVar(BOOL, &AbilityESP::CropStashItems, true, "AbilityESP.CropStashItems");
	cfg.AddVar(BOOL, &AbilityESP::ShowManabars, true, "AbilityESP.ShowManabars");
	cfg.AddVar(BOOL, &AbilityESP::ShowCooldownDecimals, false, "AbilityESP.ShowCooldownDecimals");

	cfg.AddVar(INT, &CircleRadius, 1200, "Circles.Radius");
	cfg.AddVar(VECTOR, &CircleRGB, { 0, 1, 0 }, "Circles.RGB");

	cfg.AddVar(BOOL, &AutoDodge::Enabled, false, "AutoDodge.Enabled");

	cfg.AddVar(FLOAT, &CameraDistance, 1200.0f, "CameraDistance");

	cfg.AddVar(BOOL, &ShowEnemyPointSpells, true, "ShowEnemyPointSpells");
	cfg.AddVar(BOOL, &PerfectBlink, false, "PerfectBlink");

	cfg.AddVar(BOOL, &ModifierRevealer::LinkenSphere, true, "ModifierRevealer.LinkenSphere");
	cfg.AddVar(BOOL, &ModifierRevealer::TargetedSpells, true, "ModifierRevealer.TargetedSpells");
	cfg.AddVar(BOOL, &ModifierRevealer::TrueSight, true, "ModifierRevealer.TrueSight");

	cfg.AddVar(BOOL, &TPTracker::Enabled, true, "TPTracker.Enabled");

	cfg.AddVar(BOOL, &IllusionColoring::Enabled, true, "IllusionColoring.Enabled");
	cfg.AddVar(VECTOR, &IllusionColoring::Color, { 1,0,0 }, "IllusionColoring.Color");

	cfg.AddVar(BOOL, &ManaAbuse::Enabled, false, "ManaAbuse.Enabled");

	cfg.AddVar(BOOL, &AutoAccept::Enabled, true, "AutoAccept.Enabled");
	cfg.AddVar(INT, &AutoAccept::Delay, 1, "AutoAccept.Delay");

	cfg.AddVar(BOOL, &AutoPickUpRunes, true, "AutoPickUp.Runes");
	cfg.AddVar(BOOL, &AutoPickUpAegis, true, "AutoPickUp.Aegis");

	cfg.AddVar(INT, &Changer::RiverListIdx, 0, "Changer.RiverPaint");
	cfg.AddVar(INT, &Changer::WeatherListIdx, 0, "Changer.Weather");
	cfg.AddVar(BOOL, &Changer::UnlockDotaPlus, true, "Changer.UnlockDotaPlus");
	cfg.AddVar(BOOL, &Changer::UnlockEmoticons, true, "Changer.UnlockEmoticons");

	cfg.AddVar(BOOL, &AutoHeal::Enabled, false, "AutoHeal.Enabled");
	cfg.AddVar(FLOAT, &AutoHeal::FaerieFireHPTreshold, 5.f, "AutoHeal.FaerieFireHPTreshold");
	cfg.AddVar(FLOAT, &AutoHeal::WandHPTreshold, 10.f, "AutoHeal.WandHPTreshold");
	cfg.AddVar(INT, &AutoHeal::WandMinCharges, 10, "AutoHeal.WandMinCharges");

	cfg.AddVar(BOOL, &BadCastPrevention, true, "BadCastPrevention");
	cfg.AddVar(BOOL, &LastHitMarker, true, "LastHitMarker");

	cfg.AddVar(BOOL, &AutoMidas::Enabled, false, "AutoMidas.Enabled");
	cfg.AddVar(INT, &AutoMidas::XPTreshold, 60, "AutoMidas.XPTreshold");

	cfg.AddVar(BOOL, &RenderAllParticles, true, "RenderAllParticles");
}
