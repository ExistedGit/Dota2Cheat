#include "TreeChanger.h"

void Modules::M_TreeChanger::SetTreeModel(CBaseEntity* tree, const TreeModelInfo& mdl) {
	static Function setMdl = nullptr;

	if (!setMdl.ptr)
		setMdl = Address(tree->GetVFunc(7).ptr).Offset(0x1c0).GetAbsoluteAddress(1);

	setMdl(tree, mdl.modelName);
	if (tree->ModelScale() != mdl.scale) {
		tree->ModelScale() = mdl.scale;
		tree->Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)->CallVFunc<10>(4);
	}
}

void Modules::M_TreeChanger::RestoreTreeModels() {
	static Function setMeshGroupMask = Memory::Scan("E8 ? ? ? ? 48 8B 07 48 8D 55 7F", "client.dll").GetAbsoluteAddress(1);

	auto trees = GameSystems::BinaryObjectSystem->GetTrees();

	for (auto& [tree, mdlInfo] : originalTrees) {
		if (!IsValidReadPtr(tree))
			continue;

		SetTreeModel(tree, mdlInfo);
		setMeshGroupMask(tree->GetGameSceneNode(), mdlInfo.meshGroupMask);
		tree->SetColor({ 255,255,255,255 });
		if (tree->ModelScale() != mdlInfo.scale) {
			tree->ModelScale() = mdlInfo.scale;
			tree->Member<VClass*>(Netvars::C_BaseEntity::m_pGameSceneNode)->CallVFunc<10>(4);
		}
	}

	originalTrees.clear();
}

void Modules::M_TreeChanger::UpdateTreeModels() {
	if (!needsUpdate && !needsRestore)
		return;

	if (needsUpdate) {
		auto trees = GameSystems::BinaryObjectSystem->GetTrees();
		bool shouldSaveOriginalTrees = originalTrees.empty();
		bool isGGBranch = !strcmp(queuedModel.modelName, "models/props_tree/ti7/ggbranch.vmdl");
		for (auto tree : trees) {
			if (!tree)
				continue;

			if (shouldSaveOriginalTrees)
				originalTrees[tree] = { tree->GetModelName(), tree->ModelScale(), tree->GetGameSceneNode()->GetModelState()->GetMeshGroupMask() };

			SetTreeModel(tree, queuedModel);

			if (isGGBranch)
				tree->SetColor({ 200,165,0,255 });
			else
				tree->SetColor({ 255,255,255,255 });
		}
	}
	else if (needsRestore)
		RestoreTreeModels();

	needsUpdate = false;
	needsRestore = false;
}
