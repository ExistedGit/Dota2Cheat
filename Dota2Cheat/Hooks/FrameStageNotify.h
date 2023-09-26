#pragma once
#include "../pch.h"

namespace Hooks {

	enum ClientFrameStage_t : int {
		FRAME_START,
		// A network packet is being recieved
		FRAME_NET_UPDATE_START,
		// Data has been received and we're going to start calling PostDataUpdate
		FRAME_NET_UPDATE_POSTDATAUPDATE_START,
		// Data has been received and we've called PostDataUpdate on all data recipients
		FRAME_NET_UPDATE_POSTDATAUPDATE_END,
		// We've received all packets, we can now do interpolation, prediction, etc..
		FRAME_NET_UPDATE_END,
		// We're about to start rendering the scene
		FRAME_RENDER_START,
		// We've finished rendering the scene.
		FRAME_RENDER_END,

		FRAME_SIMULATE = 8,

	};
	void* oFrameStageNotify;
	void hkFrameStageNotify(void* thisptr, int stage) {
		((decltype(&hkFrameStageNotify))oFrameStageNotify)(thisptr, stage);
	}


}