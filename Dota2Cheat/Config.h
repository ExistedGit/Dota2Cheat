#pragma once
#include "SDK/Base/Vector.h"
#include <fstream>
#include "json.hpp"

namespace Config {


	class ConfigManager {
	public:
		enum class ConfigVarType {
			BOOL,
			INT,
			FLOAT,
			VECTOR
		};
		struct ConfigVar {
			void* val;
			ConfigVarType type;
		};
		std::unordered_map<std::string, ConfigVar> vars;


		template<typename T>
		void AddVar(ConfigVarType type, T* var, T value, const std::string& name) {
			*var = value;
			vars[name] = ConfigVar{ .val = (void*)var, .type = type };
		}

		void SaveConfig(std::ofstream& stream);
		void LoadConfig(std::ifstream& stream);
		void SetupVars();
	};

	namespace AbilityESP {
		inline bool Enabled;
		inline bool ShowAllies;
		inline float UIScale;
		inline bool CropStashItems;
		inline bool ShowManabars;
		inline bool ShowCooldownDecimals;
	}

	namespace AutoAccept {
		inline bool Enabled;
		inline int Delay;
	}
	namespace AutoDodge {
		inline bool Enabled;
	}

	inline ConfigManager cfg{};

	inline bool AutoPickUpRunes = true;
	inline bool AutoPickUpAegis = false;

	inline bool AutoBuyTome = false;

	inline bool CastRedirection = true;

	namespace AutoHeal {
		inline bool Enabled;
		inline float WandHPTreshold;
		inline int WandMinCharges;
		inline float FaerieFireHPTreshold;
	}

	inline bool WarnLinearProjectiles = true;

	inline bool ShowEnemyPointSpells = true;
	inline bool ShowEnemyTargetedSpells = true;
	inline bool ShowLinkenSphere = true;

	namespace Changer {
		inline int WeatherListIdx;
		inline int RiverListIdx;
		inline bool UnlockDotaPlus;
		inline bool UnlockEmoticons;
	}

	namespace AutoMidas {
		inline bool Enabled;
		inline int XPTreshold;
	}

	inline float CameraDistance;

	inline bool RenderAllParticles;

	namespace IllusionColoring {
		inline bool Enabled = true;
		inline Vector Color{ 1,0,0 };
	}

	inline int CircleRadius = 1200;
	inline Vector CircleRGB{ 0,0,0 };
}