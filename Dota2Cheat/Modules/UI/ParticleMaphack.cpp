#include "ParticleMaphack.h"

void Hacks::ParticleMaphack::DrawScreenAppearances() {
	static constexpr ImVec2 iconSize{ 32, 32 };
	for (auto& [hero, data] : Appearances) {
		if (!IsValidReadPtr(hero)
			|| !IsValidReadPtr(hero->GetIdentity())
			|| !hero->GetIdentity()->IsDormant())
			continue;

		auto DrawList = ImGui::GetForegroundDrawList();

		auto fade = int(data.fadeCounter / data.fadeTime * 255);
		auto drawPos = WorldToScreen(data.worldPos);
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
	for (auto& [hero, data] : Appearances) {
		if (!IsValidReadPtr(hero)
			|| !IsValidReadPtr(hero->GetIdentity())
			|| !hero->GetIdentity()->IsDormant())
			continue;

		if (!IsPointOnScreen(data.mapPos))
			continue;


		auto fade = int(data.fadeCounter / data.fadeTime * 255);

		if (data.icon)
			DrawList->AddImage(
				data.icon,
				data.mapPos - iconSize / 2,
				data.mapPos + iconSize / 2,
				{ 0,0 },
				{ 1,1 },
				ImColor{ 255, 255, 255, fade }
		);
		else
			DrawList->AddCircleFilled(data.mapPos, 4, ImColor{ 0XFF, 0XCC, 0, fade });

	}
}

void Hacks::ParticleMaphack::Draw() {
	if (!Config::ParticleMapHack::Enabled)
		return;

	MTM_LOCK;
	DrawScreenAppearances();
	DrawMapAppearances();
}

void Hacks::ParticleMaphack::FrameBasedLogic() {
	MTM_LOCK;

	const float timeDelta = GameSystems::GameRules->GetGameTime() - lastTime;
	lastTime = GameSystems::GameRules->GetGameTime();
	for (auto it = Appearances.begin(); it != Appearances.end();) {
		auto& data = it->second;
		data.fadeCounter -= timeDelta;
		if (data.fadeCounter <= 0)
			it = Appearances.erase(it);
		else
			++it;
	}
}

void Hacks::ParticleMaphack::ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID != 145)
		return;

	MTM_LOCK;
	auto pmMsg = reinterpret_cast<CUserMsg_ParticleManager*>(msg);
	auto msgIndex = pmMsg->index();
	switch (pmMsg->type()) {
	case GAME_PARTICLE_MANAGER_EVENT_CREATE: {
		if (pmMsg->create_particle().has_entity_handle_for_modifiers())
			break;
		auto npc = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(NH2IDX(pmMsg->create_particle().entity_handle_for_modifiers()));
		if (!IsValidReadPtr(npc)
			|| npc->IsSameTeam(ctx.localHero)
			|| !IsValidReadPtr(npc->GetIdentity())
			|| !npc->GetIdentity()->IsDormant()
			|| !ctx.heroes.contains(npc))
			return;

		const auto particleName = Interfaces::ResourceSystem->GetResourceName(pmMsg->create_particle().particle_name_index());
		if (!particleName)
			break;

		switch (CityHash32(std::string_view(particleName))) {
		case "particles/items2_fx/teleport_start.vpcf"_city32:
		case "particles/items2_fx/teleport_end.vpcf"_city32:
			return;
		};
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
		RegisterAppearance(TransformQueue[pmMsg->index()], Vector(pos.x(), pos.y(), pos.z()));
		break;
	}

	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENT: {
		const auto attachType = pmMsg->update_particle_ent().attach_type();
		// AbsOrigin attach types
		if (attachType == 0 || attachType == 1 || attachType == 5) {
			auto pos = pmMsg->update_particle_ent().fallback_position();

			CDOTABaseNPC* npc = nullptr;
			if (TransformQueue.contains(pmMsg->index()))
				npc = TransformQueue[pmMsg->index()];
			else
				npc = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(NH2IDX(pmMsg->update_particle_ent().entity_handle()));

			if (!IsValidReadPtr(npc)
				|| npc->IsSameTeam(ctx.localHero)
				|| !IsValidReadPtr(npc->GetIdentity())
				|| !npc->GetIdentity()->IsDormant())
				return;

			RegisterAppearance(npc, Vector{ pos.x(), pos.y(), pos.z() });
		}
		break;
	}
	};
}

void Hacks::ParticleMaphack::GetHeroIcon(CDOTABaseNPC* npc, ImTextureID& icon)
{
	if (ctx.heroes.contains((CDOTABaseNPC_Hero*)npc)) {
		std::string iconName = "icon_";
		iconName += std::string_view(npc->GetIdentity()->GetName()).substr(14);
		icon = texManager.GetNamedTexture(iconName);
	}
}
