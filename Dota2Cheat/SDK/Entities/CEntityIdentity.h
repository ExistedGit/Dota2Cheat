#pragma once
#include "../Base/Definitions.h"

class CBaseEntity;

class CEntityIdentity {
public:
	CBaseEntity* entity;
	void* dunno;
	int entHandle; // LOWORD(handle) & 0x7FFF = entID
	int unk2; // always seems to be -1
	const char* internalName; // these two strings are optional!
	const char* entityName; // ex: item_tpscroll
private:
	void* unkptr3;
public:
	union {
		char m_bFlags[4];
		DWORD m_nFlags;
	};
private:
	PAD(4);
	PAD(4 * 8);
public:
	CEntityIdentity* m_pPrev;
	CEntityIdentity* m_pNext;
	CEntityIdentity* m_pPrevByClass;
	CEntityIdentity* m_pNextByClass;
public:
	const char* GetName() const {
		return internalName ? internalName : entityName;
	}

	bool IsDormant() const {
		return (m_bFlags[0] & 0x80);
	}
};