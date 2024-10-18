#include "Netvars.h"
#include "Interfaces/CSchemaSystem.h"
#include "Base/Logging.h"

uint16_t netvar_t::GetOffset() {
	if (offset.has_value()) return *offset;

	auto schemaClass = CSchemaSystem::Get()->FindTypeScopeForModule(dll.data())->FindDeclaredClass(name.data());
	for (int i = 0; i < schemaClass->m_fields_size; i++) {
		if (schemaClass->m_fields[i].m_name == member)
			return *(offset = schemaClass->m_fields[i].m_single_inheritance_offset);
	}

	LogFE("Could not find netvar {}!{}::{}!", dll, name, member);

	return *(offset = 0);
}
