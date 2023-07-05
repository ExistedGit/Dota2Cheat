#include "TPTracker.h"
#include <consthash/cityhash32.hxx>
#include <cityhash/city.h>

// Mostly calculating fade duration

void Hacks::TPTracker::FrameBasedLogic() {
	MTM_LOCK;

	auto timeDelta = GameSystems::GameRules->GetGameTime() - lastTime;
	lastTime = GameSystems::GameRules->GetGameTime();
	for (auto it = teleports.begin(); it != teleports.end();) {
		auto& data = it->second;
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
		else
			++it;
	}
}

void Hacks::TPTracker::DrawMapTeleports() {
	if (!Config::TPTracker::Enabled)
		return;

	MTM_LOCK;
	constexpr static ImVec2 iconSize{ 24,24 };
	auto  DrawList = ImGui::GetForegroundDrawList();
	for (auto& [ent, data] : teleports) {
		if (ent->IsSameTeam(ctx.localHero)
			|| !data.start.msgIdx || !data.end.msgIdx)
			continue;

		ImVec2 start = WorldToMap(data.start.pos), end = WorldToMap(data.end.pos);

		DrawList->AddLine(start, end, data.color, 3);

		if (ent->GetIdentity()->IsDormant() || data.isFading) {
			ImVec2 startXY1 = start - iconSize / 2, startXY2 = startXY1 + iconSize;
			DrawList->AddImage(data.icon,
				startXY1,
				startXY2,
				{ 0,0 },
				{ 1,1 },
				data.isFading ?
				(ImU32)ImColor {
				255, 255, 255, (int)(192 * ImColor{ data.color }.Value.w)
			}
			:
				(ImU32)ImColor {
				255, 255, 255, 255
			}
			);
		}

		if (!data.isFading || (data.isFading && !data.cancelled)) {
			ImVec2 endXY1 = end - iconSize / 2, endXY2 = endXY1 + iconSize;
			DrawList->AddImage(data.icon,
				endXY1,
				endXY2,
				{ 0,0 },
				{ 1,1 },
				data.isFading
				? (ImU32)ImColor { 255, 255, 255, 255 }
			:
				(ImU32)ImColor {
				255, 255, 255, 128
			}
			);
		}
	}
}

void Hacks::TPTracker::OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID != 145)
		return;

	MTM_LOCK;

	auto pmMsg = reinterpret_cast<CUserMsg_ParticleManager*>(msg);
	auto msgIndex = pmMsg->index();
	switch (pmMsg->type()) {
	case GAME_PARTICLE_MANAGER_EVENT_CREATE: {
		auto particle = pmMsg->create_particle();
		if (!particle.has_particle_name_index())
			break;

		const auto particleName = Interfaces::ResourceSystem->GetResourceName(particle.particle_name_index());
		if (!particleName)
			break;

		auto ent = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(NH2IDX(particle.entity_handle_for_modifiers()));
		if (!ent || !ent->GetUnitName())
			break;

		std::string prefixLessName = std::string(ent->GetUnitName()).substr(14),
			iconName = "icon_" + prefixLessName;

		auto icon = texManager.GetNamedTexture(iconName);

		switch (CityHash32(std::string_view(particleName))) {
		case "particles/items2_fx/teleport_start.vpcf"_city32: {
			auto& tpData = teleports[ent];
			tpData.fadeCounter = tpData.fadeDuration = tpData.isFading = 0;
			tpData.color = ImColor{ 220,220,220 };
			tpData.start = TPData{
						.msgIdx = msgIndex
			};
			tpData.icon = icon;
			break;
		}
		case "particles/items2_fx/teleport_end.vpcf"_city32: {
			auto& tpData = teleports[ent];
			tpData.fadeCounter = tpData.fadeDuration = tpData.isFading = 0;
			tpData.color = ImColor{ 220,220,220 };
			tpData.end = TPData{
			.msgIdx = msgIndex
			};
			tpData.icon = icon;
			break;
		}
		}

	}
	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_TRANSFORM:
	{
		if (pmMsg->update_particle_transform().control_point() == 0) {
			auto pos = pmMsg->update_particle_transform().position();

			for (auto& [_, data] : teleports)
				if (data.start.msgIdx == msgIndex)
					data.start.pos = Vector(pos.x(), pos.y(), pos.z());
			for (auto& [_, data] : teleports)
				if (data.end.msgIdx == msgIndex)
					data.end.pos = Vector(pos.x(), pos.y(), pos.z());
		}
		break;
	}
	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENT: {

		if (pmMsg->update_particle_ent().control_point() == 0) {
			auto pos = pmMsg->update_particle_ent().fallback_position();

			for (auto& [_, data] : teleports)
				if (data.start.msgIdx == msgIndex)
					data.start.pos = Vector(pos.x(), pos.y(), pos.z());
			for (auto& [_, data] : teleports)
				if (data.end.msgIdx == msgIndex)
					data.end.pos = Vector(pos.x(), pos.y(), pos.z());
		}
		break;
	}
	case GAME_PARTICLE_MANAGER_EVENT_DESTROY: {
		bool cancelled = pmMsg->destroy_particle().destroy_immediately();
		for (auto& [_, data] : teleports)
		{
			if (
				data.start.msgIdx == msgIndex
				|| data.end.msgIdx == msgIndex
				) {
				data.isFading = true;
				if (cancelled) {
					data.color = ImColor{ 255,0,0 };
					data.fadeDuration = data.fadeCounter = Config::TPTracker::FadeDuration/3;
				}
				else {
					data.color = ImColor{ 0,255,0 };
					data.fadeDuration = data.fadeCounter = Config::TPTracker::FadeDuration;
				}
				data.cancelled = cancelled;
				break;
			}
		}
		break;
	}
	};

}
