#pragma once
#include "sdk.h"
#include "Globals.h"

namespace Hacks {
	// For each of Invoker's 10 spells there is a CMsg_InvokerSpellCast message with different cast_activity for each spell
	// When we receive such a message with sunstrike's cast_activity(1743) we begin waiting for a thinker object to appear
	// It's created right after the message AND has no names in its identity, so it's easy to filter it
	// Then it's just the matter of rendering the particle(it uses Magus Apex's version which shows radius better)
	class SunStrikeHighlighter {
	private:
		BaseEntity* SunStrikeThinker = nullptr;
	public:
		bool SunStrikeIncoming = false;

		void QueueThinker(BaseEntity* thinker) {
			SunStrikeThinker = thinker;
			SunStrikeIncoming = false;
		}

		void FrameBasedLogic() {
			if (SunStrikeThinker 
				&& SunStrikeThinker->GetPos() != Vector3::Zero) {
				DrawEnemySunstrike(SunStrikeThinker->GetPos());
				SunStrikeThinker = nullptr;
			}
		}

		void ProcessMessage(NetMessageHandle_t* messageHandle, google::protobuf::Message* msg) {
			if (messageHandle->messageID == 586) { // CDOTAEntityMsg_InvokerSpellCast
				auto castMsg = reinterpret_cast<CDOTAEntityMsg_InvokerSpellCast*>(msg);
				if (castMsg->cast_activity() == 1743) { //sunstrike
					auto invoker = Interfaces::EntitySystem->GetEntity(
						castMsg->entity_msg().target_entity() & 0x3fff // weird smaller mask
					); 

					if (invoker  &&
						invoker->GetTeam() != ctx.assignedHero->GetTeam())
						SunStrikeIncoming = true;
				}
			}
		}

		void DrawEnemySunstrike(Vector3 pos) {
			Globals::ParticleManager->CreateParticle(
				"particles/econ/items/invoker/invoker_apex/invoker_sun_strike_team_immortal1.vpcf",
				PATTACH_WORLDORIGIN,
				nullptr).particle
				->SetControlPoint(0, &pos)
				->SetControlPoint(1, &Vector3::Zero);
		}
	};
}
namespace Modules {
	inline Hacks::SunStrikeHighlighter SunStrikeHighlighter{};
}