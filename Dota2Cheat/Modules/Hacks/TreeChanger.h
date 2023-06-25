#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	inline class M_TreeChanger {

		bool needsUpdate = false;
	public:
		struct TreeModelInfo {
			const char* modelName;
			float scale;
		};
		TreeModelInfo queuedModel;

		void QueueModelUpdate(TreeModelInfo mdlInfo) {
			queuedModel = mdlInfo;
			needsUpdate = true;
		}
		void UpdateTreeModels() {
			if (!needsUpdate)
				return;

			static Function setMdl = Memory::Scan("E8 ? ? ? ? 8B 7D 6F", "client.dll").GetAbsoluteAddress(1);

			if (!setMdl.ptr)
				return;

			for (auto tree : GameSystems::BinaryObjectSystem->GetTrees()) {
				if (!tree)
					continue;

				setMdl(tree, queuedModel.modelName);
				tree->ModelScale() = queuedModel.scale;
				tree->Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)->CallVFunc<10>(4);
			}

			needsUpdate = false;
		}
	} TreeChanger{};
}