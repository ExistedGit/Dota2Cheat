#include "TPTracker.h"
#include <usermessages.pb.h>

// Mostly calculating fade duration

void Modules::M_TPTracker::OnFrame() {
	MTM_LOCK;

	auto timeDelta = CGameRules::Get()->GetGameTime() - lastTime;
	lastTime = CGameRules::Get()->GetGameTime();
	for (auto it = teleports.begin(); it != teleports.end();) {
		auto& data = *it;

		if (!data.isFading) {
			++it;
			continue;
		}

		data.fadeCounter -= timeDelta;

		ImColor newColor{ data.color };
		newColor.Value.w = data.fadeCounter / data.fadeDuration;
		data.color = newColor;
		if (data.fadeCounter <= 0)
			it = teleports.erase(it);
		else {
			++it;
		}
	}
}

void Modules::M_TPTracker::Draw() {
	if (!Config::TPTracker::Enabled)
		return;

	MTM_LOCK;
	constexpr static ImVec2 iconSize{ 24,24 };
	auto  DrawList = ImGui::GetBackgroundDrawList();
	for (auto& data : teleports) {
		if (data.ent->IsSameTeam(ctx.localHero)
			|| !data.start.msgIdx || !data.end.msgIdx)
			continue;

		ImVec2 start = data.start.pos, end = data.end.pos;

		DrawList->AddLine(start, end, data.color, 3);

		if (data.ent->GetIdentity()->IsDormant() || data.isFading) {
			ImVec2 startXY1 = start - iconSize / 2, startXY2 = startXY1 + iconSize;
			DrawList->AddImage(data.icon,
				startXY1,
				startXY2,
				{ 0,0 },
				{ 1,1 },
				ImColor(255, 255, 255, data.isFading ? (int)(192 * ImColor{ data.color }.Value.w) : 255)
			);
		}

		if (!data.isFading || (data.isFading && !data.cancelled)) {
			ImVec2 endXY1 = end - iconSize / 2, endXY2 = endXY1 + iconSize;
			DrawList->AddImage(data.icon,
				endXY1,
				endXY2,
				{ 0,0 },
				{ 1,1 },
				ImColor{ 255, 255, 255, data.isFading ? 255 : 128 }
			);
		}
	}
}

void Modules::M_TPTracker::OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID != UM_ParticleManager)
		return;

	MTM_LOCK;

	auto pmMsg = reinterpret_cast<CUserMsg_ParticleManager*>(msg);
	auto particleIndex = pmMsg->index();
	switch (pmMsg->type()) {
	case GAME_PARTICLE_MANAGER_EVENT_CREATE: {
		const auto& particle = pmMsg->create_particle();
		if (!particle.has_particle_name_index())
			return;

		std::string_view particleName;
		{
			auto szParticleName = CResourceSystem::Get()->GetResourceName(particle.particle_name_index());
			if (!szParticleName)
				return;

			particleName = szParticleName;
		}

		auto ent = CEntSys::Get()->GetEntity<CDOTABaseNPC>(NH2IDX(particle.entity_handle_for_modifiers()));
		if (!ent || !ent->GetUnitName())
			return;

		if (particleName == "particles/items2_fx/teleport_start.vpcf") {
			TPLineData tpData;
			tpData.ent = ent;
			tpData.icon = assets.heroIcons.Load(ent->GetSmallIcon());
			tpData.fadeCounter =
				tpData.fadeDuration =
				tpData.isFading = 0;
			tpData.color = ImColor{ 220,220,220 };
			tpData.start = TPData{
			.msgIdx = particleIndex
			};

			teleports.push_front(tpData);
		}
		else if (particleName == "particles/items2_fx/teleport_end.vpcf") {
			auto it = std::find_if(teleports.begin(), teleports.end(), [ent](const auto& data) { return data.ent == ent; });
			if (it == teleports.end()) return;

			it->end = TPData{
				.msgIdx = particleIndex
			};
		}
		break;
	}
	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_TRANSFORM:
	{
		if (pmMsg->update_particle_transform().control_point() != 0)return;

		auto pos = pmMsg->update_particle_transform().position();
		UpdateTeleport(particleIndex, pos);

		break;
	}
	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENT: {

		if (pmMsg->update_particle_ent().control_point() != 0) return;

		auto pos = pmMsg->update_particle_ent().fallback_position();
		UpdateTeleport(particleIndex, pos);
		break;
	}
	case GAME_PARTICLE_MANAGER_EVENT_DESTROY: {
		bool cancelled = pmMsg->destroy_particle().destroy_immediately();
		for (auto& data : teleports)
		{
			if (
				data.start.msgIdx != particleIndex
				&& data.end.msgIdx != particleIndex
				) return;

			data.isFading = true;
			if (cancelled) {
				data.color = ImColor{ 255,0,0 };
				data.fadeDuration = data.fadeCounter = Config::TPTracker::FadeDuration / 3.f;
			}
			else {
				data.color = ImColor{ 0,255,0 };
				data.fadeDuration = data.fadeCounter = Config::TPTracker::FadeDuration;
			}
			data.cancelled = cancelled;
			break;

		}
		break;
	}
	};

}
