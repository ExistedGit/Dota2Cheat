#pragma once
#include "MatchStateHandling.h"
#include "AutoBuyTome.h"
#include "AutoUseMidas.h"
#include "AutoUseMagicWand.h"
#include "Wrappers.h"
#include "UIState.h"
#include "Input.h"

extern bool IsInMatch;

namespace VMTs {
	std::unique_ptr<VMT> Panorama2;
	std::unique_ptr<VMT> Engine;
	std::unique_ptr<VMT> Entity;
}
namespace Hooks {
	typedef BaseEntity* (*OnAddEntityFn)(CEntitySystem*, BaseEntity*, ENT_HANDLE);
	typedef void (*RunFrameFn)(u64, u64);



	inline void LogEntities() {
		for (int i = 0; i < Interfaces::Entity->GetHighestEntityIndex(); i++) {
			auto* ent = Interfaces::Entity->GetBaseEntity(i);
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
				std::cout << '\t' << ability.name << " " << ENTID_FROM_HANDLE(ability.handle) << " CD: " << ability.GetAs<BaseAbility>()->GetCooldown() << '\n';
		}
		std::cout << "inventory: " << '\n';
		for (const auto& item : npc->GetItems()) {
			if (item.name != nullptr)
				std::cout << '\t' << item.name << " " << ENTID_FROM_HANDLE(item.handle) << '\n';
		}
	}
	inline bool TestStringFilters(const char* str, std::vector<const char*> filters) {
		for (auto& filter : filters)
			if (strstr(str, filter))
				return true;

		return false;
	}
	inline void EntityIteration() {
		int illusionCount = 0;
		bool midasUsed = false;
		bool runePickUp = false;

		ENT_HANDLE midas = AutoUseMidasCheck(assignedHero);

		for (int i = 0; i < Interfaces::Entity->GetHighestEntityIndex(); i++) {
			auto* ent = Interfaces::Entity->GetBaseEntity(i);

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

				// If the creep is visible, not one of ours, is alive, is within 600 hammer units and its name matches one of the filters
				if (creep->GetTeam() != assignedHero->GetTeam() &&
					creep->GetHealth() > 0 &&
					!creep->IsWaitingToSpawn() &&
					IsWithinRadius(creep->GetPos2D(), assignedHero->GetPos2D(), 600) &&
					TestStringFilters(creep->GetUnitName(), filters)) {
					//std::cout << creep->GetUnitName() << '\n';
					midasUsed = true;
					localPlayer->PrepareOrder(DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_TARGET, i, &Vector3::Zero, ENTID_FROM_HANDLE(midas), PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, assignedHero);
				}
			}
			else if (!runePickUp && strstr(className, "C_DOTA_Item_Rune")) {
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
						Interfaces::Entity->GetBaseEntity(
							localPlayer->GetSelectedUnits()[0]
						) == assignedHero
						)
						localPlayer->PrepareOrder(DotaUnitOrder_t::DOTA_UNIT_ORDER_PICKUP_RUNE, i, &Vector3::Zero, 0, PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_SELECTED_UNITS, nullptr, false, false);
				}
			}
			else if (strstr(className, "DOTA_Unit_Hero") != nullptr) {
				auto hero = (BaseNpc*)ent;

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
			if (inGameStuff && IsInMatch) {
				UpdateCameraDistance();
				//int x, y;
				//Signatures::WorldToScreen(Vector3::Zero,&x, &y, nullptr);
				//std::cout << std::dec << x << ' ' << y << '\n';
				if (assignedHero->GetLifeState() == 0) { // if alive
					//visible = prevSSC == prevSSC2 && assignedHero->GetSSC() == prevSSC;
					sscSum += assignedHero->GetSSC();
					sscCount++;
					if (sscCount == 3) {
						//if (visible != (sscSum == 0))
						//	std::cout << (visible ? "HIDDEN" : "DETECTED") << '\n';

						UIState::HeroVisibleToEnemy = visible = sscSum == 0;
						sscCount = sscSum = 0;
					}

					AutoUseWandCheck(assignedHero, Config::AutoHealWandHPTreshold, Config::AutoHealWandMinCharges);
					AutoUseFaerieFireCheck(assignedHero, Config::AutoHealFaerieFireHPTreshold);
					Hacks::AutoBuyTomeCheck();
					EntityIteration();
				}

				if (IsKeyPressed(VK_NUMPAD8)) {
					auto selected = localPlayer->GetSelectedUnits();
					auto ent = (BaseNpc*)Interfaces::Entity->GetBaseEntity(selected[0]);
					auto pos = ent->GetPos();
					std::cout << std::dec << "ENT " << selected[0] << " -> " << ent
						<< "\n\t" << "POS " << pos.x << ' ' << pos.y << ' ' << pos.z
						<< "\n\t" << "IsAncient: " << ent->IsAncient()
						//<< "\n\t" << "GetCastRangeBonus: " << std::dec << Function(0x00007FFAEE5C0B00).Execute<int>(nullptr, ent->GetIdentity()->entHandle)
						<< '\n';
				}
				if (IsKeyPressed(VK_NUMPAD7)) {
					auto selected = localPlayer->GetSelectedUnits();
					auto ent = (BaseNpc*)Interfaces::Entity->GetBaseEntity(selected[0]);
					LogInvAndAbilities(ent);
					//auto ab = ent->GetAbilities()[2];
					//std::cout << std::dec << selected[0] << " " << std::dec<< ab.name << " " << ab.GetAs<BaseAbility>()->GetCooldown() << '\n';
					//LogEntities();
				}

				if (IsKeyPressed(VK_HOME)) {
					Interfaces::CVar->SetConvars();
				}
			}
		}
		VMTs::Panorama2->GetOriginalMethod<RunFrameFn>(6)(a, b);
	}

	BaseEntity* OnAddEntity(CEntitySystem* thisptr, BaseEntity* ent, ENT_HANDLE handle) {
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
}
