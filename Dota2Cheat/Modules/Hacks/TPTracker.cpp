#include "TPTracker.h"


// Mostly calculating fade duration

void Hacks::TPTracker::FrameBasedLogic() {
	if (lastTime == 0) {
		lastTime = GameSystems::GameRules->GetGameTime();
		return;
	}
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

void Hacks::TPTracker::CacheHeroIcons() {
	for (auto& hero : ctx.heroes) {
		if (heroIcons.count(hero))
			continue;
		std::string prefixLessName = std::string(hero->GetUnitName()).substr(14),
			iconName = "icon_" + prefixLessName;
		heroIcons[hero] = texManager.GetNamedTexture(iconName);
	}
}

void Hacks::TPTracker::DrawMapTeleports() {
	if (!Config::TPTracker::Enabled)
		return;

	constexpr static ImVec2 iconSize{ 24,24 };
	auto  DrawList = ImGui::GetForegroundDrawList();
	for (auto& [ent, data] : teleports) {
		if (ent->IsSameTeam(ctx.assignedHero)
			|| !data.start.msgIdx || !data.end.msgIdx)
			continue;

		ImVec2 start = WorldToMap(data.start.pos), end = WorldToMap(data.end.pos);

		DrawList->AddLine(start, end, data.color, 3);

		if (!heroIcons.count(ent))
			continue;

		auto texture = heroIcons[ent];
		if (!texture)
			continue;

		if (ent->GetIdentity()->IsDormant() || data.isFading) {
			ImVec2 startXY1 = start - iconSize / 2, startXY2 = startXY1 + iconSize;
			DrawList->AddImage(texture,
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
			DrawList->AddImage(texture,
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

		const auto particleName = Interfaces::ResourceSystem->GetResourceName(particle.particle_name_index());
		auto ent = Interfaces::EntitySystem->GetEntity(NH2IDX(particle.entity_handle_for_modifiers()));
		if (!ent)
			break;

		if (!strcmp( particleName, "particles/items2_fx/teleport_start.vpcf")) {
			auto& tpData = teleports[ent];
			tpData.fadeCounter = tpData.fadeDuration = tpData.isFading = 0;
			tpData.color = ImColor{ 220,220,220 };
			tpData.start = TPData{
						.msgIdx = msgIndex
			};
		}
		else if (!strcmp(particleName, "particles/items2_fx/teleport_end.vpcf")) {
			auto& tpData = teleports[ent];
			tpData.fadeCounter = tpData.fadeDuration = tpData.isFading = 0;
			tpData.color = ImColor{ 220,220,220 };
			tpData.end = TPData{
			.msgIdx = msgIndex
			};
		}

	}
	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_TRANSFORM:
	{
		if ( pmMsg->update_particle_transform( ).control_point( ) == 0 ) {
			auto pos = pmMsg->update_particle_transform( ).position( );

			for ( auto& [_, data] : teleports )
				if ( data.start.msgIdx == msgIndex )
					data.start.pos = Vector( pos.x( ), pos.y( ), pos.z( ) );
			for ( auto& [_, data] : teleports )
				if ( data.end.msgIdx == msgIndex )
					data.end.pos = Vector( pos.x( ), pos.y( ), pos.z( ) );
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
					data.fadeDuration = data.fadeCounter = 1.5f;
				}
				else {
					data.color = ImColor{ 0,255,0 };
					data.fadeDuration = data.fadeCounter = 5;
				}
				data.cancelled = cancelled;
				break;
			}
		}
		break;
	}
	};

}
