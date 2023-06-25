#include "TreeChanger.h"

void Modules::M_TreeChanger::SetTreeModel(CBaseEntity* tree, const TreeModelInfo& mdl) {
	static Function setMdl = Memory::Scan("E8 ? ? ? ? 8B 7D 6F", "client.dll").GetAbsoluteAddress(1);

	if (!setMdl.ptr)
		return;

	setMdl(tree, mdl.modelName);
	if (tree->ModelScale() != mdl.scale) {
		tree->ModelScale() = mdl.scale;
		tree->Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)->CallVFunc<10>(4);
	}
}

void Modules::M_TreeChanger::RestoreTreeModels() {
	static Function setMeshGroupMask = Memory::Scan("E8 ? ? ? ? 48 8B 07 48 8D 55 7F", "client.dll").GetAbsoluteAddress(1);

	auto trees = GameSystems::BinaryObjectSystem->GetTrees();
	for (auto& [idx, mdlInfo] : originalTrees) {
		auto tree = trees[idx];
		if (!tree)
			continue;

		SetTreeModel(tree, mdlInfo);
		setMeshGroupMask(tree->GetGameSceneNode(), mdlInfo.meshGroupMask);
	}

	originalTrees.clear();
}

void Modules::M_TreeChanger::UpdateTreeModels() {
	if (!needsUpdate && !needsRestore)
		return;

	if (needsUpdate) {
		auto trees = GameSystems::BinaryObjectSystem->GetTrees();
		bool shouldSaveOriginalTrees = originalTrees.empty();
		for (int i = 0; i < trees.m_Size; i++) {
			auto tree = trees[i];
			if (!tree)
				continue;

			if (shouldSaveOriginalTrees)
				originalTrees[i] = { tree->GetModelName(), tree->ModelScale(), tree->GetGameSceneNode()->GetModelState()->GetMeshGroupMask() };

			SetTreeModel(tree, queuedModel);
		}
	}
	else if (needsRestore)
		RestoreTreeModels();

	needsUpdate = false;
	needsRestore = false;
}
