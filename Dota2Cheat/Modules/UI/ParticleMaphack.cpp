#include "ParticleMaphack.h"
#include <usermessages.pb.h>

void Modules::M_ParticleMaphack::DrawScreenAppearances() {
	static constexpr ImVec2 iconSize{ 32, 32 };
	for (auto& [hero, data] : Appearances) {
		if (!hero
			|| !hero->GetIdentity()
			|| !hero->GetIdentity()->IsDormant())
			continue;

		auto dl = ImGui::GetBackgroundDrawList();

		int fade = data.fadeCounter / data.fadeTime * 255;
		auto drawPos = WorldToScreen(data.worldPos);
		if (data.icon)
			dl->AddImage(
				data.icon,
				drawPos - iconSize / 2,
				drawPos + iconSize / 2,
				{ 0,0 },
				{ 1,1 },
				ImColor{ 255, 255, 255, fade }
		);
		else
			dl->AddCircleFilled(drawPos, 8, ImColor{ 0XFF, 0XCC, 0, fade });
	}
}

void Modules::M_ParticleMaphack::DrawMapAppearances()
{
	static constexpr ImVec2 iconSize{ 24, 24 };
	auto dl = ImGui::GetBackgroundDrawList();
	for (auto& [hero, data] : Appearances) {
		if (!hero
			|| !hero->GetIdentity()
			|| !hero->GetIdentity()->IsDormant())
			continue;

		if (!IsPointOnScreen(data.mapPos))
			continue;


		auto fade = int(data.fadeCounter / data.fadeTime * 255);

		if (data.icon)
			dl->AddImage(
				data.icon,
				data.mapPos - iconSize / 2,
				data.mapPos + iconSize / 2,
				{ 0,0 },
				{ 1,1 },
				ImColor{ 255, 255, 255, fade }
		);
		else
			dl->AddCircleFilled(data.mapPos, 4, ImColor{ 0XFF, 0XCC, 0, fade });

	}
}

void Modules::M_ParticleMaphack::RegisterAppearance(CDOTABaseNPC* npc, const Vector& pos) {
	ImTextureID icon =
		EntityList.IsHero(npc) ?
		assets.heroIcons.Load(npc->GetSmallIcon()) :
		nullptr;

	auto& mapData = Appearances[npc];
	mapData.worldPos = pos;
	mapData.mapPos = WorldToMap(pos);
	mapData.fadeCounter = mapData.fadeTime = Config::ParticleMapHack::FadeDuration;
	mapData.icon = icon;
}

void Modules::M_ParticleMaphack::Draw() {
	if (!Config::ParticleMapHack::ShowSource)
		return;

	MTM_LOCK;
	DrawScreenAppearances();
	DrawMapAppearances();
}

void Modules::M_ParticleMaphack::OnFrame() {
	MTM_LOCK;

	const float timeDelta = CGameRules::Get()->GetGameTime() - lastTime;
	lastTime = CGameRules::Get()->GetGameTime();
	for (auto it = Appearances.begin(); it != Appearances.end();) {
		auto& data = it->second;
		data.fadeCounter -= timeDelta;
		if (data.fadeCounter <= 0)
			it = Appearances.erase(it);
		else
			++it;
	}
}

void Modules::M_ParticleMaphack::OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID != 145)
		return;

	MTM_LOCK;
	auto pmMsg = reinterpret_cast<CUserMsg_ParticleManager*>(msg);
	auto msgIndex = pmMsg->index();
	switch (pmMsg->type()) {
	case GAME_PARTICLE_MANAGER_EVENT_CREATE: {
		if (pmMsg->create_particle().has_entity_handle_for_modifiers())
			break;
	
		auto npc = EntityList.Get<CHero>(NH2IDX(pmMsg->create_particle().entity_handle_for_modifiers()));
		if (!npc
			|| npc->IsSameTeam(ctx.localHero)
			|| !npc->GetIdentity()
			|| !npc->GetIdentity()->IsDormant()
			|| !EntityList.IsHero(npc)
			)
			return;

		std::string_view particleName;
		{
			const auto szParticleName = CResourceSystem::Get()->GetResourceName(pmMsg->create_particle().particle_name_index());
			if (!szParticleName)
				break;

			particleName = szParticleName;
		}

		if (particleName == "particles/items2_fx/teleport_start.vpcf" ||
			particleName == "particles/items2_fx/teleport_end.vpcf")
			break;

		TransformQueue[pmMsg->index()] = npc;
		break;
	}
	case GAME_PARTICLE_MANAGER_EVENT_RELEASE: {
		TransformQueue.erase(pmMsg->index());
		break;
	}
	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_TRANSFORM: {

		if (!TransformQueue.contains(pmMsg->index()))
			break;
		auto& tf = pmMsg->update_particle_transform();
		if (tf.control_point() != 0)
			break;

		auto& pos = tf.position();
		RegisterAppearance(TransformQueue[pmMsg->index()], pos);
		break;
	}

	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENT: {
		const auto attachType = pmMsg->update_particle_ent().attach_type();
		// AbsOrigin attach types
		if (attachType == 0 || attachType == 1 || attachType == 5) {
			auto pos = pmMsg->update_particle_ent().fallback_position();

			CNPC* npc = nullptr;
			if (TransformQueue.contains(pmMsg->index()))
				npc = TransformQueue[pmMsg->index()];
			else
				npc = EntityList.Get<CNPC>(NH2IDX(pmMsg->update_particle_ent().entity_handle()));

			if (!npc
				|| npc->IsSameTeam(ctx.localHero)
				|| !npc->GetIdentity()
				|| !npc->GetIdentity()->IsDormant()
				|| npc->GetLifeState() != 0)
				return;

			RegisterAppearance(npc, pos);
		}
		break;
	}
	};
}

