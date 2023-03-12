#include "CDOTAModifier.h"

CDOTABaseNPC* CDOTAModifier::GetOwner() {
	return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(
		H2IDX(
			Member<ENT_HANDLE>(Netvars::CDOTA_Buff::m_hParent)
		)
		);
}

void CDOTAModifier::BindLua(sol::state& lua) {
	auto type = lua.new_usertype<CDOTAModifier>("CDOTAModifier");

	type["GetName"] = &CDOTAModifier::GetName;
	type["GetDuration"] = &CDOTAModifier::GetDuration;
	type["GetDieTime"] = &CDOTAModifier::GetDieTime;
	type["GetCaster"] = &CDOTAModifier::GetCaster;
	type["GetAbility"] = &CDOTAModifier::GetAbility;
	type["GetOwner"] = &CDOTAModifier::GetOwner;
}
