#pragma once
#include "../Base/VClass.h"
#include "../Base/NormalClass.h"
#include "../Base/Definitions.h"
#include "../Base/CUtlVector.h"

struct NetworkedVar {
	const char* name;
	void* classDef;
	void* callback;
};

struct CFlattenedSerializer2 : public NormalClass {
	FIELD(CUtlVector<NetworkedVar*>, NetVars, 0x10);
};

struct CFlattenedSerializer : public NormalClass {
	const char* name; // ex: "C_DOTA_Unit_Hero_AntiMage"
	CFlattenedSerializer2* s2;
};

class CFlattenedSerializers : public VClass {
public:
	struct FSListNode {
		const char* className; // ex: "client.dll!C_DOTA_Unit_Hero_AntiMage"
		CFlattenedSerializer* c;
	};

	FIELD(CUtlVector<FSListNode>, NetworkedClasses, 0x18);
};