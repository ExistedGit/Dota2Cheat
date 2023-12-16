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
		case UINT_64: entry = *(uint64_t*)var.val; break;
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
		case UINT_64: *(uint64_t*)var.val = entry; break;
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
			Modules::SkinChanger.itemsToEquip[stoi(itemDef)] = Modules::SkinChanger::QueuedEquip{
			.unClass = equip[0],
			.unSlot = equip[1]
		};
}

// Minimized the definition syntax as much as I can
// Pure elegancy
#define CFG_VAR(var, defVal) cfg.AddVar(&var, defVal, #var)

void Config::ConfigManager::SetupVars() {
	using namespace Config;

	CFG_VAR(AbilityESP::Enabled, true);
	CFG_VAR(AbilityESP::UIScale, 1.0f);
	CFG_VAR(AbilityESP::LevelCounterType, 0);
	CFG_VAR(AbilityESP::ItemPanelType, 0);
	CFG_VAR(AbilityESP::Rounding, 100);
	CFG_VAR(AbilityESP::ShowAllies, true);
	CFG_VAR(AbilityESP::ShowCooldownDecimals, false);

	CFG_VAR(Bars::ManaBars, true);
	CFG_VAR(Bars::HPNumbers, true);

	CFG_VAR(Indicators::Speed, true);
	CFG_VAR(Indicators::Kill, true);
	CFG_VAR(Indicators::KillScale, 1.0f);

	CFG_VAR(BlinkRevealer, true);

	CFG_VAR(CircleRadius, 1200);
	CFG_VAR(CircleRGB, Vector(0, 1, 0));

	CFG_VAR(AutoDodge::Enabled, false);

	CFG_VAR(CameraDistance, 1200.0f);

	CFG_VAR(ShowEnemyPointSpells, true);
	CFG_VAR(ShowLinearProjTrajectory, true);
	CFG_VAR(PerfectBlink, false);

	CFG_VAR(UIOverhaul::TopBars, true);
	CFG_VAR(UIOverhaul::NetworthPanel, true);

	CFG_VAR(ModifierRevealer::LinkenSphere, true);
	CFG_VAR(ModifierRevealer::TargetedSpells, true);
	CFG_VAR(ModifierRevealer::TrueSight, true);

	CFG_VAR(TPTracker::Enabled, true);
	CFG_VAR(TPTracker::FadeDuration, 5);

	CFG_VAR(ParticleMapHack::ShowSource, true);
	CFG_VAR(ParticleMapHack::ShowParticles, true);
	CFG_VAR(ParticleMapHack::FadeDuration, 5);

	CFG_VAR(IllusionColoring::Enabled, true);
	CFG_VAR(IllusionColoring::Color, Vector(1,1,1));

	CFG_VAR(ManaAbuse::Enabled, false);
	CFG_VAR(ManaAbuse::Mode, 0);
	CFG_VAR(ManaAbuse::SafetyRadius, 1000);

	CFG_VAR(AutoAccept::Enabled, true);
	CFG_VAR(AutoAccept::Delay, 1);

	CFG_VAR(AutoPickUpRunes, true);
	CFG_VAR(AutoPickUpAegis, true);

	CFG_VAR(Changer::RiverListIdx, 0);
	CFG_VAR(Changer::WeatherListIdx, 0);
	CFG_VAR(Changer::UnlockDotaPlus, true);
	CFG_VAR(Changer::MaxHeroTiers, true);
	CFG_VAR(Changer::TreeModelIdx, 0);
	CFG_VAR(Changer::UnlockEmoticons, true);

	CFG_VAR(AutoHeal::Enabled, false);
	CFG_VAR(AutoHeal::FaerieFireHPTreshold, 5.f);
	CFG_VAR(AutoHeal::WandHPTreshold, 10.f);
	CFG_VAR(AutoHeal::WandMinCharges, 10);

	CFG_VAR(BadCastPrevention, true);
	CFG_VAR(CastRedirection, true);
	CFG_VAR(LastHitMarker, true);

	CFG_VAR(AutoMidas::Enabled, false);
	CFG_VAR(AutoMidas::XPTreshold, 60);
}
