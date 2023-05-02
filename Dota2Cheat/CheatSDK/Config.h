#pragma once
#include "../SDK/Base/Vector.h"
#include <fstream>
#include <json.hpp>

namespace Config {
	class ConfigManager {
	public:
		enum class ConfigVarType {
			BOOL,
			INT,
			FLOAT,
			VECTOR2D,
			VECTOR3D
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
		void SaveEquippedItems(std::ofstream& stream);;
		void LoadEquippedItems(std::ifstream& stream);;
		nlohmann::json* GetJsonEntryFromCfgVar(nlohmann::json& data, const std::string& name) {
			using json = nlohmann::json;
			const static std::string delimiter = "::";
			auto nameCopy = name;
			json* entryPtr = nullptr;
			{

				size_t pos = 0;
				std::string token;
				while ((pos = nameCopy.find(delimiter)) != std::string::npos) {
					token = nameCopy.substr(0, pos);
					if (!entryPtr)
						entryPtr = &data[token];
					else
						entryPtr = &((*entryPtr)[token]);

					nameCopy.erase(0, pos + delimiter.length());
				}
			}
			if (!entryPtr)
				entryPtr = &data[name];
			else
				entryPtr = &((*entryPtr)[nameCopy]);
			return entryPtr;
		}
		void SetupVars();
	};

	namespace AbilityESP {
		inline bool Enabled;
		inline bool ShowAllies;
		inline float UIScale;
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

	inline bool AutoPickUpRunes;
	inline bool AutoPickUpAegis;

	inline bool BlinkRevealer;
	namespace Indicators {
		inline bool Speed;
		inline bool Kill;
		inline float KillScale;
	}

	namespace TPTracker {
		inline bool Enabled;
		inline int FadeDuration;
	}

	inline bool BadCastPrevention;

	inline bool CastRedirection;

	namespace AutoHeal {
		inline bool Enabled;
		inline float WandHPTreshold;
		inline int WandMinCharges;
		inline float FaerieFireHPTreshold;
	}

	inline bool ShowLinearProjTrajectory;

	inline bool ShowEnemyPointSpells;

	namespace ModifierRevealer {
		inline bool TargetedSpells;
		inline bool TrueSight;
		inline bool LinkenSphere;
	}

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
	namespace UIOverhaul {
		inline bool TopBars;
	}
	namespace ManaAbuse {
		inline bool Enabled;
	}
	namespace ParticleMapHack {
		inline bool Enabled;
		inline int FadeDuration;
	}

	inline bool LastHitMarker;

	inline float CameraDistance;

	inline bool RenderAllParticles;
	inline bool PerfectBlink;

	namespace IllusionColoring {
		inline bool Enabled;
		inline Vector Color{ 1,0,0 };
	}

	inline int CircleRadius = 1200;
	inline Vector CircleRGB{ 0,0,0 };
}