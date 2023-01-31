#pragma once
#include "MatchStateHandling.h"
#include "AutoBuyTome.h"
#include "AutoUseMidas.h"
#include "AutoUseMagicWand.h"
#include "Wrappers.h"
#include "UIState.h"
#include "Input.h"
#include "CDOTAParticleManager.h"

extern bool IsInMatch;
extern std::vector<BaseNpc*> enemyHeroes;
extern CDOTAParticleManager::ParticleWrapper particleWrap;

namespace VMTs {
	std::unique_ptr<VMT> Panorama2;
	std::unique_ptr<VMT> Engine;
	std::unique_ptr<VMT> Entity;
}
namespace Hooks {
	typedef BaseEntity* (*OnAddEntityFn)(CEntitySystem*, BaseEntity*, ENT_HANDLE);
	typedef void (*RunFrameFn)(u64, u64);



	inline void LogEntities() {
		for (int i = 0; i < Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
			auto* ent = Interfaces::EntitySystem->GetEntity(i);
			if (ent == nullptr)
				continue;
			//std::cout << ent->SchemaBinding() << '\n';
			const char* className = ent->SchemaBinding()->binaryName;
			if (className != nullptr && strstr(className, "Rune"))
				std::cout << className
				<< " // " << ent->GetPos2D().x << ' ' << ent->GetPos2D().y
				<< " // " << ent << '\n';
		}
	}
	inline void LogInvAndAbilities(BaseNpc* npc = nullptr) {
		if (npc == nullptr)
			npc = assignedHero;

		std::cout << std::dec;
		std::cout << "abilities: " << '\n';
		for (const auto& ability : npc->GetAbilities()) {
			if (ability.name != nullptr)
				std::cout << '\t' << ability.name << " " << H2IDX(ability.handle)
				//<< " CD: " << ability.GetAs<BaseAbility>()->GetCooldown() 
				//<< ' ' << std::dec << ability.GetAs<BaseAbility>()->GetEffectiveCastRange()
				<< ' ' << ability.GetEntity()
				<< '\n';
		}
		std::cout << "inventory: " << '\n';
		for (const auto& item : npc->GetItems()) {
			if (item.name != nullptr)
				std::cout << '\t' << item.name << " " << H2IDX(item.handle)
				<< ' ' << item.GetEntity()
				<< '\n';
		}
	}
	inline bool TestStringFilters(const char* str, std::vector<const char*> filters) {
		for (auto& filter : filters)
			if (strstr(str, filter))
				return true;

		return false;
	}
	inline bool test = false;
	inline void EntityIteration() {
		int illusionCount = 0;
		bool midasUsed = false;
		bool runePickUp = false;

		ENT_HANDLE midas = AutoUseMidasCheck(assignedHero);

		for (int i = 0; i < Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
			auto* ent = Interfaces::EntitySystem->GetEntity(i);

			if (ent == nullptr || ent->GetIdentity()->IsDormant())
				continue;
			const char* className = ent->SchemaBinding()->binaryName;

			if (className == nullptr)
				continue;
			if (!midasUsed && midas != -1
				&& strstr(className, "Creep")) {

				auto creep = (BaseNpc*)ent;
				//Fvector posHero = assignedHero->GetPos(), posCreep = creep->GetPos();

				//neutral prefixes because Wildwing Ripper and Dark Troll Warlord spawn a tornado and skeletons respectively
				//they have their summoner's name in them but not the word "neutral"
				static std::vector<const char*> filters = {
						"ranged",
						"flagbearer",
						"siege",
						"alpha_wolf",
						"centaur_khan",
						"neutral_dark_troll_warlord",
						"ursa_warrior",
						"ogre_magi",
						"satyr_hellcaller",
						"neutral_enraged_wildkin"
				};
				auto midasEnt = Interfaces::EntitySystem->GetEntity < BaseAbility>(H2IDX(midas));

				// If the creep is visible, not one of ours, is alive, is within 600 hammer units and its name matches one of the filters
				if (creep->GetTeam() != assignedHero->GetTeam() &&
					creep->GetHealth() > 0 &&
					!creep->IsWaitingToSpawn() &&
					IsWithinRadius(creep->GetPos2D(), assignedHero->GetPos2D(), midasEnt->GetEffectiveCastRange()) &&
					TestStringFilters(creep->GetUnitName(), filters)) {
					//std::cout << creep->GetUnitName() << '\n';
					midasUsed = true;
					localPlayer->PrepareOrder(DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_TARGET, i, &Vector3::Zero, H2IDX(midas), PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, assignedHero);
				}
			}
			else if (Config::AutoRunePickupEnabled && !runePickUp && strstr(className, "C_DOTA_Item_Rune")) {
				auto* rune = (ItemRune*)ent;
				//std::cout << "RUNE " << (int)rune->GetRuneType() << ' ' << rune->GetPos2D().x << ' ' << rune->GetPos2D().y
				//	<< ' ' << IsWithinRadius(rune->GetPos2D(), assignedHero->GetPos2D(), 150.0f)
				//	<< '\n';
				if (rune->GetRuneType() == DotaRunes::BOUNTY &&
					IsWithinRadius(rune->GetPos2D(), assignedHero->GetPos2D(), 150.0f)
					) {
					//std::cout << "Rune pickup!\n";
					//runePickUp = true;
					if (
						Interfaces::EntitySystem->GetEntity(
							localPlayer->GetSelectedUnits()[0]
						) == assignedHero
						)
						localPlayer->PrepareOrder(DotaUnitOrder_t::DOTA_UNIT_ORDER_PICKUP_RUNE, i, &Vector3::Zero, 0, PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_SELECTED_UNITS, nullptr, false, false);
				}
			}
			else if (strstr(className, "DOTA_Unit_Hero") != nullptr) {
				auto hero = (BaseNpc*)ent;
				if (!test) {
					enemyHeroes.push_back(hero);
					test = true;
				}

				//std::cout << std::hex;

				if (hero->Hero_IsIllusion() &&
					strstr(className, "CDOTA_Unit_Hero_ArcWarden") == nullptr) {
					illusionCount++;
					if (assignedHero->GetTeam() == hero->GetTeam())
						hero->SetColor(Color(0, 255, 0));
					else
						hero->SetColor(Color(255, 0, 0));
				}
			}

		}
		//rainbowIndex++;
		//rainbowIndex %= rainbow.size();
		//gotoxy(0, 5);
		//std::cout << "Illusions: " << illusionCount;
	}

