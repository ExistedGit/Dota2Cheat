#include "TreeChanger.h"

void Modules::M_TreeChanger::SetTreeModel(CBaseEntity* tree, const TreeModelInfo& mdl) {
	static Function setMdl = nullptr;

	if (!setMdl.ptr)
		// models/props_foliage/draft_tree001.vmdl goes into its RDX
		setMdl = Address(tree->GetVFunc(7).ptr).Offset(0x1b9).GetAbsoluteAddress(1);

	setMdl(tree, mdl.modelName);
	if (tree->ModelScale() != mdl.scale) {
		tree->ModelScale() = mdl.scale;
		tree->GetGameSceneNode()->CallVFunc<10>(4);
	}
}

void Modules::M_TreeChanger::RestoreTreeModels() {

	static void(*skeletonMeshGroupMaskChanged)(CBaseEntity::CModelState * mdl, CBaseEntity * owner, uint64_t * mask) = nullptr;

	if (!skeletonMeshGroupMaskChanged)
		for (const auto& data : Interfaces::NetworkMessages->GetNetvarCallbacks())
			if (IsValidReadPtr(data.m_szCallbackName) && std::string_view(data.m_szCallbackName) == "skeletonMeshGroupMaskChanged") {
				skeletonMeshGroupMaskChanged = (decltype(skeletonMeshGroupMaskChanged))data.m_CallbackFn;
				break;
			}

	auto trees = GameSystems::BinaryObjectSystem->GetTrees();

	for (auto& [tree, mdlInfo] : originalTrees) {
		if (!IsValidReadPtr(tree))
			continue;

		SetTreeModel(tree, mdlInfo);
		skeletonMeshGroupMaskChanged(tree->GetGameSceneNode()->GetModelState(), tree, &mdlInfo.meshGroupMask);
		tree->SetColor({ 255,255,255,255 });
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
				originalTrees[tree] = { tree->GetModelName(), tree->ModelScale(), tree->GetGameSceneNode()->GetModelState()->MeshGroupMask() };

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
