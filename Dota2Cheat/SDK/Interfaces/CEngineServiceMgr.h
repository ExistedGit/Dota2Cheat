#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../Base/Logging.h"

#include "CSchemaSystem.h"

class CUtlAbstractDelegate;

struct CEventDispatcher {
	// engine2.dll CEventDispatcher_Identified<CEventIDManager_Default>::RegisterEventListener_Abstract and UnregisterEventListener_Abstract 
	// follow up FSN's call chain and compare to dylibs
	// CLoopTypeClientServer::Update triggers various delegates in templated dispatch functions

	static inline void (*_UnregisterEventListener_Abstract)(CEventDispatcher* thisptr, const CUtlAbstractDelegate& del, const CSchemaClassBinding* binding);
	static inline void (*_RegisterEventListener_Abstract)(CEventDispatcher* thisptr, const CUtlAbstractDelegate& del, int a3, int a4, const CSchemaClassBinding* binding, int a6, const char* name);

	void UnregisterEventListener_Abstract(const CUtlAbstractDelegate& del, const CSchemaClassBinding* binding) {
		if (_UnregisterEventListener_Abstract)
			_UnregisterEventListener_Abstract(this, del, binding);
	}


	void RegisterEventListener_Abstract(const CUtlAbstractDelegate& del, int a3, int a4, const CSchemaClassBinding* binding, int a6, const char* name) {
		if (_RegisterEventListener_Abstract)
			_RegisterEventListener_Abstract(this, del, a3, a4, binding, a6, name);
	}

	// Helper functions
	void UnregisterEventListener_Abstract(const CUtlAbstractDelegate& del, const char* evSchemaName) {
		auto binding = CSchemaSystem::Get()->FindTypeScopeForModule("engine2.dll")->FindDeclaredClass(evSchemaName);
		if (!binding) {
			LogFE("{}: missing binding for event '{}'", __FUNCTION__, evSchemaName);
			return;
		}

		if (_UnregisterEventListener_Abstract)
			_UnregisterEventListener_Abstract(this, del, binding);
	}

	void RegisterEventListener_Abstract(const CUtlAbstractDelegate& del, const char* evSchemaName, const char* name) {
		auto binding = CSchemaSystem::Get()->FindTypeScopeForModule("engine2.dll")->FindDeclaredClass(evSchemaName);
		if (!binding) {
			LogFE("{}: missing binding for event '{}'", __FUNCTION__, evSchemaName);
			return;
		}

		if (_RegisterEventListener_Abstract)
			_RegisterEventListener_Abstract(this, del, 1, 1, binding, 0, name);
	}
};

class CEngineServiceMgr : public VClass {
public:
	VGETTER(CEventDispatcher*, GetEventDispatcher, 29);
	
	void GetEngineSwapChainSize(int* outW, int* outH) {
		GetVFunc(VMI::CEngineServiceMgr::GetEngineSwapChainSize)(outW, outH);
	}

	static CEngineServiceMgr* Get();
};