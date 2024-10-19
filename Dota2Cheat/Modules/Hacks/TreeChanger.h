#pragma once
#include "../../CheatSDK/include.h"

namespace Modules {
	inline class M_TreeChanger {

		struct TreeModelInfo {
			const char* modelName;
			float scale;
		};

		struct SavedModelInfo : public TreeModelInfo {
			uint64_t meshGroupMask;
		};

		// A hand-picked list of nice tree models
		static inline TreeModelInfo TreeModels[] = {
			{ "models/props_tree/newbloom_tree.vmdl", 1.0f },
			{ "models/props_tree/mango_tree.vmdl", 1.0f },
			{ "maps/journey_assets/props/trees/journey_armandpine/journey_armandpine_02_stump.vmdl", 4.5f },
			{ "models/props_tree/frostivus_tree.vmdl", 0.85f },
			{ "models/props_tree/ti7/ggbranch.vmdl", 1.0f },
		};

		bool needsUpdate = false;
		bool needsRestore = false;

		std::map<CBaseEntity*, SavedModelInfo> originalTrees;

		TreeModelInfo queuedModel;
	public:
		void RestoreTreeModels();
		void UpdateTreeModels();

		void QueueModelUpdate(const TreeModelInfo& mdlInfo) {
			queuedModel = mdlInfo;
			needsUpdate = true;
		}

		void QueueModelUpdate(int modelIndex) {
			QueueModelUpdate(TreeModels[modelIndex]);
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