	inline float sscCount = 0;
	inline float sscSum = 0;
	inline bool visible = false;

	inline void UpdateCameraDistance() {
		static auto varInfo = CVarSystem::CVar["dota_camera_distance"];
		if (Config::CameraDistance != varInfo.var->value.flt) {
			varInfo.var->value.flt = Config::CameraDistance;
			Interfaces::CVar->TriggerCallback(varInfo);
		}

	}

	inline void RunFrame(u64 a, u64 b) {
		const bool inGameStuff = true;
		static bool isInGame = Interfaces::Engine->IsInGame();

		if (isInGame) {
			//std::cout << "frame\n";
			if (inGameStuff && IsInMatch) {
				UpdateCameraDistance();
				if (assignedHero->GetLifeState() == 0) { // if alive
					// VBE: m_flStartSequenceCycle updates 30 times a second
					// It doesn't update when you are seen(AFAIK is set to zero)
					sscSum += assignedHero->GetSSC();
					sscCount++;
					if (sscCount == 3) {
						UIState::HeroVisibleToEnemy = visible = sscSum == 0;
						sscCount = sscSum = 0;
					}


					if (visible) {
						if (particleWrap.particle == nullptr) {
							//particleWrap = Globals::ParticleManager->CreateParticle(
							//	"particles/items5_fx/revenant_brooch.vpcf",
							//	CDOTAParticleManager::ParticleAttachment_t::PATTACH_ABSORIGIN_FOLLOW,
							//	(BaseEntity*)assignedHero
							//);
						}
					}
					else if (particleWrap.particle)
						Globals::ParticleManager->DestroyParticle(particleWrap);

					AutoUseWandCheck(assignedHero, Config::AutoHealWandHPTreshold, Config::AutoHealWandMinCharges);
					AutoUseFaerieFireCheck(assignedHero, Config::AutoHealFaerieFireHPTreshold);
					Hacks::AutoBuyTomeCheck();
					EntityIteration();
				}
#ifdef _DEBUG
				if (IsKeyPressed(VK_NUMPAD8)) {
					auto selected = localPlayer->GetSelectedUnits();
					auto ent = Interfaces::EntitySystem->GetEntity<BaseNpc>(selected[0]);
					auto pos = ent->GetPos();
					std::cout << std::dec << "ENT " << selected[0] << " -> " << ent
						<< "\n\t" << "POS " << pos.x << ' ' << pos.y << ' ' << pos.z
						//<< "\n\t" << "IsAncient: " << ent->IsAncient()
						//<< "\n\t" << "GetCastRangeBonus: " << std::dec << Function(0x00007FFAEE5C0B00).Execute<int>(nullptr, ent->GetIdentity()->entHandle)
						<< '\n';
				}
				if (IsKeyPressed(VK_NUMPAD7)) {
					auto selected = localPlayer->GetSelectedUnits();
					auto ent = (BaseNpc*)Interfaces::EntitySystem->GetEntity(selected[0]);
					LogInvAndAbilities(ent);
					//auto ab = ent->GetAbilities()[2];
					//std::cout << std::dec << selected[0] << " " << std::dec<< ab.name << " " << ab.GetAs<BaseAbility>()->GetCooldown() << '\n';
					//LogEntities();
				}
				if (IsKeyPressed(VK_NUMPAD3)) {
					auto midas = assignedHero->FindItemBySubstring("blink");
					if (HVALID(midas.handle))
						std::cout << std::dec << midas.GetAs<BaseAbility>()->GetVFunc(0xf4).ptr << '\n';

				}
				if (IsKeyPressed(VK_HOME)) {
					auto forceStaff = assignedHero->FindItemBySubstring("force");
					if (HVALID(forceStaff.handle)) {
						auto ent = forceStaff.GetAs<BaseAbility>();
						//auto pos = ent->GetPos();
						std::cout << std::dec << Function(0x7FF9FEAB1130)(nullptr, H2IDX(forceStaff.handle), "bonus_intellect", -1) << '\n';
						
						//std::cout << std::dec << Function(0x00007FF9FEA78000)(forceStaff.GetEntity(), "bonus_intellect", -1, 3) << '\n';
						//std::cout << std::dec << Function(0x00007FFB53AB6AD0).Execute<int>(ent, &pos) << '\n';
						//std::cout << std::dec << ent->GetEffectiveCastRange() << '\n';

						//std::cout << std::dec << Function(0x00007FFE04BF0A80).Execute<uint ptr_t>(nullptr, midas.handle) << '\n';

						//Interfaces::CVar->SetConvars();
					}
				}
#endif
			}
		}
		VMTs::Panorama2->GetOriginalMethod<RunFrameFn>(6)(a, b);
	}

