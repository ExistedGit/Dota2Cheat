#include "TPTracker.h"

void Hacks::TPTracker::DrawMapTeleports() {
	if (!Config::TPTracker::Enabled)
		return;

	constexpr static ImVec2 iconSize{ 32,32 };
	auto  DrawList = ImGui::GetForegroundDrawList();
	for (auto& [ent, startData] : tpStarts) {
		if (ent->IsSameTeam(ctx.assignedHero)
			|| !tpEnds.count(ent))
			continue;
		const auto& endData = tpEnds[ent];
		ImVec2 start = WorldToMap(startData.pos), end = WorldToMap(endData.pos);

		DrawList->AddLine(start, end, ImColor{ 255, 0, 0 }, 3);

		if (!heroIcons.count(ent))
			continue;

		auto texture = heroIcons[ent];
		if (!texture)
			continue;

		if (ent->GetIdentity()->IsDormant()) {
			ImVec2 startXY1 = start - iconSize / 2, startXY2 = startXY1 + iconSize;
			DrawList->AddImage(texture,
				startXY1,
				startXY2
			);
		}
		ImVec2 endXY1 = end - iconSize / 2, endXY2 = endXY1 + iconSize;
		DrawList->AddImage(texture,
			endXY1,
			endXY2,
			{ 0,0 },
			{ 1,1 },
			ImColor{ 255,255,255, 128 }
		);

	}
}

void Hacks::TPTracker::ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID != 145)
		return;

	auto pmMsg = reinterpret_cast<CUserMsg_ParticleManager*>(msg);
	auto msgIndex = pmMsg->index();
	switch (pmMsg->type()) {
	case GAME_PARTICLE_MANAGER_EVENT_CREATE: {

		auto particle = pmMsg->create_particle();
		if (!particle.has_particle_name_index())
			break;
		std::string_view particleName = "";
		{
			const char* name = Interfaces::ResourceSystem->GetResourceName(particle.particle_name_index());
			if (name)
				particleName = name;
		}
		auto ent = Interfaces::EntitySystem->GetEntity(NH2IDX(particle.entity_handle_for_modifiers()));
		if (!ent)
			break;

		if (particleName == "particles/items2_fx/teleport_start.vpcf")
			tpStarts[ent] = ParticleData{
			.name = particleName,
			.msgIdx = msgIndex
		};
		else if (particleName == "particles/items2_fx/teleport_end.vpcf")
			tpEnds[ent] = ParticleData{
			.name = particleName,
			.msgIdx = msgIndex
		};

	}
	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_TRANSFORM:
	{
		int cp = pmMsg->update_particle_transform().control_point();
		if (cp != 0)
			break;

		auto pos = pmMsg->update_particle_transform().position();

		for (auto& [_, data] : tpStarts)
			if (data.msgIdx == msgIndex)
				data.pos = Vector(pos.x(), pos.y(), pos.z());
		for (auto& [_, data] : tpEnds)
			if (data.msgIdx == msgIndex)
				data.pos = Vector(pos.x(), pos.y(), pos.z());

		break;
	}
	case GAME_PARTICLE_MANAGER_EVENT_DESTROY: {
		bool cancelled = pmMsg->destroy_particle().destroy_immediately();
		for (auto it = tpStarts.begin(); it != tpStarts.end(); )
		{
			if (it->second.msgIdx == msgIndex) {
				it = tpStarts.erase(it);
				break;
			}
			else ++it;
		}
		for (auto it = tpEnds.begin(); it != tpEnds.end(); )
		{
			if (it->second.msgIdx == msgIndex) {
				it = tpEnds.erase(it);
				break;
			}
			else ++it;
		}
		break;
	}
	};

}
