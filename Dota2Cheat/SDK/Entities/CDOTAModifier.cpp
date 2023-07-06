#include "CDOTAModifier.h"

CDOTABaseNPC* CDOTAModifier::GetOwner() {
	return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(
		H2IDX(
			Member<ENT_HANDLE>(Netvars::CDOTA_Buff::m_hParent)
		)
		);
}
