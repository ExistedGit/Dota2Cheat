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
			UINT_64,
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

// Used to infer the type of a ConfigVar to avoid manually specifying it
// Essentially links a type to an enum value
#define CFGVAR_INFER(cType, eType) if constexpr (std::is_same_v<T, cType>) \
										AddVar(eType, var, value, name);

		template<typename T>
		void AddVar(T* var, T value, const std::string& name) {
			CFGVAR_INFER(bool, ConfigVarType::BOOL);
			CFGVAR_INFER(int, ConfigVarType::INT);
			CFGVAR_INFER(float, ConfigVarType::FLOAT);
			CFGVAR_INFER(Vector, ConfigVarType::VECTOR3D);
			CFGVAR_INFER(Vector2D, ConfigVarType::VECTOR2D);
			CFGVAR_INFER(uint64_t, ConfigVarType::UINT_64);
		}

#undef CFGVAR_INFER

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
		inline int Rounding;
		inline int LevelCounterType;
		inline int ItemPanelType;
		inline bool ShowCooldownDecimals;
	}

	namespace Bars {
		inline bool ManaBars;
		inline bool HPNumbers;
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
		inline int TreeModelIdx;
		inline int RiverListIdx;

		inline bool UnlockDotaPlus;
		inline bool MaxHeroTiers;

		inline bool UnlockEmoticons;
	}

	namespace AutoMidas {
		inline bool Enabled;
		inline int XPTreshold;
	}
	namespace UIOverhaul {
		inline bool TopBars;
		inline bool NetworthPanel;
	}
	namespace ManaAbuse {
		inline bool Enabled;
		// For enum definition see Modules/Hacks/ManaHPAbuse
		inline int Mode;
		inline int SafetyRadius;
	}
	namespace ParticleMapHack {
		inline bool ShowSource;
		inline int FadeDuration;
		inline bool ShowParticles;
	}

	inline bool LastHitMarker;

	inline float CameraDistance;

	inline bool PerfectBlink;

	namespace IllusionColoring {
		inline bool Enabled;
		inline Vector Color;
	}

	inline int CircleRadius;
	inline Vector CircleRGB;
}