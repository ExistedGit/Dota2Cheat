#include "ParticleMaphack.h"

void Hacks::ParticleMaphack::DrawAllAppearances() {
	static constexpr ImVec2 iconSize{ 32, 32 };
	for (auto& data : AllAppearances) {
		auto hero = data.ent;
		if (!hero->GetIdentity()->IsDormant())
			continue;

		auto DrawList = ImGui::GetForegroundDrawList();

		auto fade = int(data.fadeCounter / data.fadeTime * 255);
		auto drawPos = WorldToScreen(data.pos);
		if (data.icon)
			DrawList->AddImage(
				data.icon,
				drawPos - iconSize / 2,
				drawPos + iconSize / 2,
				{ 0,0 },
				{ 1,1 },
				ImColor{ 255, 255, 255, fade }
		);
		else
			DrawList->AddCircleFilled(drawPos, 8, ImColor{ 0XFF, 0XCC, 0, fade });
	}
}

void Hacks::ParticleMaphack::DrawMapAppearances()
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

		if (data.icon)
			DrawList->AddImage(
				data.icon,
				drawPos - iconSize / 2,
				drawPos + iconSize / 2,
				{ 0,0 },
				{ 1,1 },
				ImColor{ 255, 255, 255, fade }
		);
		else
			DrawList->AddCircleFilled(drawPos, 4, ImColor{ 0XFF, 0XCC, 0, fade});

	}
}

void Hacks::ParticleMaphack::Draw() {
	if (!Config::ParticleMapHack::Enabled)
		return;

	std::lock_guard<std::mutex> lk(this->_m);
	DrawAllAppearances();
	DrawMapAppearances();
}

void Hacks::ParticleMaphack::FrameBasedLogic() {
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
}

void Hacks::ParticleMaphack::ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
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
			auto npc = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(NH2IDX(pmMsg->update_particle_ent().entity_handle()));

			if (!npc
				|| npc->IsSameTeam(ctx.assignedHero)
				|| !npc->GetIdentity()->IsDormant())
				return;



			ImTextureID icon = 0;

			if (ctx.heroes.contains((CDOTABaseNPC_Hero*)npc)) {
				std::string iconName = "icon_";
				iconName += std::string_view(npc->GetIdentity()->GetName()).substr(14);
				icon = texManager.GetNamedTexture(iconName);
			}

			auto& mapData = MapAppearances[npc];
			mapData.pos = Vector{ pos.x(), pos.y(), pos.z() };
			mapData.fadeCounter = mapData.fadeTime = Config::ParticleMapHack::FadeDuration;
			mapData.icon = icon;
			auto data = ParticleEntData{
				.ent = npc
			};
			data.pos = mapData.pos;
			data.icon = mapData.icon;
			data.fadeCounter = data.fadeTime = Config::ParticleMapHack::FadeDuration;
			AllAppearances.push_back(data);
		}
		break;
	}
	};
}
