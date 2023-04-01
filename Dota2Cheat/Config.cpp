#include "Config.h"

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

void Config::ConfigManager::SetupVars() {
	using namespace Config;
	using enum ConfigManager::ConfigVarType;

	Config::cfg.AddVar(BOOL, &Config::AbilityESP::Enabled, true, "AbilityESP.Enabled");
	Config::cfg.AddVar(FLOAT, &Config::AbilityESP::UIScale, 1.0f, "AbilityESP.UIScale");
	Config::cfg.AddVar(BOOL, &Config::AbilityESP::ShowAllies, true, "AbilityESP.ShowAllies");
	Config::cfg.AddVar(BOOL, &Config::AbilityESP::CropStashItems, false, "AbilityESP.CropStashItems");
	Config::cfg.AddVar(BOOL, &Config::AbilityESP::ShowManabars, true, "AbilityESP.ShowManabars");
	Config::cfg.AddVar(BOOL, &Config::AbilityESP::ShowCooldownDecimals, true, "AbilityESP.ShowCooldownDecimals");

	Config::cfg.AddVar(INT, &Config::CircleRadius, 1200, "Circles.Radius");
	Config::cfg.AddVar(VECTOR, &Config::CircleRGB, { 0, 1, 0 }, "Circles.RGB");

	Config::cfg.AddVar(BOOL, &Config::AutoDodge::Enabled, true, "AutoDodge.Enabled");
	Config::cfg.AddVar(FLOAT, &Config::CameraDistance, 1200.0f, "CameraDistance");

	Config::cfg.AddVar(BOOL, &Config::IllusionColoring::Enabled, false, "IllusionColoring.Enabled");
	Config::cfg.AddVar(VECTOR, &Config::IllusionColoring::Color, { 1,0,0 }, "IllusionColoring.Color");

	Config::cfg.AddVar(INT, &Config::Changer::RiverListIdx, 0, "Changer.RiverPaint");
	Config::cfg.AddVar(INT, &Config::Changer::WeatherListIdx, 0, "Changer.Weather");

	Config::cfg.AddVar(BOOL, &Config::AutoAccept::Enabled, true, "AutoAccept.Enabled");
	Config::cfg.AddVar(INT, &Config::AutoAccept::Delay, 1, "AutoAccept.Delay");

	Config::cfg.AddVar(BOOL, &Config::Changer::UnlockDotaPlus, true, "Changer.UnlockDotaPlus");

	Config::cfg.AddVar(BOOL, &Config::RenderAllParticles, true, "RenderAllParticles");
}
