#include "TreeChanger.h"

void Modules::M_TreeChanger::SetTreeModel(CBaseEntity* tree, const TreeModelInfo& mdl) {
	// "models/props_foliage/draft_tree001.vmdl" goes into its RDX
	static Function setMdl = Address(tree->GetVFunc(5)).Offset(0x1b9).GetAbsoluteAddress(1);

	setMdl(tree, mdl.modelName);
	if (tree->ModelScale() != mdl.scale) {
		tree->ModelScale() = mdl.scale;
		tree->GetGameSceneNode()->CallVFunc<10>(4);
	}
}

void Modules::M_TreeChanger::RestoreTreeModels() {

	static void(*skeletonMeshGroupMaskChanged)(CBaseEntity::CModelState* mdl, CBaseEntity* owner, uint64_t* mask)
		= Interfaces::NetworkMessages->FindCallback("skeletonMeshGroupMaskChanged");

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
	using namespace std::literals::string_view_literals;

	if (needsUpdate) {
		auto trees = GameSystems::BinaryObjectSystem->GetTrees();
		bool shouldSaveOriginalTrees = originalTrees.empty();
		// GG branch should be additionally colored via m_clrRender
		bool isGGBranch = queuedModel.modelName== "models/props_tree/ti7/ggbranch.vmdl"sv;
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

	needsUpdate = needsRestore = false;
}
