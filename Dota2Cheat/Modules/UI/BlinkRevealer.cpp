#include "BlinkRevealer.h"

void Hacks::BlinkRevealer::Draw() {
	if (!Config::BlinkRevealer)
		return;
	MTM_LOCK;

	static constexpr ImVec2 iconSize{ 48,48 };
	for (auto& [hero, data] : Blinks) {
		if (!hero->GetIdentity()->IsDormant())
			continue;
		auto DrawList = ImGui::GetForegroundDrawList();
		auto iconName = data.qop ? "queenofpain_blink" : "antimage_blink";

		auto icon = texManager.GetNamedTexture(iconName);
		if (!icon)
		{
			auto iconPath = ctx.cheatFolderPath + "\\assets\\spellicons\\" + iconName + "_png.png";
			texManager.LoadTextureNamed(iconPath.c_str(), icon, iconName);
		}
		auto fade = int(data.fadeCounter / data.fadeTime * 255);
		auto drawPos = WorldToScreen(data.pos);
		DrawList->AddCircle(
			drawPos,
			iconSize.x / 2 + 2,
			ImColor{ 0,0,0, fade },
			0,
			2
		); 
		DrawList->AddImageRounded(
			icon,
			drawPos - iconSize / 2,
			drawPos + iconSize / 2,
			{ 0,0 },
			{ 1,1 },
			ImColor{ 255, 255, 255, fade },
			iconSize.x / 2);
	}
}

void Hacks::BlinkRevealer::FrameBasedLogic() {
	MTM_LOCK;

	auto timeDelta = GameSystems::GameRules->GetGameTime() - lastTime;
	lastTime = GameSystems::GameRules->GetGameTime();
	for (auto it = Blinks.begin(); it != Blinks.end();) {
		auto& data = it->second;
		data.fadeCounter -= timeDelta;
		if (data.fadeCounter <= 0)
			it = Blinks.erase(it);
		else
			++it;
	}
}

void Hacks::BlinkRevealer::ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
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

		const auto m_szParticleName = Interfaces::ResourceSystem->GetResourceName(particle.particle_name_index());
		if (!m_szParticleName)
			break;

		auto ent = Interfaces::EntitySystem->GetEntity(NH2IDX(particle.entity_handle()));
		if(!ent)
			ent = Interfaces::EntitySystem->GetEntity(NH2IDX(particle.entity_handle_for_modifiers()));
		if (!ent || ent->IsSameTeam(ctx.localHero))
			break;
		std::string_view particleName = m_szParticleName;

		constexpr static uint32 am = "particles/units/heroes/hero_antimage/antimage_blink_end.vpcf"_city32,
			qop = "particles/units/heroes/hero_queenofpain/queen_blink_end.vpcf"_city32;

		switch (CityHash32(particleName)) {
		case am: Blinks[ent].qop = false; break;
		case qop: Blinks[ent].qop = true; break;
		}
	}
	case GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENT: {

		if (pmMsg->update_particle_ent().control_point() == 1) {
			auto pos = pmMsg->update_particle_ent().fallback_position();
			auto hero = Interfaces::EntitySystem->GetEntity(NH2IDX(pmMsg->update_particle_ent().entity_handle()));
			if (Blinks.contains(hero))
			{
				auto& data = Blinks[hero];
				data.pos = Vector{ pos.x(), pos.y(), pos.z() };
				data.fadeCounter = data.fadeTime = 3;
			}
		}
		break;
	}
	};

}