	inline BaseEntity* OnAddEntity(CEntitySystem* thisptr, BaseEntity* ent, ENT_HANDLE handle) {
		bool fit = true;
		const char* name = ent->SchemaBinding()->binaryName;
		if (fit) {
			int entId = ((uint16_t)(handle) & 0xfff);
			//CDotaBaseNPC* npc = (CDotaBaseNPC*)ent;
			//if (name != nullptr)
			//	std::cout << name << " // " << ent << " || " << std::dec << entId << std::hex << std::endl;
			if (strstr(ent->SchemaBinding()->binaryName, "DOTA_Unit_Hero") != nullptr) {
				std::cout << ent->SchemaBinding()->binaryName << " // " << ent << '\n';
			}
			else if (strstr(ent->SchemaBinding()->binaryName, "C_DOTAPlayer") != nullptr) {
				auto* player = (DotaPlayer*)ent;
				std::cout << "Player // " << player << " || " << entId << std::endl;
			}

		}

		return VMTs::Entity->GetOriginalMethod<OnAddEntityFn>(14)(thisptr, ent, handle);
	};
	inline Signatures::prepareUnitOrdersFn PrepareUnitOrdersOriginal = nullptr;

	inline void PrepareUnitOrdersHook(DotaPlayer* player, DotaUnitOrder_t orderType, UINT32 targetIndex, Vector3* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue, bool showEffects) {
		//std::cout << "[ORDER] " << player << '\n';
		bool giveOrder = true; // whether or not the function will continue
		switch (orderType) {
		case DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_TARGET:
		{
			//Redirects spell casts from illusions to the real hero
			auto npc = Interfaces::EntitySystem->GetEntity<BaseNpc>(targetIndex);

			if (strstr(npc->SchemaBinding()->binaryName, "C_DOTA_Unit_Hero") &&
				npc->Hero_IsIllusion()) {
				auto assignedHero = Interfaces::EntitySystem->GetEntity<DotaPlayer>(
					H2IDX(
						npc->GetOwnerEntityHandle()
					)
					)
					->GetAssignedHero();
				if (assignedHero->IsTargetable()) {
					targetIndex =
						H2IDX(
							assignedHero
							->GetIdentity()
							->entHandle
						);
					showEffects = false;
				}
			}
			break;
		}
		case DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_POSITION: {
			// Blink overshoot bypass
			auto item = Interfaces::EntitySystem->GetEntity<BaseAbility>(abilityIndex);
			if (strstr(item->GetIdentity()->GetName(), "blink")) {
				auto maxDist = item->GetEffectiveCastRange();
				auto pos2D = *(Vector2*)position;
				if (!IsWithinRadius(issuer->GetPos2D(), pos2D, maxDist)) {
					auto dist = issuer->GetPos2D().DistanceTo(pos2D);
					// Relative vector from the hero to the click point
					auto vec = Vector2(pos2D.x - issuer->GetPos2D().x, pos2D.y - issuer->GetPos2D().y);
					// -1% to make it 100% be inside the radius
					vec.x *= maxDist / dist * 0.99;
					vec.x += issuer->GetPos2D().x;
					vec.y *= maxDist / dist * 0.99;
					vec.y += issuer->GetPos2D().y;

					position->x = vec.x;
					position->y = vec.y;
					showEffects = false;
				}
			}
			break;
		}
		case DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_NO_TARGET: {
			//Automatic mana & HP abuse with items like Arcane Boots or Faerie Fire
			std::cout << issuer->GetForwardVector(10) << '\n';

			break;
		}
		}

		if (giveOrder)
			PrepareUnitOrdersOriginal(player, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);
	}

	inline void SetUpByteHooks() {
		if (MH_CreateHook(Signatures::PrepareUnitOrders, &PrepareUnitOrdersHook,
			(LPVOID*)&PrepareUnitOrdersOriginal) != MH_OK ||
			MH_EnableHook(Signatures::PrepareUnitOrders) != MH_OK)
			std::cout << "Could not hook PrepareUnitOrders!\n";

	}

}
