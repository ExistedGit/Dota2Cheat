#pragma once

#include "../../Modules/UI/D2CUIModule.h"

class D2CModuleManager {
	std::vector<D2CUIModule*> uiModules;
	std::vector<IFrameListener*> frameListeners;
	std::vector<INetChanListener*> netChanListeners;
public:
	void SubscribeUIModule(D2CUIModule* m) {
		uiModules.push_back(m);
	}

	// Called from frame hook in FrameStageNotify.h
	void OnFrame() const {
		for (auto m : uiModules) {
			if (m->Active()) m->OnFrame();
		}
	}

	// Called in Present
	void DrawUI() const {
		for (auto m : uiModules) {
			if (m->Active()) m->Draw();
		}
	}

	void OnMessage(NetMessageHandle_t* handle, google::protobuf::Message* msg) const {
		for (auto m : netChanListeners) {
			m->OnReceivedMsg(handle, msg);
		}
	}
};