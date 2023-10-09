#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	inline class M_TreeChanger {

		bool needsUpdate = false;
		bool needsRestore = false;

	public:
		void RestoreTreeModels();
		void UpdateTreeModels();

		struct TreeModelInfo {
			const char* modelName;
			float scale;
		};

		struct SavedModelInfo : public TreeModelInfo {
			uint64_t meshGroupMask;
		};

		std::map<CBaseEntity*, SavedModelInfo> originalTrees;

		TreeModelInfo queuedModel;

		void QueueModelUpdate(const TreeModelInfo& mdlInfo) {
			queuedModel = mdlInfo;
			needsUpdate = true;
		}
		void QueueModelRestore() {
			needsRestore = true;
		}

		void Reset() {
			originalTrees.clear();
		}

		void SetTreeModel(CBaseEntity* tree, const TreeModelInfo& mdl);
	} TreeChanger{};
}