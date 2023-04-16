#pragma once
#include "../../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../Config.h"

namespace Hacks {
	class TPTracker {
		struct ParticleData {
			Vector pos;
			std::string_view name;
			uint32_t msgIdx;
		};


		std::map<CBaseEntity*, ParticleData> tpStarts;
		std::map<CBaseEntity*, ParticleData> tpEnds;

		std::map<CBaseEntity*, ImTextureID> heroIcons;
	public:
		void CacheHeroIcons() {
			for (auto& hero : ctx.heroes) {
				if (heroIcons.count(hero))
					continue;
				std::string prefixLessName = std::string(hero->GetUnitName()).substr(14),
					iconName = "icon_" + prefixLessName;
				heroIcons[hero] = texManager.GetNamedTexture(iconName);
			}
		}
		void DrawMapTeleports();
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg);;
	};
}
namespace Modules {
	inline Hacks::TPTracker TPTracker{};
}