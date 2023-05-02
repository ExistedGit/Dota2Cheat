#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include "../../Utils/Drawing.h"

namespace Hacks {
	class ParticleMaphack {
		struct ParticleData {
			Vector pos{ 0, 0,0 };
			ImTextureID icon;
			float fadeCounter{}, fadeTime{};
		};
		struct ParticleEntData : public ParticleData {
			CDOTABaseNPC* ent{};
		};

		std::mutex _m;

		// Only shows the last one
		std::map<CDOTABaseNPC*, ParticleData> MapAppearances;
		std::vector<ParticleEntData> AllAppearances;

		float lastTime = 0;
		void DrawAllAppearances() {
			static constexpr ImVec2 iconSize{ 32, 32 };
			for (auto& data : AllAppearances) {
				auto hero = data.ent;
				if (!hero->GetIdentity()->IsDormant())
					continue;

				auto DrawList = ImGui::GetForegroundDrawList();

				std::string iconName = "icon_";
				iconName += hero->GetIdentity()->GetName();

				auto fade = int(data.fadeCounter / data.fadeTime * 255);
				auto drawPos = WorldToScreen(data.pos);

				DrawList->AddImage(
					data.icon,
					drawPos - iconSize / 2,
					drawPos + iconSize / 2,
					{ 0,0 },
					{ 1,1 },
					ImColor{ 255, 255, 255, fade }
				);

			}
		}
		void DrawMapAppearances()
		{
			static constexpr ImVec2 iconSize{ 24, 24 };
			auto DrawList = ImGui::GetForegroundDrawList();
			for (auto& [hero, data] : MapAppearances) {
				if (!hero->GetIdentity()->IsDormant())
					continue;
				const auto drawPos = WorldToMap(data.pos);
				if (!IsPointOnScreen(drawPos))
					continue;

				auto fade = int(data.fadeCounter / data.fadeTime * 255);

				DrawList->AddImage(
					data.icon,
					drawPos - iconSize / 2,
					drawPos + iconSize / 2,
					{ 0,0 },
					{ 1,1 },
					ImColor{ 255, 255, 255, fade }
				);

			}
		}
	public:
		void Draw() {
			if (!Config::ParticleMapHack::Enabled)
				return;

			std::lock_guard<std::mutex> lk(this->_m);
			DrawAllAppearances();
			DrawMapAppearances();
		};
		void FrameBasedLogic() {
			std::lock_guard<std::mutex> lk(this->_m);

			const float timeDelta = GameSystems::GameRules->GetGameTime() - lastTime;
			lastTime = GameSystems::GameRules->GetGameTime();
			for (auto it = MapAppearances.begin(); it != MapAppearances.end();) {
				auto& data = it->second;
				data.fadeCounter -= timeDelta;
				if (data.fadeCounter <= 0)
					it = MapAppearances.erase(it);
				else
					++it;
			}
			for (auto it = AllAppearances.begin(); it != AllAppearances.end();) {
				auto& data = *it;
				data.fadeCounter -= timeDelta;
				if (data.fadeCounter <= 0)
					it = AllAppearances.erase(it);
				else
					++it;
			}
		};
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
			std::lock_guard<std::mutex> lk(this->_m);
			if (msgHandle->messageID != 145)
				return;

			auto pmMsg = reinterpret_cast<CUserMsg_ParticleManager*>(msg);
			auto msgIndex = pmMsg->index();
			switch (pmMsg->type()) {
			case GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENT: {
				const auto attachType = pmMsg->update_particle_ent().attach_type();
				// AbsOrigin attach types
				if (attachType == 0 || attachType == 1) {
					auto pos = pmMsg->update_particle_ent().fallback_position();
					auto hero = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(NH2IDX(pmMsg->update_particle_ent().entity_handle()));

					if (!hero
						|| hero->IsSameTeam(ctx.assignedHero)
						|| !hero->GetIdentity()->IsDormant())
						return;

					std::string iconName = "icon_";
					iconName += std::string_view(hero->GetIdentity()->GetName()).substr(14);

					auto icon = texManager.GetNamedTexture(iconName);

					auto& mapData = MapAppearances[hero];
					mapData.pos = Vector{ pos.x(), pos.y(), pos.z() };
					mapData.fadeCounter = mapData.fadeTime = Config::ParticleMapHack::FadeDuration;
					mapData.icon = icon;
					auto data = ParticleEntData{
						.ent = hero
					};
					data.pos = mapData.pos;
					data.icon = mapData.icon;
					data.fadeCounter = data.fadeTime = Config::ParticleMapHack::FadeDuration;
					AllAppearances.push_back(data);
				}
				break;
			}
			};
		};
	};
}
namespace Modules {
	inline Hacks::ParticleMaphack ParticleMaphack{};
}