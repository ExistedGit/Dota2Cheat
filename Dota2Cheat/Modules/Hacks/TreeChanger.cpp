#include "TreeChanger.h"

void Modules::M_TreeChanger::SetTreeModel(CBaseEntity* tree, const TreeModelInfo& mdl) {
	// "models/props_foliage/draft_tree001.vmdl" ends up as the second argument:
	// if (!v14 || !*v14)
	//	 v15 = "models/props_foliage/draft_tree001.vmdl";
	//
	// ...
	// 
	// SetModel(a1, (__int64)v15);

	static Function setMdl = Address(tree->GetVFunc(6)).Offset(0x1b9).GetAbsoluteAddress(1);

	setMdl(tree, mdl.modelName);
	if (tree->ModelScale() != mdl.scale) {
		tree->ModelScale() = mdl.scale;
		tree->GetGameSceneNode()->CallVFunc<10>(4);
	}
}

void Modules::M_TreeChanger::RestoreTreeModels() {

	static void(*skeletonMeshGroupMaskChanged)(CBaseEntity::CModelState * mdl, CBaseEntity * owner, uint64_t * mask)
		= CNetworkMessages::Get()->FindCallback("skeletonMeshGroupMaskChanged");

	if (!skeletonMeshGroupMaskChanged) {
		ONLY_ONCE{
			LogFE("{}: {}!", __FUNCTION__, "skeletonMeshGroupMaskChanged callback not found, cannot restore tree models!");
		}

		return;
	}

	auto trees = CBinaryObjSys::Get()->GetTrees();

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
		auto trees = CBinaryObjSys::Get()->GetTrees();
		bool shouldSaveOriginalTrees = originalTrees.empty();
		// GG branch should be additionally colored via m_clrRender
		bool isGGBranch = queuedModel.modelName == "models/props_tree/ti7/ggbranch.vmdl"sv;
		for (CEnt* tree : trees) {
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
