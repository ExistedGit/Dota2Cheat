#pragma once
#include "../pch.h"
#include "../CheatSDK/include.h"

// Source engine's networking relies on executing frames in stages, such as
// receiving net packages, simulating, predicting and only then rendering

// FSN stages:
//
// 0, 1 = CNetworkGameClient::ClientProcessNetworking
// 2 = CNetworkGameClient::ProcessPacketEntities
// 3 = CNetworkGameClient::ClientSidePredict
// 4, 5, 6, 7 = CNetworkGameClient::ProcessPacketEntities
// 8 = CNetworkGameClientBase::ProcessSpawnGroup_SetCreationTick
// 9 = CNetworkGameClient::OnClientFrameSimulate
//
// CNetworkGameClient::ProcessPacketEntities:
// 2 => receiving non-incremental updates => 4 => read packet entities => 5 => PostDataUpdate(ClientSimulate event) => 6 => packet entities parsing => 7 

namespace Hooks {
	inline struct FrameEventListener {
		using EventFrameBoundary_t = NormalClass;

		void OnFrameBoundary(EventFrameBoundary_t* ev);
	} frameListener;

	inline void* oFrameStageNotify{};
	void hkFrameStageNotify(void* thisptr, int stage);
}