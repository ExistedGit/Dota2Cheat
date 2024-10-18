#pragma once
#include <cstdint>
#include <optional>

// Generated at 18-10-2024 18:09:10
// D2C Netvars: existence checked at compile time, offset lazily evaluated

class netvar_t {
	std::string_view dll, name, member;

	std::optional<uint16_t> offset;
public:
	constexpr netvar_t(std::string_view dll, std::string_view name, std::string_view member)
		: dll(dll), name(name), member(member)
	{}

	uint16_t GetOffset();

	operator uint16_t() {
		return GetOffset();
	}
};

namespace Netvars {
	// client.dll
	// CEntityInstance
	namespace CEntityInstance {
		inline netvar_t m_iszPrivateVScripts = /* 0x8, CUtlSymbolLarge */ { "client.dll", "CEntityInstance", "m_iszPrivateVScripts" };
		inline netvar_t m_pEntity = /* 0x10, CEntityIdentity* */ { "client.dll", "CEntityInstance", "m_pEntity" };
		inline netvar_t m_CScriptComponent = /* 0x28, CScriptComponent* */ { "client.dll", "CEntityInstance", "m_CScriptComponent" };
		inline netvar_t m_bVisibleinPVS = /* 0x30, bool */ { "client.dll", "CEntityInstance", "m_bVisibleinPVS" };
	}
	// client.dll
	// C_BaseEntity < CEntityInstance
	namespace C_BaseEntity {
		using namespace CEntityInstance;

		inline netvar_t m_CBodyComponent = /* 0x38, CBodyComponent* */ { "client.dll", "C_BaseEntity", "m_CBodyComponent" };
		inline netvar_t m_NetworkTransmitComponent = /* 0x40, CNetworkTransmitComponent */ { "client.dll", "C_BaseEntity", "m_NetworkTransmitComponent" };
		inline netvar_t m_nLastThinkTick = /* 0x300, GameTick_t */ { "client.dll", "C_BaseEntity", "m_nLastThinkTick" };
		inline netvar_t m_pGameSceneNode = /* 0x308, CGameSceneNode* */ { "client.dll", "C_BaseEntity", "m_pGameSceneNode" };
		inline netvar_t m_pRenderComponent = /* 0x310, CRenderComponent* */ { "client.dll", "C_BaseEntity", "m_pRenderComponent" };
		inline netvar_t m_pCollision = /* 0x318, CCollisionProperty* */ { "client.dll", "C_BaseEntity", "m_pCollision" };
		inline netvar_t m_iMaxHealth = /* 0x320, int32 */ { "client.dll", "C_BaseEntity", "m_iMaxHealth" };
		inline netvar_t m_iHealth = /* 0x324, int32 */ { "client.dll", "C_BaseEntity", "m_iHealth" };
		inline netvar_t m_lifeState = /* 0x328, uint8 */ { "client.dll", "C_BaseEntity", "m_lifeState" };
		inline netvar_t m_takedamage = /* 0x329, DamageOptions_t */ { "client.dll", "C_BaseEntity", "m_takedamage" };
		inline netvar_t m_bTakesDamage = /* 0x32a, bool */ { "client.dll", "C_BaseEntity", "m_bTakesDamage" };
		inline netvar_t m_nTakeDamageFlags = /* 0x330, TakeDamageFlags_t */ { "client.dll", "C_BaseEntity", "m_nTakeDamageFlags" };
		inline netvar_t m_nPlatformType = /* 0x338, EntityPlatformTypes_t */ { "client.dll", "C_BaseEntity", "m_nPlatformType" };
		inline netvar_t m_ubInterpolationFrame = /* 0x339, uint8 */ { "client.dll", "C_BaseEntity", "m_ubInterpolationFrame" };
		inline netvar_t m_hSceneObjectController = /* 0x33c, CHandle< C_BaseEntity > */ { "client.dll", "C_BaseEntity", "m_hSceneObjectController" };
		inline netvar_t m_nNoInterpolationTick = /* 0x340, int32 */ { "client.dll", "C_BaseEntity", "m_nNoInterpolationTick" };
		inline netvar_t m_nVisibilityNoInterpolationTick = /* 0x344, int32 */ { "client.dll", "C_BaseEntity", "m_nVisibilityNoInterpolationTick" };
		inline netvar_t m_flProxyRandomValue = /* 0x348, float32 */ { "client.dll", "C_BaseEntity", "m_flProxyRandomValue" };
		inline netvar_t m_iEFlags = /* 0x34c, int32 */ { "client.dll", "C_BaseEntity", "m_iEFlags" };
		inline netvar_t m_nWaterType = /* 0x350, uint8 */ { "client.dll", "C_BaseEntity", "m_nWaterType" };
		inline netvar_t m_bInterpolateEvenWithNoModel = /* 0x351, bool */ { "client.dll", "C_BaseEntity", "m_bInterpolateEvenWithNoModel" };
		inline netvar_t m_bPredictionEligible = /* 0x352, bool */ { "client.dll", "C_BaseEntity", "m_bPredictionEligible" };
		inline netvar_t m_bApplyLayerMatchIDToModel = /* 0x353, bool */ { "client.dll", "C_BaseEntity", "m_bApplyLayerMatchIDToModel" };
		inline netvar_t m_tokLayerMatchID = /* 0x354, CUtlStringToken */ { "client.dll", "C_BaseEntity", "m_tokLayerMatchID" };
		inline netvar_t m_nSubclassID = /* 0x358, CUtlStringToken */ { "client.dll", "C_BaseEntity", "m_nSubclassID" };
		inline netvar_t m_nSimulationTick = /* 0x368, int32 */ { "client.dll", "C_BaseEntity", "m_nSimulationTick" };
		inline netvar_t m_iCurrentThinkContext = /* 0x36c, int32 */ { "client.dll", "C_BaseEntity", "m_iCurrentThinkContext" };
		inline netvar_t m_aThinkFunctions = /* 0x370, CUtlVector< thinkfunc_t > */ { "client.dll", "C_BaseEntity", "m_aThinkFunctions" };
		inline netvar_t m_bDisabledContextThinks = /* 0x388, bool */ { "client.dll", "C_BaseEntity", "m_bDisabledContextThinks" };
		inline netvar_t m_flAnimTime = /* 0x38c, float32 */ { "client.dll", "C_BaseEntity", "m_flAnimTime" };
		inline netvar_t m_flSimulationTime = /* 0x390, float32 */ { "client.dll", "C_BaseEntity", "m_flSimulationTime" };
		inline netvar_t m_nSceneObjectOverrideFlags = /* 0x394, uint8 */ { "client.dll", "C_BaseEntity", "m_nSceneObjectOverrideFlags" };
		inline netvar_t m_bHasSuccessfullyInterpolated = /* 0x395, bool */ { "client.dll", "C_BaseEntity", "m_bHasSuccessfullyInterpolated" };
		inline netvar_t m_bHasAddedVarsToInterpolation = /* 0x396, bool */ { "client.dll", "C_BaseEntity", "m_bHasAddedVarsToInterpolation" };
		inline netvar_t m_bRenderEvenWhenNotSuccessfullyInterpolated = /* 0x397, bool */ { "client.dll", "C_BaseEntity", "m_bRenderEvenWhenNotSuccessfullyInterpolated" };
		inline netvar_t m_nInterpolationLatchDirtyFlags = /* 0x398, int32[2] */ { "client.dll", "C_BaseEntity", "m_nInterpolationLatchDirtyFlags" };
		inline netvar_t m_ListEntry = /* 0x3a0, uint16[11] */ { "client.dll", "C_BaseEntity", "m_ListEntry" };
		inline netvar_t m_flCreateTime = /* 0x3b8, GameTime_t */ { "client.dll", "C_BaseEntity", "m_flCreateTime" };
		inline netvar_t m_flSpeed = /* 0x3bc, float32 */ { "client.dll", "C_BaseEntity", "m_flSpeed" };
		inline netvar_t m_EntClientFlags = /* 0x3c0, uint16 */ { "client.dll", "C_BaseEntity", "m_EntClientFlags" };
		inline netvar_t m_bClientSideRagdoll = /* 0x3c2, bool */ { "client.dll", "C_BaseEntity", "m_bClientSideRagdoll" };
		inline netvar_t m_iTeamNum = /* 0x3c3, uint8 */ { "client.dll", "C_BaseEntity", "m_iTeamNum" };
		inline netvar_t m_spawnflags = /* 0x3c4, uint32 */ { "client.dll", "C_BaseEntity", "m_spawnflags" };
		inline netvar_t m_nNextThinkTick = /* 0x3c8, GameTick_t */ { "client.dll", "C_BaseEntity", "m_nNextThinkTick" };
		inline netvar_t m_fFlags = /* 0x3cc, uint32 */ { "client.dll", "C_BaseEntity", "m_fFlags" };
		inline netvar_t m_vecAbsVelocity = /* 0x3d0, Vector */ { "client.dll", "C_BaseEntity", "m_vecAbsVelocity" };
		inline netvar_t m_vecVelocity = /* 0x3e0, CNetworkVelocityVector */ { "client.dll", "C_BaseEntity", "m_vecVelocity" };
		inline netvar_t m_hEffectEntity = /* 0x410, CHandle< C_BaseEntity > */ { "client.dll", "C_BaseEntity", "m_hEffectEntity" };
		inline netvar_t m_hOwnerEntity = /* 0x414, CHandle< C_BaseEntity > */ { "client.dll", "C_BaseEntity", "m_hOwnerEntity" };
		inline netvar_t m_MoveCollide = /* 0x418, MoveCollide_t */ { "client.dll", "C_BaseEntity", "m_MoveCollide" };
		inline netvar_t m_MoveType = /* 0x419, MoveType_t */ { "client.dll", "C_BaseEntity", "m_MoveType" };
		inline netvar_t m_nActualMoveType = /* 0x41a, MoveType_t */ { "client.dll", "C_BaseEntity", "m_nActualMoveType" };
		inline netvar_t m_flWaterLevel = /* 0x41c, float32 */ { "client.dll", "C_BaseEntity", "m_flWaterLevel" };
		inline netvar_t m_fEffects = /* 0x420, uint32 */ { "client.dll", "C_BaseEntity", "m_fEffects" };
		inline netvar_t m_hGroundEntity = /* 0x424, CHandle< C_BaseEntity > */ { "client.dll", "C_BaseEntity", "m_hGroundEntity" };
		inline netvar_t m_nGroundBodyIndex = /* 0x428, int32 */ { "client.dll", "C_BaseEntity", "m_nGroundBodyIndex" };
		inline netvar_t m_flFriction = /* 0x42c, float32 */ { "client.dll", "C_BaseEntity", "m_flFriction" };
		inline netvar_t m_flElasticity = /* 0x430, float32 */ { "client.dll", "C_BaseEntity", "m_flElasticity" };
		inline netvar_t m_flGravityScale = /* 0x434, float32 */ { "client.dll", "C_BaseEntity", "m_flGravityScale" };
		inline netvar_t m_flTimeScale = /* 0x438, float32 */ { "client.dll", "C_BaseEntity", "m_flTimeScale" };
		inline netvar_t m_bAnimatedEveryTick = /* 0x43c, bool */ { "client.dll", "C_BaseEntity", "m_bAnimatedEveryTick" };
		inline netvar_t m_flNavIgnoreUntilTime = /* 0x440, GameTime_t */ { "client.dll", "C_BaseEntity", "m_flNavIgnoreUntilTime" };
		inline netvar_t m_hThink = /* 0x444, uint16 */ { "client.dll", "C_BaseEntity", "m_hThink" };
		inline netvar_t m_fBBoxVisFlags = /* 0x450, uint8 */ { "client.dll", "C_BaseEntity", "m_fBBoxVisFlags" };
		inline netvar_t m_bPredictable = /* 0x451, bool */ { "client.dll", "C_BaseEntity", "m_bPredictable" };
		inline netvar_t m_bRenderWithViewModels = /* 0x452, bool */ { "client.dll", "C_BaseEntity", "m_bRenderWithViewModels" };
		inline netvar_t m_nSplitUserPlayerPredictionSlot = /* 0x454, CSplitScreenSlot */ { "client.dll", "C_BaseEntity", "m_nSplitUserPlayerPredictionSlot" };
		inline netvar_t m_nFirstPredictableCommand = /* 0x458, int32 */ { "client.dll", "C_BaseEntity", "m_nFirstPredictableCommand" };
		inline netvar_t m_nLastPredictableCommand = /* 0x45c, int32 */ { "client.dll", "C_BaseEntity", "m_nLastPredictableCommand" };
		inline netvar_t m_hOldMoveParent = /* 0x460, CHandle< C_BaseEntity > */ { "client.dll", "C_BaseEntity", "m_hOldMoveParent" };
		inline netvar_t m_Particles = /* 0x468, CParticleProperty */ { "client.dll", "C_BaseEntity", "m_Particles" };
		inline netvar_t m_vecPredictedScriptFloats = /* 0x490, CUtlVector< float32 > */ { "client.dll", "C_BaseEntity", "m_vecPredictedScriptFloats" };
		inline netvar_t m_vecPredictedScriptFloatIDs = /* 0x4a8, CUtlVector< int32 > */ { "client.dll", "C_BaseEntity", "m_vecPredictedScriptFloatIDs" };
		inline netvar_t m_nNextScriptVarRecordID = /* 0x4d8, int32 */ { "client.dll", "C_BaseEntity", "m_nNextScriptVarRecordID" };
		inline netvar_t m_vecAngVelocity = /* 0x4e8, QAngle */ { "client.dll", "C_BaseEntity", "m_vecAngVelocity" };
		inline netvar_t m_DataChangeEventRef = /* 0x4f4, int32 */ { "client.dll", "C_BaseEntity", "m_DataChangeEventRef" };
		inline netvar_t m_dependencies = /* 0x4f8, CUtlVector< CEntityHandle > */ { "client.dll", "C_BaseEntity", "m_dependencies" };
		inline netvar_t m_nCreationTick = /* 0x510, int32 */ { "client.dll", "C_BaseEntity", "m_nCreationTick" };
		inline netvar_t m_bAnimTimeChanged = /* 0x51d, bool */ { "client.dll", "C_BaseEntity", "m_bAnimTimeChanged" };
		inline netvar_t m_bSimulationTimeChanged = /* 0x51e, bool */ { "client.dll", "C_BaseEntity", "m_bSimulationTimeChanged" };
		inline netvar_t m_sUniqueHammerID = /* 0x528, CUtlString */ { "client.dll", "C_BaseEntity", "m_sUniqueHammerID" };
		inline netvar_t m_nBloodType = /* 0x530, BloodType */ { "client.dll", "C_BaseEntity", "m_nBloodType" };
	}
	// client.dll
	// C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_BaseModelEntity {
		using namespace C_BaseEntity;

		inline netvar_t m_CRenderComponent = /* 0x538, CRenderComponent* */ { "client.dll", "C_BaseModelEntity", "m_CRenderComponent" };
		inline netvar_t m_CHitboxComponent = /* 0x540, CHitboxComponent */ { "client.dll", "C_BaseModelEntity", "m_CHitboxComponent" };
		inline netvar_t m_bInitModelEffects = /* 0x588, bool */ { "client.dll", "C_BaseModelEntity", "m_bInitModelEffects" };
		inline netvar_t m_bIsStaticProp = /* 0x589, bool */ { "client.dll", "C_BaseModelEntity", "m_bIsStaticProp" };
		inline netvar_t m_iViewerID = /* 0x58c, int32 */ { "client.dll", "C_BaseModelEntity", "m_iViewerID" };
		inline netvar_t m_iTeamVisibilityBitmask = /* 0x590, int32 */ { "client.dll", "C_BaseModelEntity", "m_iTeamVisibilityBitmask" };
		inline netvar_t m_nLastAddDecal = /* 0x594, int32 */ { "client.dll", "C_BaseModelEntity", "m_nLastAddDecal" };
		inline netvar_t m_nDecalsAdded = /* 0x598, int32 */ { "client.dll", "C_BaseModelEntity", "m_nDecalsAdded" };
		inline netvar_t m_iOldHealth = /* 0x59c, int32 */ { "client.dll", "C_BaseModelEntity", "m_iOldHealth" };
		inline netvar_t m_nRenderMode = /* 0x5a0, RenderMode_t */ { "client.dll", "C_BaseModelEntity", "m_nRenderMode" };
		inline netvar_t m_bVisibilityDirtyFlag = /* 0x5a1, bool */ { "client.dll", "C_BaseModelEntity", "m_bVisibilityDirtyFlag" };
		inline netvar_t m_nRenderFX = /* 0x5a2, RenderFx_t */ { "client.dll", "C_BaseModelEntity", "m_nRenderFX" };
		inline netvar_t m_bAllowFadeInView = /* 0x5a3, bool */ { "client.dll", "C_BaseModelEntity", "m_bAllowFadeInView" };
		inline netvar_t m_clrRender = /* 0x5c0, Color */ { "client.dll", "C_BaseModelEntity", "m_clrRender" };
		inline netvar_t m_vecRenderAttributes = /* 0x5c8, C_UtlVectorEmbeddedNetworkVar< EntityRenderAttribute_t > */ { "client.dll", "C_BaseModelEntity", "m_vecRenderAttributes" };
		inline netvar_t m_bRenderToCubemaps = /* 0x630, bool */ { "client.dll", "C_BaseModelEntity", "m_bRenderToCubemaps" };
		inline netvar_t m_bNoInterpolate = /* 0x631, bool */ { "client.dll", "C_BaseModelEntity", "m_bNoInterpolate" };
		inline netvar_t m_Collision = /* 0x638, CCollisionProperty */ { "client.dll", "C_BaseModelEntity", "m_Collision" };
		inline netvar_t m_Glow = /* 0x6e8, CGlowProperty */ { "client.dll", "C_BaseModelEntity", "m_Glow" };
		inline netvar_t m_flGlowBackfaceMult = /* 0x740, float32 */ { "client.dll", "C_BaseModelEntity", "m_flGlowBackfaceMult" };
		inline netvar_t m_fadeMinDist = /* 0x744, float32 */ { "client.dll", "C_BaseModelEntity", "m_fadeMinDist" };
		inline netvar_t m_fadeMaxDist = /* 0x748, float32 */ { "client.dll", "C_BaseModelEntity", "m_fadeMaxDist" };
		inline netvar_t m_flFadeScale = /* 0x74c, float32 */ { "client.dll", "C_BaseModelEntity", "m_flFadeScale" };
		inline netvar_t m_flShadowStrength = /* 0x750, float32 */ { "client.dll", "C_BaseModelEntity", "m_flShadowStrength" };
		inline netvar_t m_nObjectCulling = /* 0x754, uint8 */ { "client.dll", "C_BaseModelEntity", "m_nObjectCulling" };
		inline netvar_t m_nAddDecal = /* 0x758, int32 */ { "client.dll", "C_BaseModelEntity", "m_nAddDecal" };
		inline netvar_t m_vDecalPosition = /* 0x75c, Vector */ { "client.dll", "C_BaseModelEntity", "m_vDecalPosition" };
		inline netvar_t m_vDecalForwardAxis = /* 0x768, Vector */ { "client.dll", "C_BaseModelEntity", "m_vDecalForwardAxis" };
		inline netvar_t m_flDecalHealBloodRate = /* 0x774, float32 */ { "client.dll", "C_BaseModelEntity", "m_flDecalHealBloodRate" };
		inline netvar_t m_flDecalHealHeightRate = /* 0x778, float32 */ { "client.dll", "C_BaseModelEntity", "m_flDecalHealHeightRate" };
		inline netvar_t m_ConfigEntitiesToPropagateMaterialDecalsTo = /* 0x780, C_NetworkUtlVectorBase< CHandle< C_BaseModelEntity > > */ { "client.dll", "C_BaseModelEntity", "m_ConfigEntitiesToPropagateMaterialDecalsTo" };
		inline netvar_t m_vecViewOffset = /* 0x798, CNetworkViewOffsetVector */ { "client.dll", "C_BaseModelEntity", "m_vecViewOffset" };
		inline netvar_t m_pClientAlphaProperty = /* 0x7c8, CClientAlphaProperty* */ { "client.dll", "C_BaseModelEntity", "m_pClientAlphaProperty" };
		inline netvar_t m_ClientOverrideTint = /* 0x7d0, Color */ { "client.dll", "C_BaseModelEntity", "m_ClientOverrideTint" };
		inline netvar_t m_bUseClientOverrideTint = /* 0x7d4, bool */ { "client.dll", "C_BaseModelEntity", "m_bUseClientOverrideTint" };
	}
	// client.dll
	// CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace CBaseAnimatingActivity {
		using namespace C_BaseModelEntity;

		inline netvar_t m_bShouldAnimateDuringGameplayPause = /* 0x7d8, bool */ { "client.dll", "CBaseAnimatingActivity", "m_bShouldAnimateDuringGameplayPause" };
		inline netvar_t m_bInitiallyPopulateInterpHistory = /* 0x7d9, bool */ { "client.dll", "CBaseAnimatingActivity", "m_bInitiallyPopulateInterpHistory" };
		inline netvar_t m_bAnimationUpdateScheduled = /* 0x7da, bool */ { "client.dll", "CBaseAnimatingActivity", "m_bAnimationUpdateScheduled" };
		inline netvar_t m_pSuppressedAnimEventTags = /* 0x7e0, CUtlVector< CUtlString >* */ { "client.dll", "CBaseAnimatingActivity", "m_pSuppressedAnimEventTags" };
		inline netvar_t m_bHasAnimatedMaterialAttributes = /* 0x7e8, bool */ { "client.dll", "CBaseAnimatingActivity", "m_bHasAnimatedMaterialAttributes" };
		inline netvar_t m_bSuppressAnimEventSounds = /* 0x7f8, bool */ { "client.dll", "CBaseAnimatingActivity", "m_bSuppressAnimEventSounds" };
	}
	// client.dll
	// C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_BaseAnimatingOverlay {
		using namespace CBaseAnimatingActivity;

	}
	// client.dll
	// C_BaseFlex < C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_BaseFlex {
		using namespace C_BaseAnimatingOverlay;

		inline netvar_t m_flexWeight = /* 0x810, C_NetworkUtlVectorBase< float32 > */ { "client.dll", "C_BaseFlex", "m_flexWeight" };
		inline netvar_t m_vLookTargetPosition = /* 0x828, Vector */ { "client.dll", "C_BaseFlex", "m_vLookTargetPosition" };
		inline netvar_t m_blinktoggle = /* 0x840, bool */ { "client.dll", "C_BaseFlex", "m_blinktoggle" };
		inline netvar_t m_nLastFlexUpdateFrameCount = /* 0x8a0, int32 */ { "client.dll", "C_BaseFlex", "m_nLastFlexUpdateFrameCount" };
		inline netvar_t m_CachedViewTarget = /* 0x8a4, Vector */ { "client.dll", "C_BaseFlex", "m_CachedViewTarget" };
		inline netvar_t m_nNextSceneEventId = /* 0x8b0, SceneEventId_t */ { "client.dll", "C_BaseFlex", "m_nNextSceneEventId" };
		inline netvar_t m_iBlink = /* 0x8b4, int32 */ { "client.dll", "C_BaseFlex", "m_iBlink" };
		inline netvar_t m_blinktime = /* 0x8b8, float32 */ { "client.dll", "C_BaseFlex", "m_blinktime" };
		inline netvar_t m_prevblinktoggle = /* 0x8bc, bool */ { "client.dll", "C_BaseFlex", "m_prevblinktoggle" };
		inline netvar_t m_iMouthAttachment = /* 0x8bd, AttachmentHandle_t */ { "client.dll", "C_BaseFlex", "m_iMouthAttachment" };
		inline netvar_t m_iEyeAttachment = /* 0x8be, AttachmentHandle_t */ { "client.dll", "C_BaseFlex", "m_iEyeAttachment" };
		inline netvar_t m_bResetFlexWeightsOnModelChange = /* 0x8bf, bool */ { "client.dll", "C_BaseFlex", "m_bResetFlexWeightsOnModelChange" };
		inline netvar_t m_nEyeOcclusionRendererBone = /* 0x8d8, int32 */ { "client.dll", "C_BaseFlex", "m_nEyeOcclusionRendererBone" };
		inline netvar_t m_mEyeOcclusionRendererCameraToBoneTransform = /* 0x8dc, matrix3x4_t */ { "client.dll", "C_BaseFlex", "m_mEyeOcclusionRendererCameraToBoneTransform" };
		inline netvar_t m_vEyeOcclusionRendererHalfExtent = /* 0x90c, Vector */ { "client.dll", "C_BaseFlex", "m_vEyeOcclusionRendererHalfExtent" };
		inline netvar_t m_PhonemeClasses = /* 0x928, C_BaseFlex::Emphasized_Phoneme[3] */ { "client.dll", "C_BaseFlex", "m_PhonemeClasses" };
	}
	// client.dll
	// C_BaseCombatCharacter < C_BaseFlex < C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_BaseCombatCharacter {
		using namespace C_BaseFlex;

		inline netvar_t m_hMyWearables = /* 0x988, C_NetworkUtlVectorBase< CHandle< C_EconWearable > > */ { "client.dll", "C_BaseCombatCharacter", "m_hMyWearables" };
		inline netvar_t m_leftFootAttachment = /* 0x9a0, AttachmentHandle_t */ { "client.dll", "C_BaseCombatCharacter", "m_leftFootAttachment" };
		inline netvar_t m_rightFootAttachment = /* 0x9a1, AttachmentHandle_t */ { "client.dll", "C_BaseCombatCharacter", "m_rightFootAttachment" };
		inline netvar_t m_nWaterWakeMode = /* 0x9a4, C_BaseCombatCharacter::WaterWakeMode_t */ { "client.dll", "C_BaseCombatCharacter", "m_nWaterWakeMode" };
		inline netvar_t m_flWaterWorldZ = /* 0x9a8, float32 */ { "client.dll", "C_BaseCombatCharacter", "m_flWaterWorldZ" };
		inline netvar_t m_flWaterNextTraceTime = /* 0x9ac, float32 */ { "client.dll", "C_BaseCombatCharacter", "m_flWaterNextTraceTime" };
	}
	// client.dll
	// C_NextBotCombatCharacter < C_BaseCombatCharacter < C_BaseFlex < C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_NextBotCombatCharacter {
		using namespace C_BaseCombatCharacter;

		inline netvar_t m_shadowTimer = /* 0xa10, CountdownTimer */ { "client.dll", "C_NextBotCombatCharacter", "m_shadowTimer" };
		inline netvar_t m_bInFrustum = /* 0xa28, bool */ { "client.dll", "C_NextBotCombatCharacter", "m_bInFrustum" };
		inline netvar_t m_nInFrustumFrame = /* 0xa2c, int32 */ { "client.dll", "C_NextBotCombatCharacter", "m_nInFrustumFrame" };
		inline netvar_t m_flFrustumDistanceSqr = /* 0xa30, float32 */ { "client.dll", "C_NextBotCombatCharacter", "m_flFrustumDistanceSqr" };
		inline netvar_t m_nLod = /* 0xa34, uint8 */ { "client.dll", "C_NextBotCombatCharacter", "m_nLod" };
	}
	// client.dll
	// C_DOTA_BaseNPC < C_NextBotCombatCharacter < C_BaseCombatCharacter < C_BaseFlex < C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_DOTA_BaseNPC {
		using namespace C_NextBotCombatCharacter;

		inline netvar_t m_bIsPhantom = /* 0xa48, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsPhantom" };
		inline netvar_t m_iUnitType = /* 0xa4c, uint32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iUnitType" };
		inline netvar_t m_bSelectionRingVisible = /* 0xa60, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bSelectionRingVisible" };
		inline netvar_t m_iCurrentLevel = /* 0xa64, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iCurrentLevel" };
		inline netvar_t m_bIsAncient = /* 0xa68, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsAncient" };
		inline netvar_t m_bIsBossCreature = /* 0xa69, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsBossCreature" };
		inline netvar_t m_bStolenScepter = /* 0xa6a, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bStolenScepter" };
		inline netvar_t m_bIsNeutralUnitType = /* 0xa6b, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsNeutralUnitType" };
		inline netvar_t m_bSelectOnSpawn = /* 0xa6c, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bSelectOnSpawn" };
		inline netvar_t m_bCachedReplicatedMorphlingIllusion = /* 0xa6d, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bCachedReplicatedMorphlingIllusion" };
		inline netvar_t m_bIgnoreAddSummonedToSelection = /* 0xa6e, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIgnoreAddSummonedToSelection" };
		inline netvar_t m_bConsideredHero = /* 0xa6f, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bConsideredHero" };
		inline netvar_t m_bUsesConstantGesture = /* 0xa70, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bUsesConstantGesture" };
		inline netvar_t m_bUseHeroAbilityNumbers = /* 0xa71, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bUseHeroAbilityNumbers" };
		inline netvar_t m_bHasSharedAbilities = /* 0xa72, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bHasSharedAbilities" };
		inline netvar_t m_bIsSummoned = /* 0xa73, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsSummoned" };
		inline netvar_t m_bCanBeDominated = /* 0xa74, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bCanBeDominated" };
		inline netvar_t m_bHasUpgradeableAbilities = /* 0xa75, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bHasUpgradeableAbilities" };
		inline netvar_t m_flHealthThinkRegen = /* 0xa78, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flHealthThinkRegen" };
		inline netvar_t m_iIsControllableByPlayer64 = /* 0xa80, uint64 */ { "client.dll", "C_DOTA_BaseNPC", "m_iIsControllableByPlayer64" };
		inline netvar_t m_nHealthBarOffsetOverride = /* 0xa88, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_nHealthBarOffsetOverride" };
		inline netvar_t m_bCanRespawn = /* 0xa8c, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bCanRespawn" };
		inline netvar_t m_iAttackRange = /* 0xa90, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iAttackRange" };
		inline netvar_t m_bIsClone = /* 0xa94, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsClone" };
		inline netvar_t m_colorGemColor = /* 0xa95, Color */ { "client.dll", "C_DOTA_BaseNPC", "m_colorGemColor" };
		inline netvar_t m_bHasColorGem = /* 0xa99, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bHasColorGem" };
		inline netvar_t m_nFXDeniableIndex = /* 0xa9c, ParticleIndex_t */ { "client.dll", "C_DOTA_BaseNPC", "m_nFXDeniableIndex" };
		inline netvar_t m_iMoveSpeed = /* 0xaa0, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iMoveSpeed" };
		inline netvar_t m_iBaseAttackSpeed = /* 0xaa4, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iBaseAttackSpeed" };
		inline netvar_t m_flBaseAttackTime = /* 0xaa8, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flBaseAttackTime" };
		inline netvar_t m_iUnitNameIndex = /* 0xaac, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iUnitNameIndex" };
		inline netvar_t m_iHealthBarOffset = /* 0xab0, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iHealthBarOffset" };
		inline netvar_t m_iHealthBarHighlightColor = /* 0xab4, Color */ { "client.dll", "C_DOTA_BaseNPC", "m_iHealthBarHighlightColor" };
		inline netvar_t m_flMana = /* 0xab8, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flMana" };
		inline netvar_t m_flMaxMana = /* 0xabc, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flMaxMana" };
		inline netvar_t m_flManaThinkRegen = /* 0xac0, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flManaThinkRegen" };
		inline netvar_t m_iBKBChargesUsed = /* 0xac4, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iBKBChargesUsed" };
		inline netvar_t m_iAeonChargesUsed = /* 0xac8, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iAeonChargesUsed" };
		inline netvar_t m_flRefresherUseTime = /* 0xacc, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC", "m_flRefresherUseTime" };
		inline netvar_t m_flRefresherLastCooldown = /* 0xad0, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flRefresherLastCooldown" };
		inline netvar_t m_flLastDealtDamageTime = /* 0xad4, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC", "m_flLastDealtDamageTime" };
		inline netvar_t m_iBotDebugData = /* 0xad8, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iBotDebugData" };
		inline netvar_t m_bIsIllusion = /* 0xadc, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsIllusion" };
		inline netvar_t m_bHasClientSeenIllusionModifier = /* 0xadd, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bHasClientSeenIllusionModifier" };
		inline netvar_t m_hAbilities = /* 0xae0, CHandle< C_BaseEntity >[35] */ { "client.dll", "C_DOTA_BaseNPC", "m_hAbilities" };
		inline netvar_t m_flInvisibilityLevel = /* 0xb6c, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flInvisibilityLevel" };
		inline netvar_t m_flHullRadius = /* 0xb70, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flHullRadius" };
		inline netvar_t m_flCollisionPadding = /* 0xb74, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flCollisionPadding" };
		inline netvar_t m_flRingRadius = /* 0xb78, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flRingRadius" };
		inline netvar_t m_flProjectileCollisionSize = /* 0xb7c, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flProjectileCollisionSize" };
		inline netvar_t m_iszUnitName = /* 0xb80, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszUnitName" };
		inline netvar_t m_iszUnitNameIconOverride = /* 0xb88, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszUnitNameIconOverride" };
		inline netvar_t m_iszParticleFolder = /* 0xb90, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszParticleFolder" };
		inline netvar_t m_iszSoundSet = /* 0xb98, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszSoundSet" };
		inline netvar_t m_iszSelectionGroup = /* 0xba0, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszSelectionGroup" };
		inline netvar_t m_iszVoiceFile = /* 0xba8, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszVoiceFile" };
		inline netvar_t m_iszGameSoundsFile = /* 0xbb0, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszGameSoundsFile" };
		inline netvar_t m_iszVoiceBackgroundSound = /* 0xbb8, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszVoiceBackgroundSound" };
		inline netvar_t m_iszIdleSoundLoop = /* 0xbc0, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszIdleSoundLoop" };
		inline netvar_t m_szUnitLabel = /* 0xbc8, CUtlString */ { "client.dll", "C_DOTA_BaseNPC", "m_szUnitLabel" };
		inline netvar_t m_szScaleset = /* 0xbd0, CUtlString */ { "client.dll", "C_DOTA_BaseNPC", "m_szScaleset" };
		inline netvar_t m_nUnitLabelIndex = /* 0xbd8, uint8 */ { "client.dll", "C_DOTA_BaseNPC", "m_nUnitLabelIndex" };
		inline netvar_t m_strAnimationModifier = /* 0xbe0, CUtlString */ { "client.dll", "C_DOTA_BaseNPC", "m_strAnimationModifier" };
		inline netvar_t m_TerrainSpecificFootstepEffect = /* 0xbe8, CUtlString */ { "client.dll", "C_DOTA_BaseNPC", "m_TerrainSpecificFootstepEffect" };
		inline netvar_t m_bUseCustomTerrainWeatherEffect = /* 0xbf0, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bUseCustomTerrainWeatherEffect" };
		inline netvar_t m_bHasClientSoundReplacement = /* 0xbf1, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bHasClientSoundReplacement" };
		inline netvar_t m_bHasClientReplacementParticle = /* 0xbf2, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bHasClientReplacementParticle" };
		inline netvar_t m_bResourcesLoaded = /* 0xbfc, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bResourcesLoaded" };
		inline netvar_t m_flTauntCooldown = /* 0xc00, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC", "m_flTauntCooldown" };
		inline netvar_t m_flTauntCooldown2 = /* 0xc04, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC", "m_flTauntCooldown2" };
		inline netvar_t m_iCurShop = /* 0xc08, DOTA_SHOP_TYPE */ { "client.dll", "C_DOTA_BaseNPC", "m_iCurShop" };
		inline netvar_t m_szCurShopEntName = /* 0xc10, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_szCurShopEntName" };
		inline netvar_t m_iDayTimeVisionRange = /* 0xc18, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iDayTimeVisionRange" };
		inline netvar_t m_iNightTimeVisionRange = /* 0xc1c, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iNightTimeVisionRange" };
		inline netvar_t m_iDamageMin = /* 0xc20, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iDamageMin" };
		inline netvar_t m_iDamageMax = /* 0xc24, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iDamageMax" };
		inline netvar_t m_iDamageBonus = /* 0xc28, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iDamageBonus" };
		inline netvar_t m_iBaseDamageMin = /* 0xc2c, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iBaseDamageMin" };
		inline netvar_t m_iBaseDamageMax = /* 0xc30, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iBaseDamageMax" };
		inline netvar_t m_iTaggedAsVisibleByTeam = /* 0xc34, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iTaggedAsVisibleByTeam" };
		inline netvar_t m_ModifierManager = /* 0xc38, CDOTA_ModifierManager */ { "client.dll", "C_DOTA_BaseNPC", "m_ModifierManager" };
		inline netvar_t m_Inventory = /* 0xf58, C_DOTA_UnitInventory */ { "client.dll", "C_DOTA_BaseNPC", "m_Inventory" };
		inline netvar_t m_nUnitState64 = /* 0x1030, uint64 */ { "client.dll", "C_DOTA_BaseNPC", "m_nUnitState64" };
		inline netvar_t m_nUnitDebuffState = /* 0x1040, uint64 */ { "client.dll", "C_DOTA_BaseNPC", "m_nUnitDebuffState" };
		inline netvar_t m_bHasInventory = /* 0x1048, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bHasInventory" };
		inline netvar_t m_iAcquisitionRange = /* 0x104c, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iAcquisitionRange" };
		inline netvar_t m_FoWViewID = /* 0x1050, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_FoWViewID" };
		inline netvar_t m_iPrevHealthPct = /* 0x1054, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iPrevHealthPct" };
		inline netvar_t m_iPrevLifeState = /* 0x1058, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iPrevLifeState" };
		inline netvar_t m_iPrevTeam = /* 0x105c, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iPrevTeam" };
		inline netvar_t m_bPrevProvidesVision = /* 0x1060, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bPrevProvidesVision" };
		inline netvar_t m_nPrevControllableMask = /* 0x1068, uint64 */ { "client.dll", "C_DOTA_BaseNPC", "m_nPrevControllableMask" };
		inline netvar_t m_TagTime = /* 0x1078, CountdownTimer */ { "client.dll", "C_DOTA_BaseNPC", "m_TagTime" };
		inline netvar_t m_ClickedTime = /* 0x1090, CountdownTimer */ { "client.dll", "C_DOTA_BaseNPC", "m_ClickedTime" };
		inline netvar_t m_flIdleRunPoseStartTime = /* 0x10a8, GameTime_t[2] */ { "client.dll", "C_DOTA_BaseNPC", "m_flIdleRunPoseStartTime" };
		inline netvar_t m_flIdleRunPoseDuration = /* 0x10b0, float32[2] */ { "client.dll", "C_DOTA_BaseNPC", "m_flIdleRunPoseDuration" };
		inline netvar_t m_bAnimationTransitionActive = /* 0x10b8, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bAnimationTransitionActive" };
		inline netvar_t m_nAnimationTransitionPoseParameters = /* 0x10bc, int32[2] */ { "client.dll", "C_DOTA_BaseNPC", "m_nAnimationTransitionPoseParameters" };
		inline netvar_t m_nAnimationTransitionInjuredPoseParameters = /* 0x10c4, int32[2] */ { "client.dll", "C_DOTA_BaseNPC", "m_nAnimationTransitionInjuredPoseParameters" };
		inline netvar_t m_nAnimationTransitionAggressivePoseParameters = /* 0x10cc, int32[2] */ { "client.dll", "C_DOTA_BaseNPC", "m_nAnimationTransitionAggressivePoseParameters" };
		inline netvar_t m_flTimeSinceLastAbilityNag = /* 0x1130, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC", "m_flTimeSinceLastAbilityNag" };
		inline netvar_t m_iAttackCapabilities = /* 0x1134, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iAttackCapabilities" };
		inline netvar_t m_iSpecialAbility = /* 0x1138, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iSpecialAbility" };
		inline netvar_t m_iMoveCapabilities = /* 0x113c, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iMoveCapabilities" };
		inline netvar_t m_nPlayerOwnerID = /* 0x1140, PlayerID_t */ { "client.dll", "C_DOTA_BaseNPC", "m_nPlayerOwnerID" };
		inline netvar_t m_flLastDamageTime = /* 0x1144, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC", "m_flLastDamageTime" };
		inline netvar_t m_flLastAttackTime = /* 0x1148, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC", "m_flLastAttackTime" };
		inline netvar_t m_iszMinimapIcon = /* 0x1150, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_iszMinimapIcon" };
		inline netvar_t m_flMinimapIconSize = /* 0x1160, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flMinimapIconSize" };
		inline netvar_t m_bMinimapDisableTint = /* 0x1164, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bMinimapDisableTint" };
		inline netvar_t m_bMinimapDisableRotation = /* 0x1165, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bMinimapDisableRotation" };
		inline netvar_t m_colorHeroGlow = /* 0x1166, Color */ { "client.dll", "C_DOTA_BaseNPC", "m_colorHeroGlow" };
		inline netvar_t m_iNearShopMask = /* 0x116c, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iNearShopMask" };
		inline netvar_t m_nPoseParameterTurn = /* 0x1170, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_nPoseParameterTurn" };
		inline netvar_t m_nPoseParameterRun = /* 0x1174, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_nPoseParameterRun" };
		inline netvar_t m_flLean = /* 0x1178, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flLean" };
		inline netvar_t m_flRun = /* 0x117c, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flRun" };
		inline netvar_t m_anglediff = /* 0x1180, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_anglediff" };
		inline netvar_t m_bInfoKeyActive = /* 0x1184, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bInfoKeyActive" };
		inline netvar_t m_bNewUpdateAssetModifiersNetworked = /* 0x1185, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bNewUpdateAssetModifiersNetworked" };
		inline netvar_t m_nAssetModifiersLastUpdatedTick = /* 0x1188, GameTick_t */ { "client.dll", "C_DOTA_BaseNPC", "m_nAssetModifiersLastUpdatedTick" };
		inline netvar_t m_bSuppressGlow = /* 0x118c, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bSuppressGlow" };
		inline netvar_t m_bWasSinking = /* 0x118d, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bWasSinking" };
		inline netvar_t m_flRangeDisplayDist = /* 0x1190, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flRangeDisplayDist" };
		inline netvar_t m_szDefaultIdle = /* 0x1378, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_szDefaultIdle" };
		inline netvar_t m_szDefaultRareIdle = /* 0x1380, CUtlSymbolLarge */ { "client.dll", "C_DOTA_BaseNPC", "m_szDefaultRareIdle" };
		inline netvar_t m_damagetimer = /* 0x1388, CountdownTimer */ { "client.dll", "C_DOTA_BaseNPC", "m_damagetimer" };
		inline netvar_t m_vRenderOrigin = /* 0x13a0, Vector */ { "client.dll", "C_DOTA_BaseNPC", "m_vRenderOrigin" };
		inline netvar_t m_fZDelta = /* 0x13ac, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_fZDelta" };
		inline netvar_t m_flDeathTime = /* 0x13b0, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC", "m_flDeathTime" };
		inline netvar_t m_bBaseStatsChanged = /* 0x13b4, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bBaseStatsChanged" };
		inline netvar_t m_bNeedsSoundEmitterRefresh = /* 0x13b5, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bNeedsSoundEmitterRefresh" };
		inline netvar_t m_flPhysicalArmorValue = /* 0x13b8, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flPhysicalArmorValue" };
		inline netvar_t m_flMagicalResistanceValue = /* 0x13bc, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flMagicalResistanceValue" };
		inline netvar_t m_nPrevSequenceParity = /* 0x13c0, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_nPrevSequenceParity" };
		inline netvar_t m_flPrevInvisLevel = /* 0x1468, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flPrevInvisLevel" };
		inline netvar_t m_nOriginalModelIndex = /* 0x1470, CStrongHandle< InfoForResourceTypeCModel > */ { "client.dll", "C_DOTA_BaseNPC", "m_nOriginalModelIndex" };
		inline netvar_t m_nClientOriginalModelIndex = /* 0x1478, CStrongHandle< InfoForResourceTypeCModel > */ { "client.dll", "C_DOTA_BaseNPC", "m_nClientOriginalModelIndex" };
		inline netvar_t m_nClientReplacementModelIndex = /* 0x1480, CStrongHandle< InfoForResourceTypeCModel > */ { "client.dll", "C_DOTA_BaseNPC", "m_nClientReplacementModelIndex" };
		inline netvar_t m_bClientReplacementModelNeedsRebuild = /* 0x1488, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bClientReplacementModelNeedsRebuild" };
		inline netvar_t m_nClientReplacementModelReloadCount = /* 0x148c, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_nClientReplacementModelReloadCount" };
		inline netvar_t m_nUnitModelVariant = /* 0x1490, int8 */ { "client.dll", "C_DOTA_BaseNPC", "m_nUnitModelVariant" };
		inline netvar_t m_nUnitModelVariantCount = /* 0x1491, int8 */ { "client.dll", "C_DOTA_BaseNPC", "m_nUnitModelVariantCount" };
		inline netvar_t m_iPrevSequence = /* 0x1494, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iPrevSequence" };
		inline netvar_t m_pLastWeatherEffectName = /* 0x1498, char* */ { "client.dll", "C_DOTA_BaseNPC", "m_pLastWeatherEffectName" };
		inline netvar_t m_VoiceBackgroundSoundTimer = /* 0x14a8, CountdownTimer */ { "client.dll", "C_DOTA_BaseNPC", "m_VoiceBackgroundSoundTimer" };
		inline netvar_t m_bIsWaitingToSpawn = /* 0x14c0, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsWaitingToSpawn" };
		inline netvar_t m_nTotalDamageTaken = /* 0x14c8, int64 */ { "client.dll", "C_DOTA_BaseNPC", "m_nTotalDamageTaken" };
		inline netvar_t m_flManaRegen = /* 0x14d0, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flManaRegen" };
		inline netvar_t m_flHealthRegen = /* 0x14d4, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flHealthRegen" };
		inline netvar_t m_bIsMoving = /* 0x14d8, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsMoving" };
		inline netvar_t m_fRevealRadius = /* 0x14dc, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_fRevealRadius" };
		inline netvar_t m_bCanUseWards = /* 0x14e0, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bCanUseWards" };
		inline netvar_t m_bCanUseAllItems = /* 0x14e1, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bCanUseAllItems" };
		inline netvar_t m_iXPBounty = /* 0x14e4, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iXPBounty" };
		inline netvar_t m_iXPBountyExtra = /* 0x14e8, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iXPBountyExtra" };
		inline netvar_t m_iGoldBountyMin = /* 0x14ec, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iGoldBountyMin" };
		inline netvar_t m_iGoldBountyMax = /* 0x14f0, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_iGoldBountyMax" };
		inline netvar_t m_hOwnerNPC = /* 0x14f4, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTA_BaseNPC", "m_hOwnerNPC" };
		inline netvar_t m_hGoalEntity = /* 0x14f8, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTA_BaseNPC", "m_hGoalEntity" };
		inline netvar_t m_hNeutralSpawner = /* 0x14fc, CHandle< C_DOTA_NeutralSpawner > */ { "client.dll", "C_DOTA_BaseNPC", "m_hNeutralSpawner" };
		inline netvar_t m_hClientOverrideMaterial = /* 0x1500, CStrongHandle< InfoForResourceTypeIMaterial2 > */ { "client.dll", "C_DOTA_BaseNPC", "m_hClientOverrideMaterial" };
		inline netvar_t m_bCombinerMaterialOverrideListChanged = /* 0x1508, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bCombinerMaterialOverrideListChanged" };
		inline netvar_t m_nBaseModelMeshCount = /* 0x150c, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_nBaseModelMeshCount" };
		inline netvar_t m_combinerMaterialOverrideList = /* 0x1510, CUtlVector< CStrongHandleCopyable< InfoForResourceTypeIMaterial2 > > */ { "client.dll", "C_DOTA_BaseNPC", "m_combinerMaterialOverrideList" };
		inline netvar_t m_nArcanaLevel = /* 0x1528, int8 */ { "client.dll", "C_DOTA_BaseNPC", "m_nArcanaLevel" };
		inline netvar_t m_nDefaultArcanaLevel = /* 0x1529, int8 */ { "client.dll", "C_DOTA_BaseNPC", "m_nDefaultArcanaLevel" };
		inline netvar_t m_defaultColorGemColor = /* 0x152a, Color */ { "client.dll", "C_DOTA_BaseNPC", "m_defaultColorGemColor" };
		inline netvar_t m_bHasBuiltWearableSpawnList = /* 0x1548, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bHasBuiltWearableSpawnList" };
		inline netvar_t m_bHasCostume = /* 0x1549, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bHasCostume" };
		inline netvar_t m_bRecreateParticleSystemsOnModifiersChange = /* 0x154a, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bRecreateParticleSystemsOnModifiersChange" };
		inline netvar_t m_flAdditionalSinkDelay = /* 0x1564, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flAdditionalSinkDelay" };
		inline netvar_t m_NetworkActivity = /* 0x1648, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_NetworkActivity" };
		inline netvar_t m_PrevNetworkActivity = /* 0x164c, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_PrevNetworkActivity" };
		inline netvar_t m_NetworkSequenceIndex = /* 0x1650, int32 */ { "client.dll", "C_DOTA_BaseNPC", "m_NetworkSequenceIndex" };
		inline netvar_t m_bShouldDoFlyHeightVisual = /* 0x1654, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bShouldDoFlyHeightVisual" };
		inline netvar_t m_flStartSequenceCycle = /* 0x1658, float32 */ { "client.dll", "C_DOTA_BaseNPC", "m_flStartSequenceCycle" };
		inline netvar_t m_ActivityModifiers = /* 0x1660, CUtlVector< CUtlSymbol > */ { "client.dll", "C_DOTA_BaseNPC", "m_ActivityModifiers" };
		inline netvar_t m_hBackgroundSceneEnt = /* 0x1678, CHandle< C_SceneEntity > */ { "client.dll", "C_DOTA_BaseNPC", "m_hBackgroundSceneEnt" };
		inline netvar_t m_hSpeakingSceneEnt = /* 0x167c, CHandle< C_SceneEntity > */ { "client.dll", "C_DOTA_BaseNPC", "m_hSpeakingSceneEnt" };
		inline netvar_t m_hOldWearables = /* 0x1690, CUtlVector< CHandle< C_EconWearable > > */ { "client.dll", "C_DOTA_BaseNPC", "m_hOldWearables" };
		inline netvar_t m_hOldWearableSkins = /* 0x16a8, CUtlVector< CUtlStringToken > */ { "client.dll", "C_DOTA_BaseNPC", "m_hOldWearableSkins" };
		inline netvar_t m_CustomHealthLabel = /* 0x16c0, char[256] */ { "client.dll", "C_DOTA_BaseNPC", "m_CustomHealthLabel" };
		inline netvar_t m_CustomHealthLabelColor = /* 0x17c0, Color */ { "client.dll", "C_DOTA_BaseNPC", "m_CustomHealthLabelColor" };
		inline netvar_t m_nWearableDefIndex = /* 0x17c4, item_definition_index_t */ { "client.dll", "C_DOTA_BaseNPC", "m_nWearableDefIndex" };
		inline netvar_t m_gibTintColor = /* 0x17c9, Color */ { "client.dll", "C_DOTA_BaseNPC", "m_gibTintColor" };
		inline netvar_t m_bForceMaterialCombine = /* 0x17cd, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bForceMaterialCombine" };
		inline netvar_t m_bShouldDrawParticlesWhileHidden = /* 0x17ce, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bShouldDrawParticlesWhileHidden" };
		inline netvar_t m_bIsClientThinkPending = /* 0x17cf, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bIsClientThinkPending" };
		inline netvar_t m_bActivityModifiersDirty = /* 0x17d0, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bActivityModifiersDirty" };
		inline netvar_t m_bUnitModelVariantChanged = /* 0x17d1, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bUnitModelVariantChanged" };
		inline netvar_t m_bWearablesAreTaunting = /* 0x17d2, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bWearablesAreTaunting" };
		inline netvar_t m_bShowCannotBeDisabledIcon = /* 0x17e8, bool */ { "client.dll", "C_DOTA_BaseNPC", "m_bShowCannotBeDisabledIcon" };
	}
	// client.dll
	// C_DOTA_BaseNPC_Additive < C_DOTA_BaseNPC < C_NextBotCombatCharacter < C_BaseCombatCharacter < C_BaseFlex < C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_DOTA_BaseNPC_Additive {
		using namespace C_DOTA_BaseNPC;

	}
	// client.dll
	// CEntityIdentity
	namespace CEntityIdentity {
		inline netvar_t m_nameStringableIndex = /* 0x14, int32 */ { "client.dll", "CEntityIdentity", "m_nameStringableIndex" };
		inline netvar_t m_name = /* 0x18, CUtlSymbolLarge */ { "client.dll", "CEntityIdentity", "m_name" };
		inline netvar_t m_designerName = /* 0x20, CUtlSymbolLarge */ { "client.dll", "CEntityIdentity", "m_designerName" };
		inline netvar_t m_flags = /* 0x30, uint32 */ { "client.dll", "CEntityIdentity", "m_flags" };
		inline netvar_t m_worldGroupId = /* 0x38, WorldGroupId_t */ { "client.dll", "CEntityIdentity", "m_worldGroupId" };
		inline netvar_t m_fDataObjectTypes = /* 0x3c, uint32 */ { "client.dll", "CEntityIdentity", "m_fDataObjectTypes" };
		inline netvar_t m_PathIndex = /* 0x40, ChangeAccessorFieldPathIndex_t */ { "client.dll", "CEntityIdentity", "m_PathIndex" };
		inline netvar_t m_pPrev = /* 0x58, CEntityIdentity* */ { "client.dll", "CEntityIdentity", "m_pPrev" };
		inline netvar_t m_pNext = /* 0x60, CEntityIdentity* */ { "client.dll", "CEntityIdentity", "m_pNext" };
		inline netvar_t m_pPrevByClass = /* 0x68, CEntityIdentity* */ { "client.dll", "CEntityIdentity", "m_pPrevByClass" };
		inline netvar_t m_pNextByClass = /* 0x70, CEntityIdentity* */ { "client.dll", "CEntityIdentity", "m_pNextByClass" };
	}
	// client.dll
	// CBasePlayerController < C_BaseEntity < CEntityInstance
	namespace CBasePlayerController {
		using namespace C_BaseEntity;

		inline netvar_t m_nFinalPredictedTick = /* 0x540, int32 */ { "client.dll", "CBasePlayerController", "m_nFinalPredictedTick" };
		inline netvar_t m_CommandContext = /* 0x548, C_CommandContext */ { "client.dll", "CBasePlayerController", "m_CommandContext" };
		inline netvar_t m_nInButtonsWhichAreToggles = /* 0x5e8, uint64 */ { "client.dll", "CBasePlayerController", "m_nInButtonsWhichAreToggles" };
		inline netvar_t m_nTickBase = /* 0x5f0, uint32 */ { "client.dll", "CBasePlayerController", "m_nTickBase" };
		inline netvar_t m_hPawn = /* 0x5f4, CHandle< C_BasePlayerPawn > */ { "client.dll", "CBasePlayerController", "m_hPawn" };
		inline netvar_t m_bKnownTeamMismatch = /* 0x5f8, bool */ { "client.dll", "CBasePlayerController", "m_bKnownTeamMismatch" };
		inline netvar_t m_hPredictedPawn = /* 0x5fc, CHandle< C_BasePlayerPawn > */ { "client.dll", "CBasePlayerController", "m_hPredictedPawn" };
		inline netvar_t m_nSplitScreenSlot = /* 0x600, CSplitScreenSlot */ { "client.dll", "CBasePlayerController", "m_nSplitScreenSlot" };
		inline netvar_t m_hSplitOwner = /* 0x604, CHandle< CBasePlayerController > */ { "client.dll", "CBasePlayerController", "m_hSplitOwner" };
		inline netvar_t m_hSplitScreenPlayers = /* 0x608, CUtlVector< CHandle< CBasePlayerController > > */ { "client.dll", "CBasePlayerController", "m_hSplitScreenPlayers" };
		inline netvar_t m_bIsHLTV = /* 0x620, bool */ { "client.dll", "CBasePlayerController", "m_bIsHLTV" };
		inline netvar_t m_iConnected = /* 0x624, PlayerConnectedState */ { "client.dll", "CBasePlayerController", "m_iConnected" };
		inline netvar_t m_iszPlayerName = /* 0x628, char[128] */ { "client.dll", "CBasePlayerController", "m_iszPlayerName" };
		inline netvar_t m_steamID = /* 0x6b0, uint64 */ { "client.dll", "CBasePlayerController", "m_steamID" };
		inline netvar_t m_bIsLocalPlayerController = /* 0x6b8, bool */ { "client.dll", "CBasePlayerController", "m_bIsLocalPlayerController" };
		inline netvar_t m_iDesiredFOV = /* 0x6bc, uint32 */ { "client.dll", "CBasePlayerController", "m_iDesiredFOV" };
	}
	// client.dll
	// C_EconEntity < C_BaseFlex < C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_EconEntity {
		using namespace C_BaseFlex;

		inline netvar_t m_AttributeManager = /* 0x998, CAttributeContainer */ { "client.dll", "C_EconEntity", "m_AttributeManager" };
		inline netvar_t m_bClientside = /* 0xac0, bool */ { "client.dll", "C_EconEntity", "m_bClientside" };
		inline netvar_t m_nDisableMode = /* 0xac4, EconEntityParticleDisableMode_t */ { "client.dll", "C_EconEntity", "m_nDisableMode" };
		inline netvar_t m_bParticleSystemsCreated = /* 0xac8, bool */ { "client.dll", "C_EconEntity", "m_bParticleSystemsCreated" };
		inline netvar_t m_bForceDestroyAttachedParticlesImmediately = /* 0xac9, bool */ { "client.dll", "C_EconEntity", "m_bForceDestroyAttachedParticlesImmediately" };
		inline netvar_t m_vecAttachedParticles = /* 0xad0, CUtlVector< C_EconEntity::AttachedParticleInfo_t > */ { "client.dll", "C_EconEntity", "m_vecAttachedParticles" };
		inline netvar_t m_hViewmodelAttachment = /* 0xae8, CHandle< CBaseAnimatingActivity > */ { "client.dll", "C_EconEntity", "m_hViewmodelAttachment" };
		inline netvar_t m_iOldTeam = /* 0xaec, int32 */ { "client.dll", "C_EconEntity", "m_iOldTeam" };
		inline netvar_t m_bAttachmentDirty = /* 0xaf0, bool */ { "client.dll", "C_EconEntity", "m_bAttachmentDirty" };
		inline netvar_t m_iOldStyle = /* 0xaf1, style_index_t */ { "client.dll", "C_EconEntity", "m_iOldStyle" };
		inline netvar_t m_hOldProvidee = /* 0xaf4, CHandle< C_BaseEntity > */ { "client.dll", "C_EconEntity", "m_hOldProvidee" };
		inline netvar_t m_vecAttachedModels = /* 0xaf8, CUtlVector< C_EconEntity::AttachedModelData_t > */ { "client.dll", "C_EconEntity", "m_vecAttachedModels" };
	}
	// client.dll
	// C_DOTABaseAbility < C_BaseEntity < CEntityInstance
	namespace C_DOTABaseAbility {
		using namespace C_BaseEntity;

		inline netvar_t m_bAuxCastState = /* 0x540, bool */ { "client.dll", "C_DOTABaseAbility", "m_bAuxCastState" };
		inline netvar_t m_iEnemyLevel = /* 0x544, int32 */ { "client.dll", "C_DOTABaseAbility", "m_iEnemyLevel" };
		inline netvar_t m_iMaxLevel = /* 0x548, int32 */ { "client.dll", "C_DOTABaseAbility", "m_iMaxLevel" };
		inline netvar_t m_iRequiredLevel = /* 0x54c, int32 */ { "client.dll", "C_DOTABaseAbility", "m_iRequiredLevel" };
		inline netvar_t m_iLevelsBetweenUpgrades = /* 0x550, int32 */ { "client.dll", "C_DOTABaseAbility", "m_iLevelsBetweenUpgrades" };
		inline netvar_t m_bCanLearn = /* 0x554, bool */ { "client.dll", "C_DOTABaseAbility", "m_bCanLearn" };
		inline netvar_t m_flUpgradeBlend = /* 0x558, float32 */ { "client.dll", "C_DOTABaseAbility", "m_flUpgradeBlend" };
		inline netvar_t m_bUpgradedDuringQuickcastPreview = /* 0x55c, bool */ { "client.dll", "C_DOTABaseAbility", "m_bUpgradedDuringQuickcastPreview" };
		inline netvar_t m_bConsiderOvershootInGetCastRange = /* 0x55d, bool */ { "client.dll", "C_DOTABaseAbility", "m_bConsiderOvershootInGetCastRange" };
		inline netvar_t m_bRefCountsModifiers = /* 0x55e, bool */ { "client.dll", "C_DOTABaseAbility", "m_bRefCountsModifiers" };
		inline netvar_t m_bHidden = /* 0x55f, bool */ { "client.dll", "C_DOTABaseAbility", "m_bHidden" };
		inline netvar_t m_bOldHidden = /* 0x560, bool */ { "client.dll", "C_DOTABaseAbility", "m_bOldHidden" };
		inline netvar_t m_bActivated = /* 0x561, bool */ { "client.dll", "C_DOTABaseAbility", "m_bActivated" };
		inline netvar_t m_bOldActivated = /* 0x562, bool */ { "client.dll", "C_DOTABaseAbility", "m_bOldActivated" };
		inline netvar_t m_nAbilityBarType = /* 0x564, AbilityBarType_t */ { "client.dll", "C_DOTABaseAbility", "m_nAbilityBarType" };
		inline netvar_t m_iDirtyButtons = /* 0x568, int32 */ { "client.dll", "C_DOTABaseAbility", "m_iDirtyButtons" };
		inline netvar_t m_bPerformDirtyParity = /* 0x56c, bool */ { "client.dll", "C_DOTABaseAbility", "m_bPerformDirtyParity" };
		inline netvar_t m_iLevel = /* 0x570, int32 */ { "client.dll", "C_DOTABaseAbility", "m_iLevel" };
		inline netvar_t m_bAbilityLevelDirty = /* 0x574, bool */ { "client.dll", "C_DOTABaseAbility", "m_bAbilityLevelDirty" };
		inline netvar_t m_bToggleState = /* 0x575, bool */ { "client.dll", "C_DOTABaseAbility", "m_bToggleState" };
		inline netvar_t m_flAbilityMuteDuration = /* 0x578, float32 */ { "client.dll", "C_DOTABaseAbility", "m_flAbilityMuteDuration" };
		inline netvar_t m_bInAbilityPhase = /* 0x57c, bool */ { "client.dll", "C_DOTABaseAbility", "m_bInAbilityPhase" };
		inline netvar_t m_fCooldown = /* 0x580, float32 */ { "client.dll", "C_DOTABaseAbility", "m_fCooldown" };
		inline netvar_t m_flCooldownLength = /* 0x584, float32 */ { "client.dll", "C_DOTABaseAbility", "m_flCooldownLength" };
		inline netvar_t m_iManaCost = /* 0x588, int32 */ { "client.dll", "C_DOTABaseAbility", "m_iManaCost" };
		inline netvar_t m_bAutoCastState = /* 0x58c, bool */ { "client.dll", "C_DOTABaseAbility", "m_bAutoCastState" };
		inline netvar_t m_bAltCastState = /* 0x58d, bool */ { "client.dll", "C_DOTABaseAbility", "m_bAltCastState" };
		inline netvar_t m_flChannelStartTime = /* 0x590, GameTime_t */ { "client.dll", "C_DOTABaseAbility", "m_flChannelStartTime" };
		inline netvar_t m_flCastStartTime = /* 0x594, GameTime_t */ { "client.dll", "C_DOTABaseAbility", "m_flCastStartTime" };
		inline netvar_t m_bInIndefiniteCooldown = /* 0x598, bool */ { "client.dll", "C_DOTABaseAbility", "m_bInIndefiniteCooldown" };
		inline netvar_t m_bFrozenCooldown = /* 0x599, bool */ { "client.dll", "C_DOTABaseAbility", "m_bFrozenCooldown" };
		inline netvar_t m_flOverrideCastPoint = /* 0x59c, float32 */ { "client.dll", "C_DOTABaseAbility", "m_flOverrideCastPoint" };
		inline netvar_t m_bStolen = /* 0x5a0, bool */ { "client.dll", "C_DOTABaseAbility", "m_bStolen" };
		inline netvar_t m_bStealable = /* 0x5a1, bool */ { "client.dll", "C_DOTABaseAbility", "m_bStealable" };
		inline netvar_t m_bReplicated = /* 0x5a2, bool */ { "client.dll", "C_DOTABaseAbility", "m_bReplicated" };
		inline netvar_t m_nAbilityCurrentCharges = /* 0x5a4, int32 */ { "client.dll", "C_DOTABaseAbility", "m_nAbilityCurrentCharges" };
		inline netvar_t m_fAbilityChargeRestoreTimeRemaining = /* 0x5a8, float32 */ { "client.dll", "C_DOTABaseAbility", "m_fAbilityChargeRestoreTimeRemaining" };
		inline netvar_t m_bUpgradeRecommended = /* 0x5ac, bool */ { "client.dll", "C_DOTABaseAbility", "m_bUpgradeRecommended" };
		inline netvar_t m_nMaxLevelOverride = /* 0x5b0, int32 */ { "client.dll", "C_DOTABaseAbility", "m_nMaxLevelOverride" };
		inline netvar_t m_nRequiredLevelOverride = /* 0x5b4, int32 */ { "client.dll", "C_DOTABaseAbility", "m_nRequiredLevelOverride" };
		inline netvar_t m_nLevelsBetweenUpgradesOverride = /* 0x5b8, int32 */ { "client.dll", "C_DOTABaseAbility", "m_nLevelsBetweenUpgradesOverride" };
		inline netvar_t m_nHeroFacetKey = /* 0x5c0, HeroFacetKey_t */ { "client.dll", "C_DOTABaseAbility", "m_nHeroFacetKey" };
		inline netvar_t m_flTotalExtendedChannelTime = /* 0x5c8, float32 */ { "client.dll", "C_DOTABaseAbility", "m_flTotalExtendedChannelTime" };
		inline netvar_t m_bGrantedByFacet = /* 0x5cc, bool */ { "client.dll", "C_DOTABaseAbility", "m_bGrantedByFacet" };
		inline netvar_t m_flLastCastClickTime = /* 0x5d0, float32 */ { "client.dll", "C_DOTABaseAbility", "m_flLastCastClickTime" };
	}
	// client.dll
	// C_DOTA_Item < C_DOTABaseAbility < C_BaseEntity < CEntityInstance
	namespace C_DOTA_Item {
		using namespace C_DOTABaseAbility;

		inline netvar_t m_CastAnimation = /* 0x5d8, int32 */ { "client.dll", "C_DOTA_Item", "m_CastAnimation" };
		inline netvar_t m_bCombinable = /* 0x5dc, bool */ { "client.dll", "C_DOTA_Item", "m_bCombinable" };
		inline netvar_t m_bPermanent = /* 0x5dd, bool */ { "client.dll", "C_DOTA_Item", "m_bPermanent" };
		inline netvar_t m_bStackable = /* 0x5de, bool */ { "client.dll", "C_DOTA_Item", "m_bStackable" };
		inline netvar_t m_iStackableMax = /* 0x5e0, int32 */ { "client.dll", "C_DOTA_Item", "m_iStackableMax" };
		inline netvar_t m_bRecipe = /* 0x5e4, bool */ { "client.dll", "C_DOTA_Item", "m_bRecipe" };
		inline netvar_t m_bRecipeConsumesCharges = /* 0x5e5, bool */ { "client.dll", "C_DOTA_Item", "m_bRecipeConsumesCharges" };
		inline netvar_t m_iSharability = /* 0x5e8, int32 */ { "client.dll", "C_DOTA_Item", "m_iSharability" };
		inline netvar_t m_bDroppable = /* 0x5ec, bool */ { "client.dll", "C_DOTA_Item", "m_bDroppable" };
		inline netvar_t m_bPurchasable = /* 0x5ed, bool */ { "client.dll", "C_DOTA_Item", "m_bPurchasable" };
		inline netvar_t m_bSellable = /* 0x5ee, bool */ { "client.dll", "C_DOTA_Item", "m_bSellable" };
		inline netvar_t m_bInitiallySellable = /* 0x5ef, bool */ { "client.dll", "C_DOTA_Item", "m_bInitiallySellable" };
		inline netvar_t m_bForceUnsellable = /* 0x5f0, bool */ { "client.dll", "C_DOTA_Item", "m_bForceUnsellable" };
		inline netvar_t m_bRequiresCharges = /* 0x5f1, bool */ { "client.dll", "C_DOTA_Item", "m_bRequiresCharges" };
		inline netvar_t m_bDisplayCharges = /* 0x5f2, bool */ { "client.dll", "C_DOTA_Item", "m_bDisplayCharges" };
		inline netvar_t m_bHideCharges = /* 0x5f3, bool */ { "client.dll", "C_DOTA_Item", "m_bHideCharges" };
		inline netvar_t m_bKillable = /* 0x5f4, bool */ { "client.dll", "C_DOTA_Item", "m_bKillable" };
		inline netvar_t m_bGloballyCombinable = /* 0x5f5, bool */ { "client.dll", "C_DOTA_Item", "m_bGloballyCombinable" };
		inline netvar_t m_bDisassemblable = /* 0x5f6, bool */ { "client.dll", "C_DOTA_Item", "m_bDisassemblable" };
		inline netvar_t m_bNeverDisassemble = /* 0x5f7, bool */ { "client.dll", "C_DOTA_Item", "m_bNeverDisassemble" };
		inline netvar_t m_bIsNeutralDrop = /* 0x5f8, bool */ { "client.dll", "C_DOTA_Item", "m_bIsNeutralDrop" };
		inline netvar_t m_nNeutralDropTeam = /* 0x5fc, int32 */ { "client.dll", "C_DOTA_Item", "m_nNeutralDropTeam" };
		inline netvar_t m_bAlertable = /* 0x600, bool */ { "client.dll", "C_DOTA_Item", "m_bAlertable" };
		inline netvar_t m_iInitialCharges = /* 0x604, int32 */ { "client.dll", "C_DOTA_Item", "m_iInitialCharges" };
		inline netvar_t m_bCastOnPickup = /* 0x608, bool */ { "client.dll", "C_DOTA_Item", "m_bCastOnPickup" };
		inline netvar_t m_bOnlyPlayerHeroPickup = /* 0x609, bool */ { "client.dll", "C_DOTA_Item", "m_bOnlyPlayerHeroPickup" };
		inline netvar_t m_bCreepHeroPickup = /* 0x60a, bool */ { "client.dll", "C_DOTA_Item", "m_bCreepHeroPickup" };
		inline netvar_t m_bCanBeConsumed = /* 0x60b, bool */ { "client.dll", "C_DOTA_Item", "m_bCanBeConsumed" };
		inline netvar_t m_iValuelessCharges = /* 0x60c, int32 */ { "client.dll", "C_DOTA_Item", "m_iValuelessCharges" };
		inline netvar_t m_iCurrentCharges = /* 0x610, int32 */ { "client.dll", "C_DOTA_Item", "m_iCurrentCharges" };
		inline netvar_t m_iSecondaryCharges = /* 0x614, int32 */ { "client.dll", "C_DOTA_Item", "m_iSecondaryCharges" };
		inline netvar_t m_iMaxCharges = /* 0x618, int32 */ { "client.dll", "C_DOTA_Item", "m_iMaxCharges" };
		inline netvar_t m_bCombineLocked = /* 0x61c, bool */ { "client.dll", "C_DOTA_Item", "m_bCombineLocked" };
		inline netvar_t m_flPurchaseTime = /* 0x620, GameTime_t */ { "client.dll", "C_DOTA_Item", "m_flPurchaseTime" };
		inline netvar_t m_flAssembledTime = /* 0x624, GameTime_t */ { "client.dll", "C_DOTA_Item", "m_flAssembledTime" };
		inline netvar_t m_bPurchasedWhileDead = /* 0x628, bool */ { "client.dll", "C_DOTA_Item", "m_bPurchasedWhileDead" };
		inline netvar_t m_bCanBeUsedOutOfInventory = /* 0x629, bool */ { "client.dll", "C_DOTA_Item", "m_bCanBeUsedOutOfInventory" };
		inline netvar_t m_bItemEnabled = /* 0x62a, bool */ { "client.dll", "C_DOTA_Item", "m_bItemEnabled" };
		inline netvar_t m_flEnableTime = /* 0x62c, GameTime_t */ { "client.dll", "C_DOTA_Item", "m_flEnableTime" };
		inline netvar_t m_flReclaimTime = /* 0x630, GameTime_t */ { "client.dll", "C_DOTA_Item", "m_flReclaimTime" };
		inline netvar_t m_bDisplayOwnership = /* 0x634, bool */ { "client.dll", "C_DOTA_Item", "m_bDisplayOwnership" };
		inline netvar_t m_bShowOnMinimap = /* 0x635, bool */ { "client.dll", "C_DOTA_Item", "m_bShowOnMinimap" };
		inline netvar_t m_flMinimapIconSize = /* 0x638, float32 */ { "client.dll", "C_DOTA_Item", "m_flMinimapIconSize" };
		inline netvar_t m_bIsUpgradeable = /* 0x63c, bool */ { "client.dll", "C_DOTA_Item", "m_bIsUpgradeable" };
		inline netvar_t m_nUpgradeProgress = /* 0x640, int32 */ { "client.dll", "C_DOTA_Item", "m_nUpgradeProgress" };
		inline netvar_t m_nUpgradeGoal = /* 0x644, int32 */ { "client.dll", "C_DOTA_Item", "m_nUpgradeGoal" };
		inline netvar_t m_hOldOwnerEntity = /* 0x648, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTA_Item", "m_hOldOwnerEntity" };
		inline netvar_t m_iOldCharges = /* 0x64c, int32 */ { "client.dll", "C_DOTA_Item", "m_iOldCharges" };
		inline netvar_t m_iPlayerOwnerID = /* 0x650, PlayerID_t */ { "client.dll", "C_DOTA_Item", "m_iPlayerOwnerID" };
		inline netvar_t m_vecPreGameTransferPlayerIDs = /* 0x658, C_NetworkUtlVectorBase< uint8 > */ { "client.dll", "C_DOTA_Item", "m_vecPreGameTransferPlayerIDs" };
	}
	// client.dll
	// PlayerResourcePlayerData_t
	namespace PlayerResourcePlayerData_t {
		inline netvar_t m_bIsValid = /* 0x30, bool */ { "client.dll", "PlayerResourcePlayerData_t", "m_bIsValid" };
		inline netvar_t m_iszPlayerName = /* 0x38, CUtlSymbolLarge */ { "client.dll", "PlayerResourcePlayerData_t", "m_iszPlayerName" };
		inline netvar_t m_iPlayerTeam = /* 0x40, int32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_iPlayerTeam" };
		inline netvar_t m_bFullyJoinedServer = /* 0x44, bool */ { "client.dll", "PlayerResourcePlayerData_t", "m_bFullyJoinedServer" };
		inline netvar_t m_bFakeClient = /* 0x45, bool */ { "client.dll", "PlayerResourcePlayerData_t", "m_bFakeClient" };
		inline netvar_t m_bIsBroadcaster = /* 0x46, bool */ { "client.dll", "PlayerResourcePlayerData_t", "m_bIsBroadcaster" };
		inline netvar_t m_iBroadcasterChannel = /* 0x48, uint32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_iBroadcasterChannel" };
		inline netvar_t m_iBroadcasterChannelSlot = /* 0x4c, uint32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_iBroadcasterChannelSlot" };
		inline netvar_t m_bIsBroadcasterChannelCameraman = /* 0x50, bool */ { "client.dll", "PlayerResourcePlayerData_t", "m_bIsBroadcasterChannelCameraman" };
		inline netvar_t m_iConnectionState = /* 0x54, int32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_iConnectionState" };
		inline netvar_t m_iPlayerSteamID = /* 0x58, uint64 */ { "client.dll", "PlayerResourcePlayerData_t", "m_iPlayerSteamID" };
		inline netvar_t m_eCoachTeam = /* 0x60, DOTATeam_t */ { "client.dll", "PlayerResourcePlayerData_t", "m_eCoachTeam" };
		inline netvar_t m_vecPrivateCoachPlayerIDs = /* 0x68, C_NetworkUtlVectorBase< PlayerID_t > */ { "client.dll", "PlayerResourcePlayerData_t", "m_vecPrivateCoachPlayerIDs" };
		inline netvar_t m_unCoachRating = /* 0x80, uint32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_unCoachRating" };
		inline netvar_t m_eLiveSpectatorTeam = /* 0x84, DOTATeam_t */ { "client.dll", "PlayerResourcePlayerData_t", "m_eLiveSpectatorTeam" };
		inline netvar_t m_nLiveSpectatorSpectatedHeroIndex = /* 0x88, int32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_nLiveSpectatorSpectatedHeroIndex" };
		inline netvar_t m_bIsPlusSubscriber = /* 0x8c, bool */ { "client.dll", "PlayerResourcePlayerData_t", "m_bIsPlusSubscriber" };
		inline netvar_t m_bWasMVPLastGame = /* 0x8d, bool */ { "client.dll", "PlayerResourcePlayerData_t", "m_bWasMVPLastGame" };
		inline netvar_t m_nCavernCrawlMapVariant = /* 0x8e, CavernCrawlMapVariant_t */ { "client.dll", "PlayerResourcePlayerData_t", "m_nCavernCrawlMapVariant" };
		inline netvar_t m_eAccoladeType = /* 0x90, int32[3] */ { "client.dll", "PlayerResourcePlayerData_t", "m_eAccoladeType" };
		inline netvar_t m_unAccoladeData = /* 0xa0, uint64[3] */ { "client.dll", "PlayerResourcePlayerData_t", "m_unAccoladeData" };
		inline netvar_t m_iRankTier = /* 0xb8, int32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_iRankTier" };
		inline netvar_t m_iLeaderboardRank = /* 0xbc, int32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_iLeaderboardRank" };
		inline netvar_t m_eMmrBoostType = /* 0xc0, int32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_eMmrBoostType" };
		inline netvar_t m_iTitle = /* 0xc4, int32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_iTitle" };
		inline netvar_t m_unFavTeamPacked = /* 0xc8, uint64 */ { "client.dll", "PlayerResourcePlayerData_t", "m_unFavTeamPacked" };
		inline netvar_t m_nPlayerSlot = /* 0xd0, CPlayerSlot */ { "client.dll", "PlayerResourcePlayerData_t", "m_nPlayerSlot" };
		inline netvar_t m_bIsBot = /* 0xd4, bool */ { "client.dll", "PlayerResourcePlayerData_t", "m_bIsBot" };
		inline netvar_t m_bHasNeutralTier = /* 0xd5, bool[5] */ { "client.dll", "PlayerResourcePlayerData_t", "m_bHasNeutralTier" };
		inline netvar_t m_bHasRedeemedNeutralTier = /* 0xda, bool[5] */ { "client.dll", "PlayerResourcePlayerData_t", "m_bHasRedeemedNeutralTier" };
		inline netvar_t m_nCommLevel = /* 0xe0, int32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_nCommLevel" };
		inline netvar_t m_nBehaviorLevel = /* 0xe4, int32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_nBehaviorLevel" };
		inline netvar_t m_flLastCommsTime = /* 0xe8, float32 */ { "client.dll", "PlayerResourcePlayerData_t", "m_flLastCommsTime" };
	}
	// client.dll
	// C_DOTA_Item_PowerTreads < C_DOTA_Item < C_DOTABaseAbility < C_BaseEntity < CEntityInstance
	namespace C_DOTA_Item_PowerTreads {
		using namespace C_DOTA_Item;

		inline netvar_t m_iStat = /* 0x678, int32 */ { "client.dll", "C_DOTA_Item_PowerTreads", "m_iStat" };
	}
	// client.dll
	// C_DOTA_UnitInventory
	namespace C_DOTA_UnitInventory {
		inline netvar_t m_SharedCooldownList = /* 0x8, CUtlVector< sSharedCooldownInfo > */ { "client.dll", "C_DOTA_UnitInventory", "m_SharedCooldownList" };
		inline netvar_t m_hItems = /* 0x20, CHandle< C_BaseEntity >[19] */ { "client.dll", "C_DOTA_UnitInventory", "m_hItems" };
		inline netvar_t m_bItemQueried = /* 0x6c, bool[21] */ { "client.dll", "C_DOTA_UnitInventory", "m_bItemQueried" };
		inline netvar_t m_iParity = /* 0x84, int32 */ { "client.dll", "C_DOTA_UnitInventory", "m_iParity" };
		inline netvar_t m_hInventoryParent = /* 0x88, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTA_UnitInventory", "m_hInventoryParent" };
		inline netvar_t m_bIsActive = /* 0x8c, bool */ { "client.dll", "C_DOTA_UnitInventory", "m_bIsActive" };
		inline netvar_t m_bStashEnabled = /* 0x8d, bool */ { "client.dll", "C_DOTA_UnitInventory", "m_bStashEnabled" };
		inline netvar_t m_hTransientCastItem = /* 0x90, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTA_UnitInventory", "m_hTransientCastItem" };
		inline netvar_t m_bSendChangedMsg = /* 0xb0, bool */ { "client.dll", "C_DOTA_UnitInventory", "m_bSendChangedMsg" };
	}
	// client.dll
	// C_DOTA_BaseNPC_Hero < C_DOTA_BaseNPC_Additive < C_DOTA_BaseNPC < C_NextBotCombatCharacter < C_BaseCombatCharacter < C_BaseFlex < C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_DOTA_BaseNPC_Hero {
		using namespace C_DOTA_BaseNPC_Additive;

		inline netvar_t m_bBuybackDisabled = /* 0x0, bitfield:1 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_bBuybackDisabled" };
		inline netvar_t m_iCurrentXP = /* 0x1810, int32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_iCurrentXP" };
		inline netvar_t m_iAbilityPoints = /* 0x1814, int32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_iAbilityPoints" };
		inline netvar_t m_iTotalAbilityPoints = /* 0x1818, int32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_iTotalAbilityPoints" };
		inline netvar_t m_flRespawnTime = /* 0x181c, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flRespawnTime" };
		inline netvar_t m_flRespawnTimePenalty = /* 0x1820, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flRespawnTimePenalty" };
		inline netvar_t m_flStrength = /* 0x1824, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flStrength" };
		inline netvar_t m_flAgility = /* 0x1828, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flAgility" };
		inline netvar_t m_flIntellect = /* 0x182c, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flIntellect" };
		inline netvar_t m_flStrengthTotal = /* 0x1830, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flStrengthTotal" };
		inline netvar_t m_flAgilityTotal = /* 0x1834, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flAgilityTotal" };
		inline netvar_t m_flIntellectTotal = /* 0x1838, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flIntellectTotal" };
		inline netvar_t m_hFacetAbilities = /* 0x1840, C_NetworkUtlVectorBase< CHandle< C_BaseEntity > > */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_hFacetAbilities" };
		inline netvar_t m_iRecentDamage = /* 0x1858, int32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_iRecentDamage" };
		inline netvar_t m_fPainFactor = /* 0x185c, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_fPainFactor" };
		inline netvar_t m_fTargetPainFactor = /* 0x1860, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_fTargetPainFactor" };
		inline netvar_t m_bLifeState = /* 0x1864, bool */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_bLifeState" };
		inline netvar_t m_bFirstSpawn = /* 0x1865, bool */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_bFirstSpawn" };
		inline netvar_t m_nFXStunIndex = /* 0x1868, ParticleIndex_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_nFXStunIndex" };
		inline netvar_t m_nFXSilenceIndex = /* 0x186c, ParticleIndex_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_nFXSilenceIndex" };
		inline netvar_t m_nFXDeathIndex = /* 0x1870, ParticleIndex_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_nFXDeathIndex" };
		inline netvar_t m_iPlayerID = /* 0x1898, PlayerID_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_iPlayerID" };
		inline netvar_t m_iHeroFacetID = /* 0x189c, HeroFacetID_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_iHeroFacetID" };
		inline netvar_t m_hReplicatingOtherHeroModel = /* 0x18a0, CHandle< C_DOTA_BaseNPC_Hero > */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_hReplicatingOtherHeroModel" };
		inline netvar_t m_bReincarnating = /* 0x18a4, bool */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_bReincarnating" };
		inline netvar_t m_bCustomKillEffect = /* 0x18a5, bool */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_bCustomKillEffect" };
		inline netvar_t m_flSpawnedAt = /* 0x18a8, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flSpawnedAt" };
		inline netvar_t m_bScriptDisableRespawns = /* 0x18ac, bool */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_bScriptDisableRespawns" };
		inline netvar_t m_iPrimaryAttribute = /* 0x18b0, int32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_iPrimaryAttribute" };
		inline netvar_t m_nLastDrawnHealth = /* 0x18b4, int32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_nLastDrawnHealth" };
		inline netvar_t m_flHurtAmount = /* 0x18b8, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flHurtAmount" };
		inline netvar_t m_flLastHurtTime = /* 0x18bc, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flLastHurtTime" };
		inline netvar_t m_flHurtDecayRate = /* 0x18c0, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flHurtDecayRate" };
		inline netvar_t m_flLastHealTime = /* 0x18c4, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flLastHealTime" };
		inline netvar_t m_flLastTreeShakeTime = /* 0x18c8, GameTime_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flLastTreeShakeTime" };
		inline netvar_t m_CenterOnHeroCooldownTimer = /* 0x18d0, CountdownTimer */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_CenterOnHeroCooldownTimer" };
		inline netvar_t m_CombinedModels = /* 0x1af0, CStrongHandle< InfoForResourceTypeCModel >[4] */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_CombinedModels" };
		inline netvar_t m_nCurrentCombinedModelIndex = /* 0x1b10, int32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_nCurrentCombinedModelIndex" };
		inline netvar_t m_nPendingCombinedModelIndex = /* 0x1b14, int32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_nPendingCombinedModelIndex" };
		inline netvar_t m_iHeroID = /* 0x1b18, HeroID_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_iHeroID" };
		inline netvar_t m_flCheckLegacyItemsAt = /* 0x1b1c, float32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_flCheckLegacyItemsAt" };
		inline netvar_t m_bDisplayAdditionalHeroes = /* 0x1b20, bool */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_bDisplayAdditionalHeroes" };
		inline netvar_t m_CombinedParticleModels = /* 0x1b30, CStrongHandle< InfoForResourceTypeCModel >[4] */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_CombinedParticleModels" };
		inline netvar_t m_vecAttachedParticleIndeces = /* 0x1b50, CUtlVector< ParticleIndex_t > */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_vecAttachedParticleIndeces" };
		inline netvar_t m_hPets = /* 0x1b68, CUtlVector< CHandle< C_BaseEntity > > */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_hPets" };
		inline netvar_t m_nKillStreakFX = /* 0x1b80, ParticleIndex_t */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_nKillStreakFX" };
		inline netvar_t m_nKillStreakFXTier = /* 0x1b84, int32 */ { "client.dll", "C_DOTA_BaseNPC_Hero", "m_nKillStreakFXTier" };
	}
	// client.dll
	// C_DOTAPlayerController < CBasePlayerController < C_BaseEntity < CEntityInstance
	namespace C_DOTAPlayerController {
		using namespace CBasePlayerController;

		inline netvar_t m_iMinimapMove = /* 0x778, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iMinimapMove" };
		inline netvar_t m_pClickBehaviorKeys = /* 0x780, KeyValues* */ { "client.dll", "C_DOTAPlayerController", "m_pClickBehaviorKeys" };
		inline netvar_t m_flCenterTime = /* 0x788, GameTime_t */ { "client.dll", "C_DOTAPlayerController", "m_flCenterTime" };
		inline netvar_t m_iConfirmationIndex = /* 0x78c, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iConfirmationIndex" };
		inline netvar_t m_bCenterOnHero = /* 0x790, bool */ { "client.dll", "C_DOTAPlayerController", "m_bCenterOnHero" };
		inline netvar_t m_bHeroAssigned = /* 0x791, bool */ { "client.dll", "C_DOTAPlayerController", "m_bHeroAssigned" };
		inline netvar_t m_nKeyBindHeroID = /* 0x794, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nKeyBindHeroID" };
		inline netvar_t m_bUsingCameraMan = /* 0x798, bool */ { "client.dll", "C_DOTAPlayerController", "m_bUsingCameraMan" };
		inline netvar_t m_bUsingAssistedCameraOperator = /* 0x799, bool */ { "client.dll", "C_DOTAPlayerController", "m_bUsingAssistedCameraOperator" };
		inline netvar_t m_nPlayerAssistFlags = /* 0x79c, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nPlayerAssistFlags" };
		inline netvar_t m_vLatestEvent = /* 0x7a0, Vector */ { "client.dll", "C_DOTAPlayerController", "m_vLatestEvent" };
		inline netvar_t m_hFreeDrawAbility = /* 0x7d0, CHandle< C_DOTABaseAbility > */ { "client.dll", "C_DOTAPlayerController", "m_hFreeDrawAbility" };
		inline netvar_t m_vLastFreeDrawPosition = /* 0x7d4, Vector */ { "client.dll", "C_DOTAPlayerController", "m_vLastFreeDrawPosition" };
		inline netvar_t m_nPlayerID = /* 0x7e0, PlayerID_t */ { "client.dll", "C_DOTAPlayerController", "m_nPlayerID" };
		inline netvar_t m_hAssignedHero = /* 0x7e4, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAPlayerController", "m_hAssignedHero" };
		inline netvar_t m_hLastAssignedHero = /* 0x7e8, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAPlayerController", "m_hLastAssignedHero" };
		inline netvar_t m_hKillCamUnit = /* 0x7ec, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAPlayerController", "m_hKillCamUnit" };
		inline netvar_t m_hPreviousKillCamUnit = /* 0x7f0, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAPlayerController", "m_hPreviousKillCamUnit" };
		inline netvar_t m_flKillCamUnitReceivedTime = /* 0x7f4, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flKillCamUnitReceivedTime" };
		inline netvar_t m_nRareLineClickCount = /* 0x800, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nRareLineClickCount" };
		inline netvar_t m_nRareLinesPlayed = /* 0x804, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nRareLinesPlayed" };
		inline netvar_t m_nRareLineGroup = /* 0x808, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nRareLineGroup" };
		inline netvar_t m_flLastRareLinePlayTime = /* 0x80c, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flLastRareLinePlayTime" };
		inline netvar_t m_flLastUnitOrdersSendTime = /* 0x810, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flLastUnitOrdersSendTime" };
		inline netvar_t m_bTeleportRequiresHalt = /* 0x814, bool */ { "client.dll", "C_DOTAPlayerController", "m_bTeleportRequiresHalt" };
		inline netvar_t m_bChannelRequiresHalt = /* 0x815, bool */ { "client.dll", "C_DOTAPlayerController", "m_bChannelRequiresHalt" };
		inline netvar_t m_bAutoPurchaseItems = /* 0x816, bool */ { "client.dll", "C_DOTAPlayerController", "m_bAutoPurchaseItems" };
		inline netvar_t m_bDisableHUDErrorMessages = /* 0x817, bool */ { "client.dll", "C_DOTAPlayerController", "m_bDisableHUDErrorMessages" };
		inline netvar_t m_iMouseDragStartX = /* 0x818, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iMouseDragStartX" };
		inline netvar_t m_iMouseDragStartY = /* 0x81c, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iMouseDragStartY" };
		inline netvar_t m_nWeatherType = /* 0x820, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nWeatherType" };
		inline netvar_t m_bDynamicWeatherSystemActive = /* 0x824, bool */ { "client.dll", "C_DOTAPlayerController", "m_bDynamicWeatherSystemActive" };
		inline netvar_t m_bDynamicSoundHandled = /* 0x825, bool */ { "client.dll", "C_DOTAPlayerController", "m_bDynamicSoundHandled" };
		inline netvar_t m_flDynamicWeatherNextSwitchTime = /* 0x828, GameTime_t */ { "client.dll", "C_DOTAPlayerController", "m_flDynamicWeatherNextSwitchTime" };
		inline netvar_t m_flDynamicWeatherScaleFinishedTime = /* 0x82c, GameTime_t */ { "client.dll", "C_DOTAPlayerController", "m_flDynamicWeatherScaleFinishedTime" };
		inline netvar_t m_flDynamicWeatherIntensity = /* 0x830, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flDynamicWeatherIntensity" };
		inline netvar_t m_nXPRangeFXIndex = /* 0x834, ParticleIndex_t */ { "client.dll", "C_DOTAPlayerController", "m_nXPRangeFXIndex" };
		inline netvar_t m_nVisionRangeFXIndex = /* 0x838, ParticleIndex_t */ { "client.dll", "C_DOTAPlayerController", "m_nVisionRangeFXIndex" };
		inline netvar_t m_hRangeHintAbility = /* 0x840, CHandle< C_DOTABaseAbility > */ { "client.dll", "C_DOTAPlayerController", "m_hRangeHintAbility" };
		inline netvar_t m_nRangeHintFXIndex = /* 0x844, ParticleIndex_t */ { "client.dll", "C_DOTAPlayerController", "m_nRangeHintFXIndex" };
		inline netvar_t m_flRangeHintFXLastRadius = /* 0x848, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flRangeHintFXLastRadius" };
		inline netvar_t m_nSelectedControlGroup = /* 0x84c, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nSelectedControlGroup" };
		inline netvar_t m_nCachedCoachedTeam = /* 0x850, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nCachedCoachedTeam" };
		inline netvar_t m_hActiveAbility = /* 0x854, CHandle< C_DOTABaseAbility > */ { "client.dll", "C_DOTAPlayerController", "m_hActiveAbility" };
		inline netvar_t m_unitorders = /* 0x858, CUtlVector< CUnitOrders > */ { "client.dll", "C_DOTAPlayerController", "m_unitorders" };
		inline netvar_t m_nOutgoingOrderSequenceNumber = /* 0x870, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nOutgoingOrderSequenceNumber" };
		inline netvar_t m_nServerOrderSequenceNumber = /* 0x874, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nServerOrderSequenceNumber" };
		inline netvar_t m_nLastSentOutgoingOrderSequenceNumber = /* 0x878, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nLastSentOutgoingOrderSequenceNumber" };
		inline netvar_t m_nSelectedUnits = /* 0x880, CUtlVector< CEntityIndex > */ { "client.dll", "C_DOTAPlayerController", "m_nSelectedUnits" };
		inline netvar_t m_nWaypoints = /* 0x898, CUtlVector< ParticleIndex_t > */ { "client.dll", "C_DOTAPlayerController", "m_nWaypoints" };
		inline netvar_t m_iActions = /* 0x8b0, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iActions" };
		inline netvar_t m_hQueryUnit = /* 0x8b4, CHandle< C_DOTA_BaseNPC > */ { "client.dll", "C_DOTAPlayerController", "m_hQueryUnit" };
		inline netvar_t m_bInQuery = /* 0x8b8, bool */ { "client.dll", "C_DOTAPlayerController", "m_bInQuery" };
		inline netvar_t m_bSelectionChangedInDataUpdate = /* 0x8b9, bool */ { "client.dll", "C_DOTAPlayerController", "m_bSelectionChangedInDataUpdate" };
		inline netvar_t m_flQueryInhibitingActionTime = /* 0x920, GameTime_t */ { "client.dll", "C_DOTAPlayerController", "m_flQueryInhibitingActionTime" };
		inline netvar_t m_flQueryInhibitDuration = /* 0x924, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flQueryInhibitDuration" };
		inline netvar_t m_RingedEntities = /* 0x928, CUtlVector< CHandle< C_BaseEntity > > */ { "client.dll", "C_DOTAPlayerController", "m_RingedEntities" };
		inline netvar_t m_ActiveRingOwners = /* 0x940, CUtlVector< CHandle< C_BaseEntity > > */ { "client.dll", "C_DOTAPlayerController", "m_ActiveRingOwners" };
		inline netvar_t m_bOverridingQuery = /* 0x958, bool */ { "client.dll", "C_DOTAPlayerController", "m_bOverridingQuery" };
		inline netvar_t m_flLastAutoRepeatTime = /* 0x960, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flLastAutoRepeatTime" };
		inline netvar_t m_flConsumeDoubleclickTime = /* 0x964, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flConsumeDoubleclickTime" };
		inline netvar_t m_LightInfoWeatherEffect = /* 0x968, CUtlString */ { "client.dll", "C_DOTAPlayerController", "m_LightInfoWeatherEffect" };
		inline netvar_t m_bPreviousWasLightInfoWeather = /* 0x970, bool */ { "client.dll", "C_DOTAPlayerController", "m_bPreviousWasLightInfoWeather" };
		inline netvar_t m_MapDefaultWeatherEffect = /* 0x978, CUtlString */ { "client.dll", "C_DOTAPlayerController", "m_MapDefaultWeatherEffect" };
		inline netvar_t m_bMapUsesDynamicWeather = /* 0x980, bool */ { "client.dll", "C_DOTAPlayerController", "m_bMapUsesDynamicWeather" };
		inline netvar_t m_nCastRangeEffectCreationRadius = /* 0x9a8, int32 */ { "client.dll", "C_DOTAPlayerController", "m_nCastRangeEffectCreationRadius" };
		inline netvar_t m_vecSuggestedWardLocationEffects = /* 0x9c8, CUtlVector< ParticleIndex_t > */ { "client.dll", "C_DOTAPlayerController", "m_vecSuggestedWardLocationEffects" };
		inline netvar_t m_pSmartCastNPC = /* 0x9e0, C_DOTA_BaseNPC* */ { "client.dll", "C_DOTAPlayerController", "m_pSmartCastNPC" };
		inline netvar_t m_nTeamSprayParticleIndex = /* 0x9e8, ParticleIndex_t */ { "client.dll", "C_DOTAPlayerController", "m_nTeamSprayParticleIndex" };
		inline netvar_t m_bIsNextCastOrderFromMouseClick = /* 0x9ec, bool */ { "client.dll", "C_DOTAPlayerController", "m_bIsNextCastOrderFromMouseClick" };
		inline netvar_t m_iCursor = /* 0xa20, int32[2] */ { "client.dll", "C_DOTAPlayerController", "m_iCursor" };
		inline netvar_t m_iSpectatorClickBehavior = /* 0xa28, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iSpectatorClickBehavior" };
		inline netvar_t m_flAspectRatio = /* 0xa2c, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flAspectRatio" };
		inline netvar_t m_hSpectatorQueryUnit = /* 0xa30, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAPlayerController", "m_hSpectatorQueryUnit" };
		inline netvar_t m_iStatsPanel = /* 0xa34, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iStatsPanel" };
		inline netvar_t m_iShopPanel = /* 0xa38, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iShopPanel" };
		inline netvar_t m_iShopViewMode = /* 0xa3c, ShopItemViewMode_t */ { "client.dll", "C_DOTAPlayerController", "m_iShopViewMode" };
		inline netvar_t m_iStatsDropdownCategory = /* 0xa40, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iStatsDropdownCategory" };
		inline netvar_t m_iStatsDropdownSort = /* 0xa44, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iStatsDropdownSort" };
		inline netvar_t m_szShopString = /* 0xa48, char[64] */ { "client.dll", "C_DOTAPlayerController", "m_szShopString" };
		inline netvar_t m_vecClientQuickBuyState = /* 0xa88, C_UtlVectorEmbeddedNetworkVar< ClientQuickBuyItemState > */ { "client.dll", "C_DOTAPlayerController", "m_vecClientQuickBuyState" };
		inline netvar_t m_bInShowCaseMode = /* 0xad8, bool */ { "client.dll", "C_DOTAPlayerController", "m_bInShowCaseMode" };
		inline netvar_t m_flCameraZoomAmount = /* 0xadc, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flCameraZoomAmount" };
		inline netvar_t m_iHighPriorityScore = /* 0xae0, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iHighPriorityScore" };
		inline netvar_t m_quickBuyItems = /* 0xae4, AbilityID_t[9] */ { "client.dll", "C_DOTAPlayerController", "m_quickBuyItems" };
		inline netvar_t m_quickBuyIsPurchasable = /* 0xb08, bool[9] */ { "client.dll", "C_DOTAPlayerController", "m_quickBuyIsPurchasable" };
		inline netvar_t m_flFrameTime = /* 0xb14, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flFrameTime" };
		inline netvar_t m_flFrameTimeStdDev = /* 0xb18, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flFrameTimeStdDev" };
		inline netvar_t m_flUnfilteredFrameTime = /* 0xb1c, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flUnfilteredFrameTime" };
		inline netvar_t m_NeutralCampAlarms = /* 0xb20, CUtlVector< NeutralCampStackPullAlarm_t > */ { "client.dll", "C_DOTAPlayerController", "m_NeutralCampAlarms" };
		inline netvar_t m_iPrevCursor = /* 0xb38, int32[2] */ { "client.dll", "C_DOTAPlayerController", "m_iPrevCursor" };
		inline netvar_t m_iPositionHistoryTail = /* 0xbe0, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iPositionHistoryTail" };
		inline netvar_t m_iMusicStatus = /* 0xbe4, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iMusicStatus" };
		inline netvar_t m_iPreviousMusicStatus = /* 0xbe8, int32 */ { "client.dll", "C_DOTAPlayerController", "m_iPreviousMusicStatus" };
		inline netvar_t m_bRequestedInventory = /* 0xbec, bool */ { "client.dll", "C_DOTAPlayerController", "m_bRequestedInventory" };
		inline netvar_t m_flMusicOperatorVals = /* 0xbf0, float32[3] */ { "client.dll", "C_DOTAPlayerController", "m_flMusicOperatorVals" };
		inline netvar_t m_iMusicOperatorVals = /* 0xbfc, int32[4] */ { "client.dll", "C_DOTAPlayerController", "m_iMusicOperatorVals" };
		inline netvar_t m_ControlGroups = /* 0xc10, CUtlVector< sControlGroupElem >[10] */ { "client.dll", "C_DOTAPlayerController", "m_ControlGroups" };
		inline netvar_t m_pkvControlGroupKV = /* 0xd00, KeyValues* */ { "client.dll", "C_DOTAPlayerController", "m_pkvControlGroupKV" };
		inline netvar_t m_flAltHeldStartTime = /* 0xd60, float32 */ { "client.dll", "C_DOTAPlayerController", "m_flAltHeldStartTime" };
	}
	// !GlobalTypes
	// CParticleFunction
	namespace CParticleFunction {
		inline netvar_t m_flOpStrength = /* 0x8, CParticleCollectionFloatInput */ { "!GlobalTypes", "CParticleFunction", "m_flOpStrength" };
		inline netvar_t m_nOpEndCapState = /* 0x168, ParticleEndcapMode_t */ { "!GlobalTypes", "CParticleFunction", "m_nOpEndCapState" };
		inline netvar_t m_flOpStartFadeInTime = /* 0x16c, float32 */ { "!GlobalTypes", "CParticleFunction", "m_flOpStartFadeInTime" };
		inline netvar_t m_flOpEndFadeInTime = /* 0x170, float32 */ { "!GlobalTypes", "CParticleFunction", "m_flOpEndFadeInTime" };
		inline netvar_t m_flOpStartFadeOutTime = /* 0x174, float32 */ { "!GlobalTypes", "CParticleFunction", "m_flOpStartFadeOutTime" };
		inline netvar_t m_flOpEndFadeOutTime = /* 0x178, float32 */ { "!GlobalTypes", "CParticleFunction", "m_flOpEndFadeOutTime" };
		inline netvar_t m_flOpFadeOscillatePeriod = /* 0x17c, float32 */ { "!GlobalTypes", "CParticleFunction", "m_flOpFadeOscillatePeriod" };
		inline netvar_t m_bNormalizeToStopTime = /* 0x180, bool */ { "!GlobalTypes", "CParticleFunction", "m_bNormalizeToStopTime" };
		inline netvar_t m_flOpTimeOffsetMin = /* 0x184, float32 */ { "!GlobalTypes", "CParticleFunction", "m_flOpTimeOffsetMin" };
		inline netvar_t m_flOpTimeOffsetMax = /* 0x188, float32 */ { "!GlobalTypes", "CParticleFunction", "m_flOpTimeOffsetMax" };
		inline netvar_t m_nOpTimeOffsetSeed = /* 0x18c, int32 */ { "!GlobalTypes", "CParticleFunction", "m_nOpTimeOffsetSeed" };
		inline netvar_t m_nOpTimeScaleSeed = /* 0x190, int32 */ { "!GlobalTypes", "CParticleFunction", "m_nOpTimeScaleSeed" };
		inline netvar_t m_flOpTimeScaleMin = /* 0x194, float32 */ { "!GlobalTypes", "CParticleFunction", "m_flOpTimeScaleMin" };
		inline netvar_t m_flOpTimeScaleMax = /* 0x198, float32 */ { "!GlobalTypes", "CParticleFunction", "m_flOpTimeScaleMax" };
		inline netvar_t m_bDisableOperator = /* 0x19e, bool */ { "!GlobalTypes", "CParticleFunction", "m_bDisableOperator" };
		inline netvar_t m_Notes = /* 0x1a0, CUtlString */ { "!GlobalTypes", "CParticleFunction", "m_Notes" };
	}
	// client.dll
	// CGameSceneNode
	namespace CGameSceneNode {
		inline netvar_t m_bDirtyHierarchy = /* 0x0, bitfield:1 */ { "client.dll", "CGameSceneNode", "m_bDirtyHierarchy" };
		inline netvar_t m_nodeToWorld = /* 0x10, CTransform */ { "client.dll", "CGameSceneNode", "m_nodeToWorld" };
		inline netvar_t m_pOwner = /* 0x30, CEntityInstance* */ { "client.dll", "CGameSceneNode", "m_pOwner" };
		inline netvar_t m_pParent = /* 0x38, CGameSceneNode* */ { "client.dll", "CGameSceneNode", "m_pParent" };
		inline netvar_t m_pChild = /* 0x40, CGameSceneNode* */ { "client.dll", "CGameSceneNode", "m_pChild" };
		inline netvar_t m_pNextSibling = /* 0x48, CGameSceneNode* */ { "client.dll", "CGameSceneNode", "m_pNextSibling" };
		inline netvar_t m_hParent = /* 0x78, CGameSceneNodeHandle */ { "client.dll", "CGameSceneNode", "m_hParent" };
		inline netvar_t m_vecOrigin = /* 0x88, CNetworkOriginCellCoordQuantizedVector */ { "client.dll", "CGameSceneNode", "m_vecOrigin" };
		inline netvar_t m_angRotation = /* 0xc0, QAngle */ { "client.dll", "CGameSceneNode", "m_angRotation" };
		inline netvar_t m_flScale = /* 0xcc, float32 */ { "client.dll", "CGameSceneNode", "m_flScale" };
		inline netvar_t m_vecAbsOrigin = /* 0xd0, Vector */ { "client.dll", "CGameSceneNode", "m_vecAbsOrigin" };
		inline netvar_t m_angAbsRotation = /* 0xdc, QAngle */ { "client.dll", "CGameSceneNode", "m_angAbsRotation" };
		inline netvar_t m_flAbsScale = /* 0xe8, float32 */ { "client.dll", "CGameSceneNode", "m_flAbsScale" };
		inline netvar_t m_nParentAttachmentOrBone = /* 0xec, int16 */ { "client.dll", "CGameSceneNode", "m_nParentAttachmentOrBone" };
		inline netvar_t m_bDebugAbsOriginChanges = /* 0xee, bool */ { "client.dll", "CGameSceneNode", "m_bDebugAbsOriginChanges" };
		inline netvar_t m_bDormant = /* 0xef, bool */ { "client.dll", "CGameSceneNode", "m_bDormant" };
		inline netvar_t m_bForceParentToBeNetworked = /* 0xf0, bool */ { "client.dll", "CGameSceneNode", "m_bForceParentToBeNetworked" };
		inline netvar_t m_nHierarchicalDepth = /* 0xf3, uint8 */ { "client.dll", "CGameSceneNode", "m_nHierarchicalDepth" };
		inline netvar_t m_nHierarchyType = /* 0xf4, uint8 */ { "client.dll", "CGameSceneNode", "m_nHierarchyType" };
		inline netvar_t m_nDoNotSetAnimTimeInInvalidatePhysicsCount = /* 0xf5, uint8 */ { "client.dll", "CGameSceneNode", "m_nDoNotSetAnimTimeInInvalidatePhysicsCount" };
		inline netvar_t m_name = /* 0xf8, CUtlStringToken */ { "client.dll", "CGameSceneNode", "m_name" };
		inline netvar_t m_hierarchyAttachName = /* 0x138, CUtlStringToken */ { "client.dll", "CGameSceneNode", "m_hierarchyAttachName" };
		inline netvar_t m_flZOffset = /* 0x13c, float32 */ { "client.dll", "CGameSceneNode", "m_flZOffset" };
		inline netvar_t m_flClientLocalScale = /* 0x140, float32 */ { "client.dll", "CGameSceneNode", "m_flClientLocalScale" };
		inline netvar_t m_vRenderOrigin = /* 0x144, Vector */ { "client.dll", "CGameSceneNode", "m_vRenderOrigin" };
	}
	// client.dll
	// CSkeletonInstance < CGameSceneNode
	namespace CSkeletonInstance {
		using namespace CGameSceneNode;

		inline netvar_t m_bDirtyMotionType = /* 0x0, bitfield:1 */ { "client.dll", "CSkeletonInstance", "m_bDirtyMotionType" };
		inline netvar_t m_modelState = /* 0x170, CModelState */ { "client.dll", "CSkeletonInstance", "m_modelState" };
		inline netvar_t m_bIsAnimationEnabled = /* 0x3a0, bool */ { "client.dll", "CSkeletonInstance", "m_bIsAnimationEnabled" };
		inline netvar_t m_bUseParentRenderBounds = /* 0x3a1, bool */ { "client.dll", "CSkeletonInstance", "m_bUseParentRenderBounds" };
		inline netvar_t m_bDisableSolidCollisionsForHierarchy = /* 0x3a2, bool */ { "client.dll", "CSkeletonInstance", "m_bDisableSolidCollisionsForHierarchy" };
		inline netvar_t m_materialGroup = /* 0x3a4, CUtlStringToken */ { "client.dll", "CSkeletonInstance", "m_materialGroup" };
		inline netvar_t m_nHitboxSet = /* 0x3a8, uint8 */ { "client.dll", "CSkeletonInstance", "m_nHitboxSet" };
	}
	// !GlobalTypes
	// CParticleInput
	namespace CParticleInput {
	}
	// client.dll
	// CModelState
	namespace CModelState {
		inline netvar_t m_hModel = /* 0xa0, CStrongHandle< InfoForResourceTypeCModel > */ { "client.dll", "CModelState", "m_hModel" };
		inline netvar_t m_ModelName = /* 0xa8, CUtlSymbolLarge */ { "client.dll", "CModelState", "m_ModelName" };
		inline netvar_t m_bClientClothCreationSuppressed = /* 0xe8, bool */ { "client.dll", "CModelState", "m_bClientClothCreationSuppressed" };
		inline netvar_t m_MeshGroupMask = /* 0x198, uint64 */ { "client.dll", "CModelState", "m_MeshGroupMask" };
		inline netvar_t m_nIdealMotionType = /* 0x21a, int8 */ { "client.dll", "CModelState", "m_nIdealMotionType" };
		inline netvar_t m_nForceLOD = /* 0x21b, int8 */ { "client.dll", "CModelState", "m_nForceLOD" };
		inline netvar_t m_nClothUpdateFlags = /* 0x21c, int8 */ { "client.dll", "CModelState", "m_nClothUpdateFlags" };
	}
	// client.dll
	// C_DOTA_PlayerResource < C_BaseEntity < CEntityInstance
	namespace C_DOTA_PlayerResource {
		using namespace C_BaseEntity;

		inline netvar_t m_bWasDataUpdateCreated = /* 0x548, bool */ { "client.dll", "C_DOTA_PlayerResource", "m_bWasDataUpdateCreated" };
		inline netvar_t m_vecPlayerTeamData = /* 0x550, C_UtlVectorEmbeddedNetworkVar< PlayerResourcePlayerTeamData_t > */ { "client.dll", "C_DOTA_PlayerResource", "m_vecPlayerTeamData" };
		inline netvar_t m_vecPlayerData = /* 0x5a0, C_UtlVectorEmbeddedNetworkVar< PlayerResourcePlayerData_t > */ { "client.dll", "C_DOTA_PlayerResource", "m_vecPlayerData" };
		inline netvar_t m_vecBrodcasterData = /* 0x5f0, C_UtlVectorEmbeddedNetworkVar< PlayerResourceBroadcasterData_t > */ { "client.dll", "C_DOTA_PlayerResource", "m_vecBrodcasterData" };
		inline netvar_t m_vecEventsForDisplay = /* 0x640, C_NetworkUtlVectorBase< uint32 > */ { "client.dll", "C_DOTA_PlayerResource", "m_vecEventsForDisplay" };
		inline netvar_t m_nPrimaryEventIndex = /* 0x658, int8 */ { "client.dll", "C_DOTA_PlayerResource", "m_nPrimaryEventIndex" };
		inline netvar_t m_nObsoleteEventIDAssociatedWithEventData = /* 0x65c, uint32 */ { "client.dll", "C_DOTA_PlayerResource", "m_nObsoleteEventIDAssociatedWithEventData" };
		inline netvar_t m_playerIDToPlayer = /* 0x660, CHandle< C_BaseEntity >[64] */ { "client.dll", "C_DOTA_PlayerResource", "m_playerIDToPlayer" };
		inline netvar_t m_iszName = /* 0x760, CUtlSymbolLarge[64] */ { "client.dll", "C_DOTA_PlayerResource", "m_iszName" };
		inline netvar_t m_iszHTMLSafeName = /* 0x960, CUtlSymbolLarge[64] */ { "client.dll", "C_DOTA_PlayerResource", "m_iszHTMLSafeName" };
		inline netvar_t m_iszFilteredHTMLSafeName = /* 0xb60, CUtlSymbolLarge[64] */ { "client.dll", "C_DOTA_PlayerResource", "m_iszFilteredHTMLSafeName" };
		inline netvar_t m_bDirtySuggestedItems = /* 0x2660, bool */ { "client.dll", "C_DOTA_PlayerResource", "m_bDirtySuggestedItems" };
		inline netvar_t m_bDirtyEstimatedMatchDuration = /* 0x2661, bool */ { "client.dll", "C_DOTA_PlayerResource", "m_bDirtyEstimatedMatchDuration" };
		inline netvar_t m_bDirtySelection = /* 0x2662, bool */ { "client.dll", "C_DOTA_PlayerResource", "m_bDirtySelection" };
		inline netvar_t m_bHasWorldTreesChanged = /* 0x2663, bool */ { "client.dll", "C_DOTA_PlayerResource", "m_bHasWorldTreesChanged" };
		inline netvar_t m_bWorldTreeModelsChanged = /* 0x2664, bool */ { "client.dll", "C_DOTA_PlayerResource", "m_bWorldTreeModelsChanged" };
		inline netvar_t m_bSwapWillingness = /* 0x2665, bool[24] */ { "client.dll", "C_DOTA_PlayerResource", "m_bSwapWillingness" };
		inline netvar_t m_hTeamCouriers = /* 0x26c0, CUtlVector< CHandle< C_DOTA_Unit_Courier > >[15] */ { "client.dll", "C_DOTA_PlayerResource", "m_hTeamCouriers" };
		inline netvar_t m_hPlayerCouriers = /* 0x2828, CUtlVector< CHandle< C_DOTA_Unit_Courier > >[64] */ { "client.dll", "C_DOTA_PlayerResource", "m_hPlayerCouriers" };
		inline netvar_t m_vecOnstageHomeTeams = /* 0x2e28, CUtlVector< uint32 > */ { "client.dll", "C_DOTA_PlayerResource", "m_vecOnstageHomeTeams" };
		inline netvar_t m_pPlayerIDToOnstageSlot = /* 0x2e40, PlayerSeatAssignment_t*[24] */ { "client.dll", "C_DOTA_PlayerResource", "m_pPlayerIDToOnstageSlot" };
		inline netvar_t m_vecOnstagePlayerSeats = /* 0x2f00, CUtlVector< PlayerSeatAssignment_t > */ { "client.dll", "C_DOTA_PlayerResource", "m_vecOnstagePlayerSeats" };
		inline netvar_t m_nEventNPCReplaced = /* 0x2f18, int32 */ { "client.dll", "C_DOTA_PlayerResource", "m_nEventNPCReplaced" };
		inline netvar_t m_nEventPlayerInfo = /* 0x2f1c, int32 */ { "client.dll", "C_DOTA_PlayerResource", "m_nEventPlayerInfo" };
		inline netvar_t m_nInventoryUpdated = /* 0x2f20, int32 */ { "client.dll", "C_DOTA_PlayerResource", "m_nInventoryUpdated" };
	}
	// client.dll
	// PlayerResourcePlayerTeamData_t
	namespace PlayerResourcePlayerTeamData_t {
		inline netvar_t m_ThreatLevelInfos = /* 0x30, C_UtlVectorEmbeddedNetworkVar< DOTAThreatLevelInfo_t > */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_ThreatLevelInfos" };
		inline netvar_t m_nSelectedHeroID = /* 0x80, HeroID_t */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_nSelectedHeroID" };
		inline netvar_t m_nSelectedHeroVariant = /* 0x84, HeroFacetID_t */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_nSelectedHeroVariant" };
		inline netvar_t m_iKills = /* 0x88, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iKills" };
		inline netvar_t m_iAssists = /* 0x8c, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iAssists" };
		inline netvar_t m_iDeaths = /* 0x90, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iDeaths" };
		inline netvar_t m_iStreak = /* 0x94, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iStreak" };
		inline netvar_t m_iLevel = /* 0x98, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iLevel" };
		inline netvar_t m_iCustomIntParam = /* 0x9c, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iCustomIntParam" };
		inline netvar_t m_iRespawnSeconds = /* 0xa0, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iRespawnSeconds" };
		inline netvar_t m_flLastBuybackTime = /* 0xa4, GameTime_t */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_flLastBuybackTime" };
		inline netvar_t m_iLastBuybackTime_Obsolete = /* 0xa8, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iLastBuybackTime_Obsolete" };
		inline netvar_t m_hSelectedHero = /* 0xac, CHandle< C_BaseEntity > */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_hSelectedHero" };
		inline netvar_t m_bAFK = /* 0xb0, bool */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bAFK" };
		inline netvar_t m_nSuggestedHeroes = /* 0xb4, HeroID_t[4] */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_nSuggestedHeroes" };
		inline netvar_t m_bBanSuggestedHeroes = /* 0xc4, bool[4] */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bBanSuggestedHeroes" };
		inline netvar_t m_unCompendiumLevel = /* 0xc8, uint16 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unCompendiumLevel" };
		inline netvar_t m_bCanRepick = /* 0xca, bool */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bCanRepick" };
		inline netvar_t m_bCanEarnRewards = /* 0xcb, bool */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bCanEarnRewards" };
		inline netvar_t m_bHasRandomed = /* 0xcc, bool */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bHasRandomed" };
		inline netvar_t m_nRandomedHeroID = /* 0xd0, HeroID_t */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_nRandomedHeroID" };
		inline netvar_t m_bBattleBonusActive = /* 0xd4, bool */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bBattleBonusActive" };
		inline netvar_t m_iBattleBonusRate = /* 0xd6, uint16 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iBattleBonusRate" };
		inline netvar_t m_iCustomBuybackCost = /* 0xd8, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iCustomBuybackCost" };
		inline netvar_t m_CustomPlayerColor = /* 0xdc, Color */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_CustomPlayerColor" };
		inline netvar_t m_bQualifiesForPAContractReward = /* 0xe0, bool */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bQualifiesForPAContractReward" };
		inline netvar_t m_bHasPredictedVictory = /* 0xe1, bool */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bHasPredictedVictory" };
		inline netvar_t m_UnitShareMasks = /* 0xe4, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_UnitShareMasks" };
		inline netvar_t m_iTeamSlot = /* 0xe8, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iTeamSlot" };
		inline netvar_t m_iBattleCupWinStreak = /* 0xec, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iBattleCupWinStreak" };
		inline netvar_t m_iBattleCupWinDate = /* 0xf0, uint64 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iBattleCupWinDate" };
		inline netvar_t m_iBattleCupSkillLevel = /* 0xf8, uint16 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iBattleCupSkillLevel" };
		inline netvar_t m_iBattleCupTeamID = /* 0xfc, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iBattleCupTeamID" };
		inline netvar_t m_iBattleCupTournamentID = /* 0x100, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iBattleCupTournamentID" };
		inline netvar_t m_iBattleCupDivision = /* 0x104, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iBattleCupDivision" };
		inline netvar_t m_flTeamFightParticipation = /* 0x108, float32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_flTeamFightParticipation" };
		inline netvar_t m_iFirstBloodClaimed = /* 0x10c, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iFirstBloodClaimed" };
		inline netvar_t m_iFirstBloodGiven = /* 0x110, int32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iFirstBloodGiven" };
		inline netvar_t m_unPickOrder = /* 0x114, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unPickOrder" };
		inline netvar_t m_flTimeOfLastSaluteSent = /* 0x118, GameTime_t */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_flTimeOfLastSaluteSent" };
		inline netvar_t m_vecPlayerEventData = /* 0x120, C_UtlVectorEmbeddedNetworkVar< PlayerResourcePlayerEventData_t > */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_vecPlayerEventData" };
		inline netvar_t m_unSelectedHeroBadgeXP = /* 0x170, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unSelectedHeroBadgeXP" };
		inline netvar_t m_iBountyRunes = /* 0x174, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iBountyRunes" };
		inline netvar_t m_iPowerRunes = /* 0x175, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iPowerRunes" };
		inline netvar_t m_iWaterRunes = /* 0x176, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iWaterRunes" };
		inline netvar_t m_iOutpostsCaptured = /* 0x177, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iOutpostsCaptured" };
		inline netvar_t m_unGuildTier = /* 0x178, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unGuildTier" };
		inline netvar_t m_unGuildLevel = /* 0x17a, uint16 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unGuildLevel" };
		inline netvar_t m_unGuildPrimaryColor = /* 0x17c, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unGuildPrimaryColor" };
		inline netvar_t m_unGuildSecondaryColor = /* 0x17d, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unGuildSecondaryColor" };
		inline netvar_t m_unGuildPattern = /* 0x17e, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unGuildPattern" };
		inline netvar_t m_unGuildLogo = /* 0x180, uint64 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unGuildLogo" };
		inline netvar_t m_unGuildFlags = /* 0x188, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unGuildFlags" };
		inline netvar_t m_bIsPartyGuild = /* 0x18c, bool */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bIsPartyGuild" };
		inline netvar_t m_unGuildID = /* 0x190, GuildID_t */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unGuildID" };
		inline netvar_t m_unHeroStickerDefIndex = /* 0x194, item_definition_index_t */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unHeroStickerDefIndex" };
		inline netvar_t m_eHeroStickerQuality = /* 0x198, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_eHeroStickerQuality" };
		inline netvar_t m_eLaneSelectionFlags = /* 0x199, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_eLaneSelectionFlags" };
		inline netvar_t m_nPlayerDraftPreferredRoles = /* 0x19a, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_nPlayerDraftPreferredRoles" };
		inline netvar_t m_nPlayerDraftPreferredTeam = /* 0x19b, int8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_nPlayerDraftPreferredTeam" };
		inline netvar_t m_nAvailableGifts = /* 0x19c, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_nAvailableGifts" };
		inline netvar_t m_unFowTeam = /* 0x19d, uint8 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_unFowTeam" };
		inline netvar_t m_iObsoleteEventPoints = /* 0x1a0, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteEventPoints" };
		inline netvar_t m_iObsoleteEventPremiumPoints = /* 0x1a4, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteEventPremiumPoints" };
		inline netvar_t m_iObsoleteEventWagerTokensRemaining = /* 0x1a8, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteEventWagerTokensRemaining" };
		inline netvar_t m_iObsoleteEventWagerTokensMax = /* 0x1ac, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteEventWagerTokensMax" };
		inline netvar_t m_iObsoleteEventEffectsMask = /* 0x1b0, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteEventEffectsMask" };
		inline netvar_t m_iObsoleteEventRanks = /* 0x1b4, uint16 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteEventRanks" };
		inline netvar_t m_bObsoleteIsEventOwned = /* 0x1b6, bool */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_bObsoleteIsEventOwned" };
		inline netvar_t m_iObsoleteRankWagersAvailable = /* 0x1b8, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteRankWagersAvailable" };
		inline netvar_t m_iObsoleteRankWagersMax = /* 0x1bc, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteRankWagersMax" };
		inline netvar_t m_iObsoleteEventPointAdjustmentsRemaining = /* 0x1c0, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteEventPointAdjustmentsRemaining" };
		inline netvar_t m_iObsoleteAvailableSalutes = /* 0x1c4, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteAvailableSalutes" };
		inline netvar_t m_iObsoleteSaluteAmounts = /* 0x1c8, uint32 */ { "client.dll", "PlayerResourcePlayerTeamData_t", "m_iObsoleteSaluteAmounts" };
	}
	// client.dll
	// C_GameRules
	namespace C_GameRules {
		inline netvar_t __m_pChainEntity = /* 0x8, CNetworkVarChainer */ { "client.dll", "C_GameRules", "__m_pChainEntity" };
		inline netvar_t m_nTotalPausedTicks = /* 0x30, int32 */ { "client.dll", "C_GameRules", "m_nTotalPausedTicks" };
		inline netvar_t m_nPauseStartTick = /* 0x34, int32 */ { "client.dll", "C_GameRules", "m_nPauseStartTick" };
		inline netvar_t m_bGamePaused = /* 0x38, bool */ { "client.dll", "C_GameRules", "m_bGamePaused" };
	}
	// client.dll
	// C_MultiplayRules < C_GameRules
	namespace C_MultiplayRules {
		using namespace C_GameRules;

	}
	// client.dll
	// C_DOTAGamerules < C_MultiplayRules < C_GameRules
	namespace C_DOTAGamerules {
		using namespace C_MultiplayRules;

		inline netvar_t m_nRuneCycle = /* 0x40, int32 */ { "client.dll", "C_DOTAGamerules", "m_nRuneCycle" };
		inline netvar_t m_iMiscHeroPickCounter = /* 0x44, int32 */ { "client.dll", "C_DOTAGamerules", "m_iMiscHeroPickCounter" };
		inline netvar_t m_hEndGameCinematicEntity = /* 0x48, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hEndGameCinematicEntity" };
		inline netvar_t m_hOverlayHealthBarUnit = /* 0x4c, CHandle< C_DOTA_BaseNPC > */ { "client.dll", "C_DOTAGamerules", "m_hOverlayHealthBarUnit" };
		inline netvar_t m_nOverlayHealthBarType = /* 0x50, int32 */ { "client.dll", "C_DOTAGamerules", "m_nOverlayHealthBarType" };
		inline netvar_t m_bIsInCinematicMode = /* 0x54, bool */ { "client.dll", "C_DOTAGamerules", "m_bIsInCinematicMode" };
		inline netvar_t m_bIsInClientSideCinematicMode = /* 0x55, bool */ { "client.dll", "C_DOTAGamerules", "m_bIsInClientSideCinematicMode" };
		inline netvar_t m_bFreeCourierMode = /* 0x56, bool */ { "client.dll", "C_DOTAGamerules", "m_bFreeCourierMode" };
		inline netvar_t m_nStartingGold = /* 0x58, int32 */ { "client.dll", "C_DOTAGamerules", "m_nStartingGold" };
		inline netvar_t m_nGoldPerTick = /* 0x5c, int32 */ { "client.dll", "C_DOTAGamerules", "m_nGoldPerTick" };
		inline netvar_t m_flGoldTickTime = /* 0x60, float32 */ { "client.dll", "C_DOTAGamerules", "m_flGoldTickTime" };
		inline netvar_t m_bItemWhiteListChanged = /* 0x64, bool */ { "client.dll", "C_DOTAGamerules", "m_bItemWhiteListChanged" };
		inline netvar_t m_bEnableSuggestAbilities = /* 0x65, bool */ { "client.dll", "C_DOTAGamerules", "m_bEnableSuggestAbilities" };
		inline netvar_t m_bEnableSuggestItems = /* 0x66, bool */ { "client.dll", "C_DOTAGamerules", "m_bEnableSuggestItems" };
		inline netvar_t m_unFanfareGoodGuys = /* 0x68, uint32 */ { "client.dll", "C_DOTAGamerules", "m_unFanfareGoodGuys" };
		inline netvar_t m_unFanfareBadGuys = /* 0x6c, uint32 */ { "client.dll", "C_DOTAGamerules", "m_unFanfareBadGuys" };
		inline netvar_t m_iMapType = /* 0x70, int32 */ { "client.dll", "C_DOTAGamerules", "m_iMapType" };
		inline netvar_t m_nServerGameState = /* 0x74, int32 */ { "client.dll", "C_DOTAGamerules", "m_nServerGameState" };
		inline netvar_t m_nServerHeroPickState = /* 0x78, DOTA_HeroPickState */ { "client.dll", "C_DOTAGamerules", "m_nServerHeroPickState" };
		inline netvar_t m_nGameState = /* 0x7c, int32 */ { "client.dll", "C_DOTAGamerules", "m_nGameState" };
		inline netvar_t m_nHeroPickState = /* 0x80, DOTA_HeroPickState */ { "client.dll", "C_DOTAGamerules", "m_nHeroPickState" };
		inline netvar_t m_bAlternateHeroGridsEnabled = /* 0x84, bool */ { "client.dll", "C_DOTAGamerules", "m_bAlternateHeroGridsEnabled" };
		inline netvar_t m_flStateTransitionTime = /* 0x88, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flStateTransitionTime" };
		inline netvar_t m_flOverride_dota_hero_selection_time = /* 0x8c, float32 */ { "client.dll", "C_DOTAGamerules", "m_flOverride_dota_hero_selection_time" };
		inline netvar_t m_flOverride_dota_pregame_time = /* 0x90, float32 */ { "client.dll", "C_DOTAGamerules", "m_flOverride_dota_pregame_time" };
		inline netvar_t m_flOverride_dota_postgame_time = /* 0x94, float32 */ { "client.dll", "C_DOTAGamerules", "m_flOverride_dota_postgame_time" };
		inline netvar_t m_flOverride_dota_strategy_time = /* 0x98, float32 */ { "client.dll", "C_DOTAGamerules", "m_flOverride_dota_strategy_time" };
		inline netvar_t m_flOverride_dota_team_showcase_duration = /* 0x9c, float32 */ { "client.dll", "C_DOTAGamerules", "m_flOverride_dota_team_showcase_duration" };
		inline netvar_t m_flOverride_dota_rune_spawn_time = /* 0xa0, float32 */ { "client.dll", "C_DOTAGamerules", "m_flOverride_dota_rune_spawn_time" };
		inline netvar_t m_flShowcaseTime = /* 0xa4, float32[15] */ { "client.dll", "C_DOTAGamerules", "m_flShowcaseTime" };
		inline netvar_t m_flNeutralInitialSpawnOffset = /* 0xe0, float32 */ { "client.dll", "C_DOTAGamerules", "m_flNeutralInitialSpawnOffset" };
		inline netvar_t m_iGameMode = /* 0xe4, int32 */ { "client.dll", "C_DOTAGamerules", "m_iGameMode" };
		inline netvar_t m_hGameModeEntity = /* 0xe8, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hGameModeEntity" };
		inline netvar_t m_hCustomHeroPickRulesEntity = /* 0xec, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hCustomHeroPickRulesEntity" };
		inline netvar_t m_flHeroPickStateTransitionTime = /* 0xf0, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flHeroPickStateTransitionTime" };
		inline netvar_t m_iPlayerIDsInControl = /* 0xf8, uint64 */ { "client.dll", "C_DOTAGamerules", "m_iPlayerIDsInControl" };
		inline netvar_t m_bSameHeroSelectionEnabled = /* 0x100, bool */ { "client.dll", "C_DOTAGamerules", "m_bSameHeroSelectionEnabled" };
		inline netvar_t m_bUseCustomHeroXPValue = /* 0x101, bool */ { "client.dll", "C_DOTAGamerules", "m_bUseCustomHeroXPValue" };
		inline netvar_t m_bUseBaseGoldBountyOnHeroes = /* 0x102, bool */ { "client.dll", "C_DOTAGamerules", "m_bUseBaseGoldBountyOnHeroes" };
		inline netvar_t m_bUseUniversalShopMode = /* 0x103, bool */ { "client.dll", "C_DOTAGamerules", "m_bUseUniversalShopMode" };
		inline netvar_t m_bHideKillMessageHeaders = /* 0x104, bool */ { "client.dll", "C_DOTAGamerules", "m_bHideKillMessageHeaders" };
		inline netvar_t m_flHeroMinimapIconScale = /* 0x108, float32 */ { "client.dll", "C_DOTAGamerules", "m_flHeroMinimapIconScale" };
		inline netvar_t m_flCreepMinimapIconScale = /* 0x10c, float32 */ { "client.dll", "C_DOTAGamerules", "m_flCreepMinimapIconScale" };
		inline netvar_t m_bCreepSpawningEnabled = /* 0x110, bool */ { "client.dll", "C_DOTAGamerules", "m_bCreepSpawningEnabled" };
		inline netvar_t m_flRuneMinimapIconScale = /* 0x114, float32 */ { "client.dll", "C_DOTAGamerules", "m_flRuneMinimapIconScale" };
		inline netvar_t m_CustomVictoryMessage = /* 0x118, char[256] */ { "client.dll", "C_DOTAGamerules", "m_CustomVictoryMessage" };
		inline netvar_t m_flCustomGameEndDelay = /* 0x218, float32 */ { "client.dll", "C_DOTAGamerules", "m_flCustomGameEndDelay" };
		inline netvar_t m_flCustomGameSetupAutoLaunchDelay = /* 0x21c, float32 */ { "client.dll", "C_DOTAGamerules", "m_flCustomGameSetupAutoLaunchDelay" };
		inline netvar_t m_flCustomGameSetupTimeout = /* 0x220, float32 */ { "client.dll", "C_DOTAGamerules", "m_flCustomGameSetupTimeout" };
		inline netvar_t m_flCustomVictoryMessageDuration = /* 0x224, float32 */ { "client.dll", "C_DOTAGamerules", "m_flCustomVictoryMessageDuration" };
		inline netvar_t m_flHeroSelectPenaltyTime = /* 0x228, float32 */ { "client.dll", "C_DOTAGamerules", "m_flHeroSelectPenaltyTime" };
		inline netvar_t m_bCustomGameSetupAutoLaunchEnabled = /* 0x22c, bool */ { "client.dll", "C_DOTAGamerules", "m_bCustomGameSetupAutoLaunchEnabled" };
		inline netvar_t m_bCustomGameTeamSelectionLocked = /* 0x22d, bool */ { "client.dll", "C_DOTAGamerules", "m_bCustomGameTeamSelectionLocked" };
		inline netvar_t m_bCustomGameEnablePickRules = /* 0x22e, bool */ { "client.dll", "C_DOTAGamerules", "m_bCustomGameEnablePickRules" };
		inline netvar_t m_bCustomGameAllowHeroPickMusic = /* 0x22f, bool */ { "client.dll", "C_DOTAGamerules", "m_bCustomGameAllowHeroPickMusic" };
		inline netvar_t m_bCustomGameAllowMusicAtGameStart = /* 0x230, bool */ { "client.dll", "C_DOTAGamerules", "m_bCustomGameAllowMusicAtGameStart" };
		inline netvar_t m_bCustomGameAllowBattleMusic = /* 0x231, bool */ { "client.dll", "C_DOTAGamerules", "m_bCustomGameAllowBattleMusic" };
		inline netvar_t m_bCustomGameDisableIK = /* 0x232, bool */ { "client.dll", "C_DOTAGamerules", "m_bCustomGameDisableIK" };
		inline netvar_t m_bCustomGameAllowSecondaryAbilitiesOnOtherUnits = /* 0x233, bool */ { "client.dll", "C_DOTAGamerules", "m_bCustomGameAllowSecondaryAbilitiesOnOtherUnits" };
		inline netvar_t m_iCMModePickBanOrder = /* 0x234, int32 */ { "client.dll", "C_DOTAGamerules", "m_iCMModePickBanOrder" };
		inline netvar_t m_iCDModePickBanOrder = /* 0x238, int32 */ { "client.dll", "C_DOTAGamerules", "m_iCDModePickBanOrder" };
		inline netvar_t m_nGGTeam = /* 0x23c, int32 */ { "client.dll", "C_DOTAGamerules", "m_nGGTeam" };
		inline netvar_t m_flGGEndsAtTime = /* 0x240, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flGGEndsAtTime" };
		inline netvar_t m_bGGCalled = /* 0x244, bool */ { "client.dll", "C_DOTAGamerules", "m_bGGCalled" };
		inline netvar_t m_bWhiteListEnabled = /* 0x245, bool */ { "client.dll", "C_DOTAGamerules", "m_bWhiteListEnabled" };
		inline netvar_t m_bItemWhiteList = /* 0x248, uint64[4] */ { "client.dll", "C_DOTAGamerules", "m_bItemWhiteList" };
		inline netvar_t m_nLastHitUIMode = /* 0x268, int32 */ { "client.dll", "C_DOTAGamerules", "m_nLastHitUIMode" };
		inline netvar_t m_bHUDTimerTutorialMode = /* 0x26c, bool */ { "client.dll", "C_DOTAGamerules", "m_bHUDTimerTutorialMode" };
		inline netvar_t m_HeroPickMiscTimer = /* 0x270, CountdownTimer */ { "client.dll", "C_DOTAGamerules", "m_HeroPickMiscTimer" };
		inline netvar_t m_ExtraTimeTimer = /* 0x288, CountdownTimer */ { "client.dll", "C_DOTAGamerules", "m_ExtraTimeTimer" };
		inline netvar_t m_fExtraTimeRemaining = /* 0x2a0, float32[2] */ { "client.dll", "C_DOTAGamerules", "m_fExtraTimeRemaining" };
		inline netvar_t m_bHeroRespawnEnabled = /* 0x2a8, bool */ { "client.dll", "C_DOTAGamerules", "m_bHeroRespawnEnabled" };
		inline netvar_t m_bIsRandomingEnabled = /* 0x2a9, bool */ { "client.dll", "C_DOTAGamerules", "m_bIsRandomingEnabled" };
		inline netvar_t m_iCaptainPlayerIDs = /* 0x2ac, PlayerID_t[2] */ { "client.dll", "C_DOTAGamerules", "m_iCaptainPlayerIDs" };
		inline netvar_t m_BannedHeroes = /* 0x2b4, HeroID_t[24] */ { "client.dll", "C_DOTAGamerules", "m_BannedHeroes" };
		inline netvar_t m_SelectedHeroes = /* 0x314, HeroID_t[24] */ { "client.dll", "C_DOTAGamerules", "m_SelectedHeroes" };
		inline netvar_t m_iActiveTeam = /* 0x374, int32 */ { "client.dll", "C_DOTAGamerules", "m_iActiveTeam" };
		inline netvar_t m_iStartingTeam = /* 0x378, int32 */ { "client.dll", "C_DOTAGamerules", "m_iStartingTeam" };
		inline netvar_t m_iPenaltyLevelRadiant = /* 0x37c, int32 */ { "client.dll", "C_DOTAGamerules", "m_iPenaltyLevelRadiant" };
		inline netvar_t m_iPenaltyLevelDire = /* 0x380, int32 */ { "client.dll", "C_DOTAGamerules", "m_iPenaltyLevelDire" };
		inline netvar_t m_bIsLoadingScenario = /* 0x384, bool */ { "client.dll", "C_DOTAGamerules", "m_bIsLoadingScenario" };
		inline netvar_t m_vecNewBannedHeroes = /* 0x388, C_NetworkUtlVectorBase< HeroID_t > */ { "client.dll", "C_DOTAGamerules", "m_vecNewBannedHeroes" };
		inline netvar_t m_vecWhitelistedHeroes = /* 0x3a0, C_NetworkUtlVectorBase< HeroID_t > */ { "client.dll", "C_DOTAGamerules", "m_vecWhitelistedHeroes" };
		inline netvar_t m_vecBlacklistedHeroes = /* 0x3b8, C_NetworkUtlVectorBase< HeroID_t > */ { "client.dll", "C_DOTAGamerules", "m_vecBlacklistedHeroes" };
		inline netvar_t m_bHideBlacklistedHeroes = /* 0x3d0, bool */ { "client.dll", "C_DOTAGamerules", "m_bHideBlacklistedHeroes" };
		inline netvar_t m_nSeriesType = /* 0x3d4, int32 */ { "client.dll", "C_DOTAGamerules", "m_nSeriesType" };
		inline netvar_t m_nRadiantSeriesWins = /* 0x3d8, int32 */ { "client.dll", "C_DOTAGamerules", "m_nRadiantSeriesWins" };
		inline netvar_t m_nDireSeriesWins = /* 0x3dc, int32 */ { "client.dll", "C_DOTAGamerules", "m_nDireSeriesWins" };
		inline netvar_t m_vecAvailableHerosPerPlayerID = /* 0x3e0, C_UtlVectorEmbeddedNetworkVar< CHeroesPerPlayer > */ { "client.dll", "C_DOTAGamerules", "m_vecAvailableHerosPerPlayerID" };
		inline netvar_t m_vecLockedHerosByPlayerID = /* 0x430, C_UtlVectorEmbeddedNetworkVar< CHeroesPerPlayer > */ { "client.dll", "C_DOTAGamerules", "m_vecLockedHerosByPlayerID" };
		inline netvar_t m_vecDisabledRandomHerosByPlayerID = /* 0x480, C_UtlVectorEmbeddedNetworkVar< CHeroesPerPlayer > */ { "client.dll", "C_DOTAGamerules", "m_vecDisabledRandomHerosByPlayerID" };
		inline netvar_t m_CustomGameForceSelectHero = /* 0x4d0, HeroID_t[24] */ { "client.dll", "C_DOTAGamerules", "m_CustomGameForceSelectHero" };
		inline netvar_t m_flGoldTime = /* 0x530, float32 */ { "client.dll", "C_DOTAGamerules", "m_flGoldTime" };
		inline netvar_t m_flXPTime = /* 0x534, float32 */ { "client.dll", "C_DOTAGamerules", "m_flXPTime" };
		inline netvar_t m_flCreepSpawntime = /* 0x538, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flCreepSpawntime" };
		inline netvar_t m_flAnnounceStartTime = /* 0x53c, float32 */ { "client.dll", "C_DOTAGamerules", "m_flAnnounceStartTime" };
		inline netvar_t m_iGoodTomeCount = /* 0x540, int32 */ { "client.dll", "C_DOTAGamerules", "m_iGoodTomeCount" };
		inline netvar_t m_iBadTomeCount = /* 0x544, int32 */ { "client.dll", "C_DOTAGamerules", "m_iBadTomeCount" };
		inline netvar_t m_flPreGameStartTime = /* 0x548, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flPreGameStartTime" };
		inline netvar_t m_flGameStartTime = /* 0x54c, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flGameStartTime" };
		inline netvar_t m_flGameEndTime = /* 0x550, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flGameEndTime" };
		inline netvar_t m_flGameLoadTime = /* 0x554, float32 */ { "client.dll", "C_DOTAGamerules", "m_flGameLoadTime" };
		inline netvar_t m_iCustomGameScore = /* 0x558, int32[2] */ { "client.dll", "C_DOTAGamerules", "m_iCustomGameScore" };
		inline netvar_t m_nCustomGameDifficulty = /* 0x560, int32 */ { "client.dll", "C_DOTAGamerules", "m_nCustomGameDifficulty" };
		inline netvar_t m_bEnemyModifiersEnabled = /* 0x564, bool */ { "client.dll", "C_DOTAGamerules", "m_bEnemyModifiersEnabled" };
		inline netvar_t m_iWaves = /* 0x568, int32 */ { "client.dll", "C_DOTAGamerules", "m_iWaves" };
		inline netvar_t m_iCreepUpgradeState = /* 0x56c, int32 */ { "client.dll", "C_DOTAGamerules", "m_iCreepUpgradeState" };
		inline netvar_t m_fGoodGlyphCooldown = /* 0x570, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_fGoodGlyphCooldown" };
		inline netvar_t m_fBadGlyphCooldown = /* 0x574, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_fBadGlyphCooldown" };
		inline netvar_t m_flGlyphCooldowns = /* 0x578, GameTime_t[15] */ { "client.dll", "C_DOTAGamerules", "m_flGlyphCooldowns" };
		inline netvar_t m_fGoodRadarCooldown = /* 0x5b4, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_fGoodRadarCooldown" };
		inline netvar_t m_fBadRadarCooldown = /* 0x5b8, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_fBadRadarCooldown" };
		inline netvar_t m_flRadarCooldowns = /* 0x5bc, GameTime_t[15] */ { "client.dll", "C_DOTAGamerules", "m_flRadarCooldowns" };
		inline netvar_t m_iGoodRadarCharges = /* 0x5f8, int32 */ { "client.dll", "C_DOTAGamerules", "m_iGoodRadarCharges" };
		inline netvar_t m_iBadRadarCharges = /* 0x5fc, int32 */ { "client.dll", "C_DOTAGamerules", "m_iBadRadarCharges" };
		inline netvar_t m_iRadarCharges = /* 0x600, int32[15] */ { "client.dll", "C_DOTAGamerules", "m_iRadarCharges" };
		inline netvar_t m_flOutpostTimes = /* 0x63c, float32[24] */ { "client.dll", "C_DOTAGamerules", "m_flOutpostTimes" };
		inline netvar_t m_bIsNightstalkerNight = /* 0x69c, bool */ { "client.dll", "C_DOTAGamerules", "m_bIsNightstalkerNight" };
		inline netvar_t m_bIsTemporaryNight = /* 0x69d, bool */ { "client.dll", "C_DOTAGamerules", "m_bIsTemporaryNight" };
		inline netvar_t m_bIsTemporaryDay = /* 0x69e, bool */ { "client.dll", "C_DOTAGamerules", "m_bIsTemporaryDay" };
		inline netvar_t m_nRiverType = /* 0x6a0, int32 */ { "client.dll", "C_DOTAGamerules", "m_nRiverType" };
		inline netvar_t m_nTeamFeaturedPlayerID = /* 0x6a4, PlayerID_t[15] */ { "client.dll", "C_DOTAGamerules", "m_nTeamFeaturedPlayerID" };
		inline netvar_t m_nTeamVersusScreen = /* 0x6e0, item_definition_index_t[15] */ { "client.dll", "C_DOTAGamerules", "m_nTeamVersusScreen" };
		inline netvar_t m_flGoldRedistributeTime = /* 0x71c, float32 */ { "client.dll", "C_DOTAGamerules", "m_flGoldRedistributeTime" };
		inline netvar_t m_nGoldToRedistribute = /* 0x720, int32[2] */ { "client.dll", "C_DOTAGamerules", "m_nGoldToRedistribute" };
		inline netvar_t m_flNextPreGameThink = /* 0x728, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flNextPreGameThink" };
		inline netvar_t m_flNextAllDraftGoldThink = /* 0x72c, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flNextAllDraftGoldThink" };
		inline netvar_t m_flTimeEnteredState = /* 0x730, float64 */ { "client.dll", "C_DOTAGamerules", "m_flTimeEnteredState" };
		inline netvar_t m_unRiverAccountID = /* 0x738, uint32 */ { "client.dll", "C_DOTAGamerules", "m_unRiverAccountID" };
		inline netvar_t m_ulRiverItemID = /* 0x740, itemid_t */ { "client.dll", "C_DOTAGamerules", "m_ulRiverItemID" };
		inline netvar_t m_vecItemStockInfo = /* 0x788, C_UtlVectorEmbeddedNetworkVar< CDOTA_ItemStockInfo > */ { "client.dll", "C_DOTAGamerules", "m_vecItemStockInfo" };
		inline netvar_t m_AssassinMiniGameNetData = /* 0x7d8, DOTA_AssassinMinigameNetworkState */ { "client.dll", "C_DOTAGamerules", "m_AssassinMiniGameNetData" };
		inline netvar_t m_nGameWinner = /* 0x7e8, int32 */ { "client.dll", "C_DOTAGamerules", "m_nGameWinner" };
		inline netvar_t m_unMatchID64 = /* 0x7f0, MatchID_t */ { "client.dll", "C_DOTAGamerules", "m_unMatchID64" };
		inline netvar_t m_bMatchSignoutComplete = /* 0x7f8, bool */ { "client.dll", "C_DOTAGamerules", "m_bMatchSignoutComplete" };
		inline netvar_t m_hSideShop1 = /* 0x7fc, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hSideShop1" };
		inline netvar_t m_hSideShop2 = /* 0x800, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hSideShop2" };
		inline netvar_t m_hSecretShop1 = /* 0x804, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hSecretShop1" };
		inline netvar_t m_hSecretShop2 = /* 0x808, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hSecretShop2" };
		inline netvar_t m_hTeamFountains = /* 0x80c, CHandle< C_BaseEntity >[15] */ { "client.dll", "C_DOTAGamerules", "m_hTeamFountains" };
		inline netvar_t m_hTeamForts = /* 0x848, CHandle< C_BaseEntity >[15] */ { "client.dll", "C_DOTAGamerules", "m_hTeamForts" };
		inline netvar_t m_hTeamShops = /* 0x884, CHandle< C_BaseEntity >[15] */ { "client.dll", "C_DOTAGamerules", "m_hTeamShops" };
		inline netvar_t m_hAnnouncerGood = /* 0x8c0, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hAnnouncerGood" };
		inline netvar_t m_hAnnouncerBad = /* 0x8c4, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hAnnouncerBad" };
		inline netvar_t m_hAnnouncerSpectator = /* 0x8c8, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hAnnouncerSpectator" };
		inline netvar_t m_hAnnouncerGood_KillingSpree = /* 0x8cc, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hAnnouncerGood_KillingSpree" };
		inline netvar_t m_hAnnouncerBad_KillingSpree = /* 0x8d0, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hAnnouncerBad_KillingSpree" };
		inline netvar_t m_hAnnouncerSpectator_KillingSpree = /* 0x8d4, CHandle< C_BaseEntity > */ { "client.dll", "C_DOTAGamerules", "m_hAnnouncerSpectator_KillingSpree" };
		inline netvar_t m_arrTier1TowerDestroyed = /* 0x8d8, bool[15] */ { "client.dll", "C_DOTAGamerules", "m_arrTier1TowerDestroyed" };
		inline netvar_t m_arrTier2TowerDestroyed = /* 0x8e7, bool[15] */ { "client.dll", "C_DOTAGamerules", "m_arrTier2TowerDestroyed" };
		inline netvar_t m_arrTier3TowerDestroyed = /* 0x8f6, bool[15] */ { "client.dll", "C_DOTAGamerules", "m_arrTier3TowerDestroyed" };
		inline netvar_t m_arrMeleeBarracksDestroyed = /* 0x905, bool[15] */ { "client.dll", "C_DOTAGamerules", "m_arrMeleeBarracksDestroyed" };
		inline netvar_t m_bTier1TowerDestroyed = /* 0x914, bool */ { "client.dll", "C_DOTAGamerules", "m_bTier1TowerDestroyed" };
		inline netvar_t m_bTier2TowerDestroyed = /* 0x915, bool */ { "client.dll", "C_DOTAGamerules", "m_bTier2TowerDestroyed" };
		inline netvar_t m_bTier3TowerDestroyed = /* 0x916, bool */ { "client.dll", "C_DOTAGamerules", "m_bTier3TowerDestroyed" };
		inline netvar_t m_bMeleeBarracksDestroyed = /* 0x917, bool */ { "client.dll", "C_DOTAGamerules", "m_bMeleeBarracksDestroyed" };
		inline netvar_t m_fTimeOfDay = /* 0x918, float32 */ { "client.dll", "C_DOTAGamerules", "m_fTimeOfDay" };
		inline netvar_t m_iNetTimeOfDay = /* 0x91c, int32 */ { "client.dll", "C_DOTAGamerules", "m_iNetTimeOfDay" };
		inline netvar_t m_flDaytimeStart = /* 0x920, float32 */ { "client.dll", "C_DOTAGamerules", "m_flDaytimeStart" };
		inline netvar_t m_flNighttimeStart = /* 0x924, float32 */ { "client.dll", "C_DOTAGamerules", "m_flNighttimeStart" };
		inline netvar_t m_nLoadedPlayers = /* 0x928, int32 */ { "client.dll", "C_DOTAGamerules", "m_nLoadedPlayers" };
		inline netvar_t m_nExpectedPlayers = /* 0x92c, int32 */ { "client.dll", "C_DOTAGamerules", "m_nExpectedPlayers" };
		inline netvar_t m_iMinimapDebugGridState = /* 0x938, int32 */ { "client.dll", "C_DOTAGamerules", "m_iMinimapDebugGridState" };
		inline netvar_t m_iFoWFrameNumber = /* 0x9b8, int32 */ { "client.dll", "C_DOTAGamerules", "m_iFoWFrameNumber" };
		inline netvar_t m_vWeatherWindDirection = /* 0x9bc, Vector */ { "client.dll", "C_DOTAGamerules", "m_vWeatherWindDirection" };
		inline netvar_t m_vecRequiredPlaytestHeroesStorage = /* 0x9c8, HeroID_t[24] */ { "client.dll", "C_DOTAGamerules", "m_vecRequiredPlaytestHeroesStorage" };
		inline netvar_t m_vecRecommendedPlaytestHeroesStorage = /* 0xa28, HeroID_t[24] */ { "client.dll", "C_DOTAGamerules", "m_vecRecommendedPlaytestHeroesStorage" };
		inline netvar_t m_bAllRequiredPlaytestHeroesPicked = /* 0xa88, bool */ { "client.dll", "C_DOTAGamerules", "m_bAllRequiredPlaytestHeroesPicked" };
		inline netvar_t m_nLastPlaytestPickPhase = /* 0xa8c, int32 */ { "client.dll", "C_DOTAGamerules", "m_nLastPlaytestPickPhase" };
		inline netvar_t m_nCustomGameFowTeamCount = /* 0xa90, int32 */ { "client.dll", "C_DOTAGamerules", "m_nCustomGameFowTeamCount" };
		inline netvar_t m_bUseAlternateABRules = /* 0xa94, bool */ { "client.dll", "C_DOTAGamerules", "m_bUseAlternateABRules" };
		inline netvar_t m_bLobbyIsAssociatedWithGame = /* 0xa95, bool */ { "client.dll", "C_DOTAGamerules", "m_bLobbyIsAssociatedWithGame" };
		inline netvar_t m_BotDebugTimer = /* 0xa98, CountdownTimer */ { "client.dll", "C_DOTAGamerules", "m_BotDebugTimer" };
		inline netvar_t m_BotDebugPushLane = /* 0xab0, uint8[18] */ { "client.dll", "C_DOTAGamerules", "m_BotDebugPushLane" };
		inline netvar_t m_BotDebugDefendLane = /* 0xac2, uint8[18] */ { "client.dll", "C_DOTAGamerules", "m_BotDebugDefendLane" };
		inline netvar_t m_BotDebugFarmLane = /* 0xad4, uint8[6] */ { "client.dll", "C_DOTAGamerules", "m_BotDebugFarmLane" };
		inline netvar_t m_BotDebugRoam = /* 0xada, uint8[8] */ { "client.dll", "C_DOTAGamerules", "m_BotDebugRoam" };
		inline netvar_t m_hBotDebugRoamTarget = /* 0xae4, CHandle< C_BaseEntity >[2] */ { "client.dll", "C_DOTAGamerules", "m_hBotDebugRoamTarget" };
		inline netvar_t m_BotDebugRoshan = /* 0xaec, uint8[2] */ { "client.dll", "C_DOTAGamerules", "m_BotDebugRoshan" };
		inline netvar_t m_nRoshanRespawnPhase = /* 0xaf0, ERoshanSpawnPhase */ { "client.dll", "C_DOTAGamerules", "m_nRoshanRespawnPhase" };
		inline netvar_t m_flRoshanRespawnPhaseEndTime = /* 0xaf4, GameTime_t */ { "client.dll", "C_DOTAGamerules", "m_flRoshanRespawnPhaseEndTime" };
		inline netvar_t m_AbilityDraftAbilities = /* 0xaf8, C_UtlVectorEmbeddedNetworkVar< CDOTA_AbilityDraftAbilityState > */ { "client.dll", "C_DOTAGamerules", "m_AbilityDraftAbilities" };
		inline netvar_t m_bAbilityDraftCurrentPlayerHasPicked = /* 0xb48, bool */ { "client.dll", "C_DOTAGamerules", "m_bAbilityDraftCurrentPlayerHasPicked" };
		inline netvar_t m_nAbilityDraftPlayerTracker = /* 0xb4c, int32 */ { "client.dll", "C_DOTAGamerules", "m_nAbilityDraftPlayerTracker" };
		inline netvar_t m_nAbilityDraftRoundNumber = /* 0xb50, int32 */ { "client.dll", "C_DOTAGamerules", "m_nAbilityDraftRoundNumber" };
		inline netvar_t m_nAbilityDraftAdvanceSteps = /* 0xb54, int32 */ { "client.dll", "C_DOTAGamerules", "m_nAbilityDraftAdvanceSteps" };
		inline netvar_t m_nAbilityDraftPhase = /* 0xb58, int32 */ { "client.dll", "C_DOTAGamerules", "m_nAbilityDraftPhase" };
		inline netvar_t m_bIsPlayerDraft = /* 0xb5c, bool */ { "client.dll", "C_DOTAGamerules", "m_bIsPlayerDraft" };
		inline netvar_t m_ePlayerDraftState = /* 0xb60, DOTA_PlayerDraftState */ { "client.dll", "C_DOTAGamerules", "m_ePlayerDraftState" };
		inline netvar_t m_vecPlayerDraftPickOrder = /* 0xb68, C_NetworkUtlVectorBase< uint8 > */ { "client.dll", "C_DOTAGamerules", "m_vecPlayerDraftPickOrder" };
		inline netvar_t m_nPlayerDraftPick = /* 0xb80, int32 */ { "client.dll", "C_DOTAGamerules", "m_nPlayerDraftPick" };
		inline netvar_t m_nPlayerDraftActiveTeam = /* 0xb84, int32 */ { "client.dll", "C_DOTAGamerules", "m_nPlayerDraftActiveTeam" };
		inline netvar_t m_flPlayerDraftTimeBank = /* 0xb88, float32[2] */ { "client.dll", "C_DOTAGamerules", "m_flPlayerDraftTimeBank" };
		inline netvar_t m_vecARDMHeroes = /* 0xb90, CUtlVector< KeyValues* >[2] */ { "client.dll", "C_DOTAGamerules", "m_vecARDMHeroes" };
		inline netvar_t m_nARDMHeroesPrecached = /* 0xbc0, int32 */ { "client.dll", "C_DOTAGamerules", "m_nARDMHeroesPrecached" };
		inline netvar_t m_fLastARDMPrecache = /* 0xbc4, float32 */ { "client.dll", "C_DOTAGamerules", "m_fLastARDMPrecache" };
		inline netvar_t m_nAllDraftPhase = /* 0xbc8, int32 */ { "client.dll", "C_DOTAGamerules", "m_nAllDraftPhase" };
		inline netvar_t m_bAllDraftRadiantFirst = /* 0xbcc, bool */ { "client.dll", "C_DOTAGamerules", "m_bAllDraftRadiantFirst" };
		inline netvar_t m_bAllowOverrideVPK = /* 0xbcd, bool */ { "client.dll", "C_DOTAGamerules", "m_bAllowOverrideVPK" };
		inline netvar_t m_nARDMHeroesRemaining = /* 0xbd0, int32[2] */ { "client.dll", "C_DOTAGamerules", "m_nARDMHeroesRemaining" };
		inline netvar_t m_bUpdateHeroStatues = /* 0xbd8, bool */ { "client.dll", "C_DOTAGamerules", "m_bUpdateHeroStatues" };
		inline netvar_t m_bExperimentalGameplay = /* 0xbf0, bool */ { "client.dll", "C_DOTAGamerules", "m_bExperimentalGameplay" };
		inline netvar_t m_lobbyType = /* 0xc38, uint32 */ { "client.dll", "C_DOTAGamerules", "m_lobbyType" };
		inline netvar_t m_lobbyLeagueID = /* 0xc3c, LeagueID_t */ { "client.dll", "C_DOTAGamerules", "m_lobbyLeagueID" };
		inline netvar_t m_lobbyGameName = /* 0xc40, char[256] */ { "client.dll", "C_DOTAGamerules", "m_lobbyGameName" };
		inline netvar_t m_vecHeroStatueLiked = /* 0xd40, C_UtlVectorEmbeddedNetworkVar< CHeroStatueLiked > */ { "client.dll", "C_DOTAGamerules", "m_vecHeroStatueLiked" };
		inline netvar_t m_CustomGameTeamMaxPlayers = /* 0xd90, int32[15] */ { "client.dll", "C_DOTAGamerules", "m_CustomGameTeamMaxPlayers" };
		inline netvar_t m_iMutations = /* 0xdcc, int32[5] */ { "client.dll", "C_DOTAGamerules", "m_iMutations" };
		inline netvar_t m_vecIngameEvents = /* 0xde0, C_NetworkUtlVectorBase< CHandle< C_IngameEvent_Base > > */ { "client.dll", "C_DOTAGamerules", "m_vecIngameEvents" };
		inline netvar_t m_nPrimaryIngameEventIndex = /* 0xdf8, int8 */ { "client.dll", "C_DOTAGamerules", "m_nPrimaryIngameEventIndex" };
		inline netvar_t m_hObsoleteIngameEvent = /* 0xdfc, CHandle< C_IngameEvent_Base > */ { "client.dll", "C_DOTAGamerules", "m_hObsoleteIngameEvent" };
		inline netvar_t m_nOfrendaPledges = /* 0xe00, uint32 */ { "client.dll", "C_DOTAGamerules", "m_nOfrendaPledges" };
		inline netvar_t m_nRadiantOfrendas = /* 0xe04, uint32 */ { "client.dll", "C_DOTAGamerules", "m_nRadiantOfrendas" };
		inline netvar_t m_nDireOfrendas = /* 0xe08, uint32 */ { "client.dll", "C_DOTAGamerules", "m_nDireOfrendas" };
		inline netvar_t m_bOfrendaEnabled = /* 0xe0c, bool */ { "client.dll", "C_DOTAGamerules", "m_bOfrendaEnabled" };
		inline netvar_t m_NeutralSpawnBoxes = /* 0xe40, C_NetworkUtlVectorBase< NeutralSpawnBoxes_t > */ { "client.dll", "C_DOTAGamerules", "m_NeutralSpawnBoxes" };
		inline netvar_t m_RegionTriggerBoxes = /* 0xe58, C_NetworkUtlVectorBase< RegionTriggerBoxes_t > */ { "client.dll", "C_DOTAGamerules", "m_RegionTriggerBoxes" };
		inline netvar_t m_flLastPerfSampleTime = /* 0x1e28, float64 */ { "client.dll", "C_DOTAGamerules", "m_flLastPerfSampleTime" };
		inline netvar_t m_flLastPerfSampleSendTime = /* 0x1e30, float64 */ { "client.dll", "C_DOTAGamerules", "m_flLastPerfSampleSendTime" };
		inline netvar_t m_bDidSeeStrategyTime = /* 0x1e38, bool */ { "client.dll", "C_DOTAGamerules", "m_bDidSeeStrategyTime" };
		inline netvar_t m_flLastUnfocusedSleepTime = /* 0xab90, float64 */ { "client.dll", "C_DOTAGamerules", "m_flLastUnfocusedSleepTime" };
	}
	// client.dll
	// C_DOTA_Item_Rune < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_DOTA_Item_Rune {
		using namespace CBaseAnimatingActivity;

		inline netvar_t m_iRuneType = /* 0x800, int32 */ { "client.dll", "C_DOTA_Item_Rune", "m_iRuneType" };
		inline netvar_t m_flRuneTime = /* 0x804, float32 */ { "client.dll", "C_DOTA_Item_Rune", "m_flRuneTime" };
		inline netvar_t m_nMapLocationTeam = /* 0x808, int32 */ { "client.dll", "C_DOTA_Item_Rune", "m_nMapLocationTeam" };
		inline netvar_t m_szLocation = /* 0x80c, char[512] */ { "client.dll", "C_DOTA_Item_Rune", "m_szLocation" };
		inline netvar_t m_iOldRuneType = /* 0xa0c, int32 */ { "client.dll", "C_DOTA_Item_Rune", "m_iOldRuneType" };
		inline netvar_t m_bShowingTooltip = /* 0xa10, bool */ { "client.dll", "C_DOTA_Item_Rune", "m_bShowingTooltip" };
	}
	// !GlobalTypes
	// GameTime_t
	namespace GameTime_t {
		inline netvar_t m_Value = /* 0x0, float32 */ { "!GlobalTypes", "GameTime_t", "m_Value" };
	}
	// client.dll
	// C_DOTA_Item_EmptyBottle < C_DOTA_Item < C_DOTABaseAbility < C_BaseEntity < CEntityInstance
	namespace C_DOTA_Item_EmptyBottle {
		using namespace C_DOTA_Item;

		inline netvar_t m_iStoredRuneType = /* 0x678, int32 */ { "client.dll", "C_DOTA_Item_EmptyBottle", "m_iStoredRuneType" };
		inline netvar_t m_fStoredRuneTime = /* 0x67c, GameTime_t */ { "client.dll", "C_DOTA_Item_EmptyBottle", "m_fStoredRuneTime" };
		inline netvar_t rune_expire_time = /* 0x680, float32 */ { "client.dll", "C_DOTA_Item_EmptyBottle", "rune_expire_time" };
	}
	// client.dll
	// C_GameRulesProxy < C_BaseEntity < CEntityInstance
	namespace C_GameRulesProxy {
		using namespace C_BaseEntity;

	}
	// client.dll
	// C_DOTAGamerulesProxy < C_GameRulesProxy < C_BaseEntity < CEntityInstance
	namespace C_DOTAGamerulesProxy {
		using namespace C_GameRulesProxy;

		inline netvar_t m_pGameRules = /* 0x538, C_DOTAGamerules* */ { "client.dll", "C_DOTAGamerulesProxy", "m_pGameRules" };
	}
	// client.dll
	// C_EconWearable < C_EconEntity < C_BaseFlex < C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_EconWearable {
		using namespace C_EconEntity;

	}
	// !GlobalTypes
	// CParticleFunctionRenderer < CParticleFunction
	namespace CParticleFunctionRenderer {
		using namespace CParticleFunction;

		inline netvar_t VisibilityInputs = /* 0x1c0, CParticleVisibilityInputs */ { "!GlobalTypes", "CParticleFunctionRenderer", "VisibilityInputs" };
		inline netvar_t m_bCannotBeRefracted = /* 0x208, bool */ { "!GlobalTypes", "CParticleFunctionRenderer", "m_bCannotBeRefracted" };
		inline netvar_t m_bSkipRenderingOnMobile = /* 0x209, bool */ { "!GlobalTypes", "CParticleFunctionRenderer", "m_bSkipRenderingOnMobile" };
	}
	// !GlobalTypes
	// CBaseRendererSource2 < CParticleFunctionRenderer < CParticleFunction
	namespace CBaseRendererSource2 {
		using namespace CParticleFunctionRenderer;

		inline netvar_t m_flRadiusScale = /* 0x210, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flRadiusScale" };
		inline netvar_t m_flAlphaScale = /* 0x370, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flAlphaScale" };
		inline netvar_t m_flRollScale = /* 0x4d0, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flRollScale" };
		inline netvar_t m_nAlpha2Field = /* 0x630, ParticleAttributeIndex_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nAlpha2Field" };
		inline netvar_t m_vecColorScale = /* 0x638, CParticleCollectionRendererVecInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_vecColorScale" };
		inline netvar_t m_nColorBlendType = /* 0xcb0, ParticleColorBlendType_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nColorBlendType" };
		inline netvar_t m_nShaderType = /* 0xcb4, SpriteCardShaderType_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nShaderType" };
		inline netvar_t m_strShaderOverride = /* 0xcb8, CUtlString */ { "!GlobalTypes", "CBaseRendererSource2", "m_strShaderOverride" };
		inline netvar_t m_flCenterXOffset = /* 0xcc0, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flCenterXOffset" };
		inline netvar_t m_flCenterYOffset = /* 0xe20, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flCenterYOffset" };
		inline netvar_t m_flBumpStrength = /* 0xf80, float32 */ { "!GlobalTypes", "CBaseRendererSource2", "m_flBumpStrength" };
		inline netvar_t m_nCropTextureOverride = /* 0xf84, ParticleSequenceCropOverride_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nCropTextureOverride" };
		inline netvar_t m_vecTexturesInput = /* 0xf88, CUtlVector< TextureGroup_t > */ { "!GlobalTypes", "CBaseRendererSource2", "m_vecTexturesInput" };
		inline netvar_t m_flAnimationRate = /* 0xfa0, float32 */ { "!GlobalTypes", "CBaseRendererSource2", "m_flAnimationRate" };
		inline netvar_t m_nAnimationType = /* 0xfa4, AnimationType_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nAnimationType" };
		inline netvar_t m_bAnimateInFPS = /* 0xfa8, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bAnimateInFPS" };
		inline netvar_t m_flSelfIllumAmount = /* 0xfb0, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flSelfIllumAmount" };
		inline netvar_t m_flDiffuseAmount = /* 0x1110, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flDiffuseAmount" };
		inline netvar_t m_flDiffuseClamp = /* 0x1270, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flDiffuseClamp" };
		inline netvar_t m_nLightingControlPoint = /* 0x13d0, int32 */ { "!GlobalTypes", "CBaseRendererSource2", "m_nLightingControlPoint" };
		inline netvar_t m_nSelfIllumPerParticle = /* 0x13d4, ParticleAttributeIndex_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nSelfIllumPerParticle" };
		inline netvar_t m_nOutputBlendMode = /* 0x13d8, ParticleOutputBlendMode_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nOutputBlendMode" };
		inline netvar_t m_bGammaCorrectVertexColors = /* 0x13dc, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bGammaCorrectVertexColors" };
		inline netvar_t m_bSaturateColorPreAlphaBlend = /* 0x13dd, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bSaturateColorPreAlphaBlend" };
		inline netvar_t m_flAddSelfAmount = /* 0x13e0, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flAddSelfAmount" };
		inline netvar_t m_flDesaturation = /* 0x1540, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flDesaturation" };
		inline netvar_t m_flOverbrightFactor = /* 0x16a0, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flOverbrightFactor" };
		inline netvar_t m_nHSVShiftControlPoint = /* 0x1800, int32 */ { "!GlobalTypes", "CBaseRendererSource2", "m_nHSVShiftControlPoint" };
		inline netvar_t m_nFogType = /* 0x1804, ParticleFogType_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nFogType" };
		inline netvar_t m_flFogAmount = /* 0x1808, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flFogAmount" };
		inline netvar_t m_bTintByFOW = /* 0x1968, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bTintByFOW" };
		inline netvar_t m_bTintByGlobalLight = /* 0x1969, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bTintByGlobalLight" };
		inline netvar_t m_nPerParticleAlphaReference = /* 0x196c, SpriteCardPerParticleScale_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nPerParticleAlphaReference" };
		inline netvar_t m_nPerParticleAlphaRefWindow = /* 0x1970, SpriteCardPerParticleScale_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nPerParticleAlphaRefWindow" };
		inline netvar_t m_nAlphaReferenceType = /* 0x1974, ParticleAlphaReferenceType_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nAlphaReferenceType" };
		inline netvar_t m_flAlphaReferenceSoftness = /* 0x1978, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flAlphaReferenceSoftness" };
		inline netvar_t m_flSourceAlphaValueToMapToZero = /* 0x1ad8, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flSourceAlphaValueToMapToZero" };
		inline netvar_t m_flSourceAlphaValueToMapToOne = /* 0x1c38, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flSourceAlphaValueToMapToOne" };
		inline netvar_t m_bRefract = /* 0x1d98, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bRefract" };
		inline netvar_t m_bRefractSolid = /* 0x1d99, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bRefractSolid" };
		inline netvar_t m_flRefractAmount = /* 0x1da0, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flRefractAmount" };
		inline netvar_t m_nRefractBlurRadius = /* 0x1f00, int32 */ { "!GlobalTypes", "CBaseRendererSource2", "m_nRefractBlurRadius" };
		inline netvar_t m_nRefractBlurType = /* 0x1f04, BlurFilterType_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nRefractBlurType" };
		inline netvar_t m_bOnlyRenderInEffectsBloomPass = /* 0x1f08, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bOnlyRenderInEffectsBloomPass" };
		inline netvar_t m_bOnlyRenderInEffectsWaterPass = /* 0x1f09, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bOnlyRenderInEffectsWaterPass" };
		inline netvar_t m_bUseMixedResolutionRendering = /* 0x1f0a, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bUseMixedResolutionRendering" };
		inline netvar_t m_bOnlyRenderInEffecsGameOverlay = /* 0x1f0b, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bOnlyRenderInEffecsGameOverlay" };
		inline netvar_t m_stencilTestID = /* 0x1f0c, char[128] */ { "!GlobalTypes", "CBaseRendererSource2", "m_stencilTestID" };
		inline netvar_t m_bStencilTestExclude = /* 0x1f8c, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bStencilTestExclude" };
		inline netvar_t m_stencilWriteID = /* 0x1f8d, char[128] */ { "!GlobalTypes", "CBaseRendererSource2", "m_stencilWriteID" };
		inline netvar_t m_bWriteStencilOnDepthPass = /* 0x200d, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bWriteStencilOnDepthPass" };
		inline netvar_t m_bWriteStencilOnDepthFail = /* 0x200e, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bWriteStencilOnDepthFail" };
		inline netvar_t m_bReverseZBuffering = /* 0x200f, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bReverseZBuffering" };
		inline netvar_t m_bDisableZBuffering = /* 0x2010, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bDisableZBuffering" };
		inline netvar_t m_nFeatheringMode = /* 0x2014, ParticleDepthFeatheringMode_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nFeatheringMode" };
		inline netvar_t m_flFeatheringMinDist = /* 0x2018, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flFeatheringMinDist" };
		inline netvar_t m_flFeatheringMaxDist = /* 0x2178, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flFeatheringMaxDist" };
		inline netvar_t m_flFeatheringFilter = /* 0x22d8, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flFeatheringFilter" };
		inline netvar_t m_flDepthBias = /* 0x2438, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "CBaseRendererSource2", "m_flDepthBias" };
		inline netvar_t m_nSortMethod = /* 0x2598, ParticleSortingChoiceList_t */ { "!GlobalTypes", "CBaseRendererSource2", "m_nSortMethod" };
		inline netvar_t m_bBlendFramesSeq0 = /* 0x259c, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bBlendFramesSeq0" };
		inline netvar_t m_bMaxLuminanceBlendingSequence0 = /* 0x259d, bool */ { "!GlobalTypes", "CBaseRendererSource2", "m_bMaxLuminanceBlendingSequence0" };
	}
	// !GlobalTypes
	// C_OP_RenderSprites < CBaseRendererSource2 < CParticleFunctionRenderer < CParticleFunction
	namespace C_OP_RenderSprites {
		using namespace CBaseRendererSource2;

		inline netvar_t m_nSequenceOverride = /* 0x27e8, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "C_OP_RenderSprites", "m_nSequenceOverride" };
		inline netvar_t m_nOrientationType = /* 0x2948, ParticleOrientationChoiceList_t */ { "!GlobalTypes", "C_OP_RenderSprites", "m_nOrientationType" };
		inline netvar_t m_nOrientationControlPoint = /* 0x294c, int32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_nOrientationControlPoint" };
		inline netvar_t m_bUseYawWithNormalAligned = /* 0x2950, bool */ { "!GlobalTypes", "C_OP_RenderSprites", "m_bUseYawWithNormalAligned" };
		inline netvar_t m_flMinSize = /* 0x2958, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flMinSize" };
		inline netvar_t m_flMaxSize = /* 0x2ab8, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flMaxSize" };
		inline netvar_t m_flAlphaAdjustWithSizeAdjust = /* 0x2c18, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flAlphaAdjustWithSizeAdjust" };
		inline netvar_t m_flStartFadeSize = /* 0x2d78, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flStartFadeSize" };
		inline netvar_t m_flEndFadeSize = /* 0x2ed8, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flEndFadeSize" };
		inline netvar_t m_flStartFadeDot = /* 0x3038, float32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flStartFadeDot" };
		inline netvar_t m_flEndFadeDot = /* 0x303c, float32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flEndFadeDot" };
		inline netvar_t m_bDistanceAlpha = /* 0x3040, bool */ { "!GlobalTypes", "C_OP_RenderSprites", "m_bDistanceAlpha" };
		inline netvar_t m_bSoftEdges = /* 0x3041, bool */ { "!GlobalTypes", "C_OP_RenderSprites", "m_bSoftEdges" };
		inline netvar_t m_flEdgeSoftnessStart = /* 0x3044, float32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flEdgeSoftnessStart" };
		inline netvar_t m_flEdgeSoftnessEnd = /* 0x3048, float32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flEdgeSoftnessEnd" };
		inline netvar_t m_bOutline = /* 0x304c, bool */ { "!GlobalTypes", "C_OP_RenderSprites", "m_bOutline" };
		inline netvar_t m_OutlineColor = /* 0x304d, Color */ { "!GlobalTypes", "C_OP_RenderSprites", "m_OutlineColor" };
		inline netvar_t m_nOutlineAlpha = /* 0x3054, int32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_nOutlineAlpha" };
		inline netvar_t m_flOutlineStart0 = /* 0x3058, float32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flOutlineStart0" };
		inline netvar_t m_flOutlineStart1 = /* 0x305c, float32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flOutlineStart1" };
		inline netvar_t m_flOutlineEnd0 = /* 0x3060, float32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flOutlineEnd0" };
		inline netvar_t m_flOutlineEnd1 = /* 0x3064, float32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flOutlineEnd1" };
		inline netvar_t m_nLightingMode = /* 0x3068, ParticleLightingQuality_t */ { "!GlobalTypes", "C_OP_RenderSprites", "m_nLightingMode" };
		inline netvar_t m_flLightingTessellation = /* 0x3070, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flLightingTessellation" };
		inline netvar_t m_flLightingDirectionality = /* 0x31d0, CParticleCollectionRendererFloatInput */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flLightingDirectionality" };
		inline netvar_t m_bParticleShadows = /* 0x3330, bool */ { "!GlobalTypes", "C_OP_RenderSprites", "m_bParticleShadows" };
		inline netvar_t m_flShadowDensity = /* 0x3334, float32 */ { "!GlobalTypes", "C_OP_RenderSprites", "m_flShadowDensity" };
		inline netvar_t m_replicationParameters = /* 0x3338, CReplicationParameters */ { "!GlobalTypes", "C_OP_RenderSprites", "m_replicationParameters" };
	}
	// client.dll
	// C_DOTAWearableItem < C_EconWearable < C_EconEntity < C_BaseFlex < C_BaseAnimatingOverlay < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_DOTAWearableItem {
		using namespace C_EconWearable;

		inline netvar_t m_bOwnerModelChanged = /* 0xb18, bool */ { "client.dll", "C_DOTAWearableItem", "m_bOwnerModelChanged" };
		inline netvar_t m_bIsGeneratingEconItem = /* 0xb19, bool */ { "client.dll", "C_DOTAWearableItem", "m_bIsGeneratingEconItem" };
		inline netvar_t m_bIsItemVisibleOnGeneratedEntity = /* 0xb1a, bool */ { "client.dll", "C_DOTAWearableItem", "m_bIsItemVisibleOnGeneratedEntity" };
		inline netvar_t m_bHideWearableButDrawAdditionalWearables = /* 0xb1b, bool */ { "client.dll", "C_DOTAWearableItem", "m_bHideWearableButDrawAdditionalWearables" };
		inline netvar_t m_hAdditionalWearable = /* 0xb1c, CHandle< CBaseAnimatingActivity > */ { "client.dll", "C_DOTAWearableItem", "m_hAdditionalWearable" };
		inline netvar_t m_hDrawWearable = /* 0xb20, CHandle< C_DOTAWearableItem > */ { "client.dll", "C_DOTAWearableItem", "m_hDrawWearable" };
		inline netvar_t m_bHiddenByCombiner = /* 0xb24, bool */ { "client.dll", "C_DOTAWearableItem", "m_bHiddenByCombiner" };
		inline netvar_t m_bHiddenByEquipmentSlot = /* 0xb25, bool */ { "client.dll", "C_DOTAWearableItem", "m_bHiddenByEquipmentSlot" };
		inline netvar_t m_bIsPortrait = /* 0xb26, bool */ { "client.dll", "C_DOTAWearableItem", "m_bIsPortrait" };
		inline netvar_t m_fZDelta = /* 0xb28, float32 */ { "client.dll", "C_DOTAWearableItem", "m_fZDelta" };
		inline netvar_t m_combinerMaterialOverrideList = /* 0xb30, CUtlVector< CStrongHandleCopyable< InfoForResourceTypeIMaterial2 > > */ { "client.dll", "C_DOTAWearableItem", "m_combinerMaterialOverrideList" };
		inline netvar_t m_bCombinerMaterialOverrideListChanged = /* 0xb50, bool */ { "client.dll", "C_DOTAWearableItem", "m_bCombinerMaterialOverrideListChanged" };
		inline netvar_t m_bRubickFollower = /* 0xb51, bool */ { "client.dll", "C_DOTAWearableItem", "m_bRubickFollower" };
		inline netvar_t m_bHasPlayerModel = /* 0xb52, bool */ { "client.dll", "C_DOTAWearableItem", "m_bHasPlayerModel" };
	}
	// !GlobalTypes
	// IEconItemInterface
	namespace IEconItemInterface {
	}
	// client.dll
	// C_EconItemView < IEconItemInterface
	namespace C_EconItemView {
		using namespace IEconItemInterface;

		inline netvar_t m_iItemDefinitionIndex = /* 0x8, item_definition_index_t */ { "client.dll", "C_EconItemView", "m_iItemDefinitionIndex" };
		inline netvar_t m_iEntityQuality = /* 0xc, int32 */ { "client.dll", "C_EconItemView", "m_iEntityQuality" };
		inline netvar_t m_iEntityLevel = /* 0x10, uint32 */ { "client.dll", "C_EconItemView", "m_iEntityLevel" };
		inline netvar_t m_iItemID = /* 0x18, itemid_t */ { "client.dll", "C_EconItemView", "m_iItemID" };
		inline netvar_t m_iAccountID = /* 0x20, uint32 */ { "client.dll", "C_EconItemView", "m_iAccountID" };
		inline netvar_t m_iInventoryPosition = /* 0x24, uint32 */ { "client.dll", "C_EconItemView", "m_iInventoryPosition" };
		inline netvar_t m_bInitialized = /* 0x30, bool */ { "client.dll", "C_EconItemView", "m_bInitialized" };
		inline netvar_t m_nOverrideStyle = /* 0x31, style_index_t */ { "client.dll", "C_EconItemView", "m_nOverrideStyle" };
		inline netvar_t m_bIsStoreItem = /* 0x32, bool */ { "client.dll", "C_EconItemView", "m_bIsStoreItem" };
		inline netvar_t m_bIsTradeItem = /* 0x33, bool */ { "client.dll", "C_EconItemView", "m_bIsTradeItem" };
		inline netvar_t m_bHasComputedAttachedParticles = /* 0x34, bool */ { "client.dll", "C_EconItemView", "m_bHasComputedAttachedParticles" };
		inline netvar_t m_bHasAttachedParticles = /* 0x35, bool */ { "client.dll", "C_EconItemView", "m_bHasAttachedParticles" };
		inline netvar_t m_iEntityQuantity = /* 0x38, int32 */ { "client.dll", "C_EconItemView", "m_iEntityQuantity" };
		inline netvar_t m_unClientFlags = /* 0x3c, uint8 */ { "client.dll", "C_EconItemView", "m_unClientFlags" };
		inline netvar_t m_unOverrideOrigin = /* 0x40, eEconItemOrigin */ { "client.dll", "C_EconItemView", "m_unOverrideOrigin" };
		inline netvar_t m_AttributeList = /* 0x58, CAttributeList */ { "client.dll", "C_EconItemView", "m_AttributeList" };
	}
	// client.dll
	// C_DOTA_Item_Physical < CBaseAnimatingActivity < C_BaseModelEntity < C_BaseEntity < CEntityInstance
	namespace C_DOTA_Item_Physical {
		using namespace CBaseAnimatingActivity;

		inline netvar_t m_hItem = /* 0x800, CHandle< C_DOTA_Item > */ { "client.dll", "C_DOTA_Item_Physical", "m_hItem" };
		inline netvar_t m_bIsLowPriorityHoverItem = /* 0x804, bool */ { "client.dll", "C_DOTA_Item_Physical", "m_bIsLowPriorityHoverItem" };
		inline netvar_t m_hOldItem = /* 0x808, CHandle< C_DOTA_Item > */ { "client.dll", "C_DOTA_Item_Physical", "m_hOldItem" };
		inline netvar_t m_pszParticleName = /* 0x810, char* */ { "client.dll", "C_DOTA_Item_Physical", "m_pszParticleName" };
		inline netvar_t m_nFXIndex = /* 0x818, ParticleIndex_t */ { "client.dll", "C_DOTA_Item_Physical", "m_nFXIndex" };
		inline netvar_t m_bShowingTooltip = /* 0x81c, bool */ { "client.dll", "C_DOTA_Item_Physical", "m_bShowingTooltip" };
		inline netvar_t m_bShowingSimpleTooltip = /* 0x81d, bool */ { "client.dll", "C_DOTA_Item_Physical", "m_bShowingSimpleTooltip" };
	}
	// !GlobalTypes
	// CHorizontalMotionController
	namespace CHorizontalMotionController {
	}
	// !GlobalTypes
	// CDOTA_Buff < CHorizontalMotionController
	namespace CDOTA_Buff {
		using namespace CHorizontalMotionController;

		inline netvar_t m_bIsAura = /* 0x0, bitfield:1 */ { "!GlobalTypes", "CDOTA_Buff", "m_bIsAura" };
		inline netvar_t m_name = /* 0x28, CUtlSymbolLarge */ { "!GlobalTypes", "CDOTA_Buff", "m_name" };
		inline netvar_t m_iszModifierAura = /* 0x38, CUtlSymbolLarge */ { "!GlobalTypes", "CDOTA_Buff", "m_iszModifierAura" };
		inline netvar_t m_iSerialNumber = /* 0x48, int32 */ { "!GlobalTypes", "CDOTA_Buff", "m_iSerialNumber" };
		inline netvar_t m_iStringIndex = /* 0x4c, int32 */ { "!GlobalTypes", "CDOTA_Buff", "m_iStringIndex" };
		inline netvar_t m_iIndex = /* 0x50, int32 */ { "!GlobalTypes", "CDOTA_Buff", "m_iIndex" };
		inline netvar_t m_flCreationTime = /* 0x54, GameTime_t */ { "!GlobalTypes", "CDOTA_Buff", "m_flCreationTime" };
		inline netvar_t m_iCreationFrame = /* 0x58, int32 */ { "!GlobalTypes", "CDOTA_Buff", "m_iCreationFrame" };
		inline netvar_t m_flLastAppliedTime = /* 0x5c, GameTime_t */ { "!GlobalTypes", "CDOTA_Buff", "m_flLastAppliedTime" };
		inline netvar_t m_flDuration = /* 0x60, float32 */ { "!GlobalTypes", "CDOTA_Buff", "m_flDuration" };
		inline netvar_t m_flDieTime = /* 0x64, GameTime_t */ { "!GlobalTypes", "CDOTA_Buff", "m_flDieTime" };
		inline netvar_t m_hCaster = /* 0x68, CHandle< CBaseEntity > */ { "!GlobalTypes", "CDOTA_Buff", "m_hCaster" };
		inline netvar_t m_hAbility = /* 0x6c, CHandle< CBaseEntity > */ { "!GlobalTypes", "CDOTA_Buff", "m_hAbility" };
		inline netvar_t m_hParent = /* 0x70, CHandle< CBaseEntity > */ { "!GlobalTypes", "CDOTA_Buff", "m_hParent" };
		inline netvar_t m_hAuraOwner = /* 0x74, CHandle< CBaseEntity > */ { "!GlobalTypes", "CDOTA_Buff", "m_hAuraOwner" };
		inline netvar_t m_iStackCount = /* 0x78, int32 */ { "!GlobalTypes", "CDOTA_Buff", "m_iStackCount" };
		inline netvar_t m_iAuraSearchTeam = /* 0x7c, int16 */ { "!GlobalTypes", "CDOTA_Buff", "m_iAuraSearchTeam" };
		inline netvar_t m_iAuraSearchType = /* 0x7e, int16 */ { "!GlobalTypes", "CDOTA_Buff", "m_iAuraSearchType" };
		inline netvar_t m_iAuraSearchFlags = /* 0x80, int32 */ { "!GlobalTypes", "CDOTA_Buff", "m_iAuraSearchFlags" };
		inline netvar_t m_iAuraRadius = /* 0x84, int32 */ { "!GlobalTypes", "CDOTA_Buff", "m_iAuraRadius" };
		inline netvar_t m_iTeam = /* 0x88, int32 */ { "!GlobalTypes", "CDOTA_Buff", "m_iTeam" };
		inline netvar_t m_iAttributes = /* 0x8c, int32 */ { "!GlobalTypes", "CDOTA_Buff", "m_iAttributes" };
		inline netvar_t m_iPaddingToMakeSchemaHappy2 = /* 0x90, int16 */ { "!GlobalTypes", "CDOTA_Buff", "m_iPaddingToMakeSchemaHappy2" };
		inline netvar_t m_bPurgedDestroy = /* 0x93, bool */ { "!GlobalTypes", "CDOTA_Buff", "m_bPurgedDestroy" };
		inline netvar_t m_flPreviousTick = /* 0x94, GameTime_t */ { "!GlobalTypes", "CDOTA_Buff", "m_flPreviousTick" };
		inline netvar_t m_flThinkInterval = /* 0x98, float32 */ { "!GlobalTypes", "CDOTA_Buff", "m_flThinkInterval" };
		inline netvar_t m_flThinkTimeAccumulator = /* 0x9c, float32 */ { "!GlobalTypes", "CDOTA_Buff", "m_flThinkTimeAccumulator" };
		inline netvar_t m_iParticles = /* 0xa0, CUtlVector< CDOTA_BuffParticle > */ { "!GlobalTypes", "CDOTA_Buff", "m_iParticles" };
		inline netvar_t m_hAuraUnits = /* 0xb8, CUtlVector< CHandle< CBaseEntity > > */ { "!GlobalTypes", "CDOTA_Buff", "m_hAuraUnits" };
		inline netvar_t m_hScriptScope = /* 0x16e0, HSCRIPT */ { "!GlobalTypes", "CDOTA_Buff", "m_hScriptScope" };
	}
	// !GlobalTypes
	// IParticleEffect
	namespace IParticleEffect {
	}
	// !GlobalTypes
	// CNewParticleEffect < IParticleEffect
	namespace CNewParticleEffect {
		using namespace IParticleEffect;

		inline netvar_t m_bDontRemove = /* 0x0, bitfield:1 */ { "!GlobalTypes", "CNewParticleEffect", "m_bDontRemove" };
		inline netvar_t m_pNext = /* 0x10, CNewParticleEffect* */ { "!GlobalTypes", "CNewParticleEffect", "m_pNext" };
		inline netvar_t m_pPrev = /* 0x18, CNewParticleEffect* */ { "!GlobalTypes", "CNewParticleEffect", "m_pPrev" };
		inline netvar_t m_pParticles = /* 0x20, IParticleCollection* */ { "!GlobalTypes", "CNewParticleEffect", "m_pParticles" };
		inline netvar_t m_pDebugName = /* 0x28, char* */ { "!GlobalTypes", "CNewParticleEffect", "m_pDebugName" };
		inline netvar_t m_vSortOrigin = /* 0x40, Vector */ { "!GlobalTypes", "CNewParticleEffect", "m_vSortOrigin" };
		inline netvar_t m_flScale = /* 0x4c, float32 */ { "!GlobalTypes", "CNewParticleEffect", "m_flScale" };
		inline netvar_t m_hOwner = /* 0x50, PARTICLE_EHANDLE__* */ { "!GlobalTypes", "CNewParticleEffect", "m_hOwner" };
		inline netvar_t m_pOwningParticleProperty = /* 0x58, CParticleProperty* */ { "!GlobalTypes", "CNewParticleEffect", "m_pOwningParticleProperty" };
		inline netvar_t m_flFreezeTransitionStart = /* 0x70, float32 */ { "!GlobalTypes", "CNewParticleEffect", "m_flFreezeTransitionStart" };
		inline netvar_t m_flFreezeTransitionDuration = /* 0x74, float32 */ { "!GlobalTypes", "CNewParticleEffect", "m_flFreezeTransitionDuration" };
		inline netvar_t m_flFreezeTransitionOverride = /* 0x78, float32 */ { "!GlobalTypes", "CNewParticleEffect", "m_flFreezeTransitionOverride" };
		inline netvar_t m_bFreezeTransitionActive = /* 0x7c, bool */ { "!GlobalTypes", "CNewParticleEffect", "m_bFreezeTransitionActive" };
		inline netvar_t m_bFreezeTargetState = /* 0x7d, bool */ { "!GlobalTypes", "CNewParticleEffect", "m_bFreezeTargetState" };
		inline netvar_t m_bCanFreeze = /* 0x7e, bool */ { "!GlobalTypes", "CNewParticleEffect", "m_bCanFreeze" };
		inline netvar_t m_LastMin = /* 0x80, Vector */ { "!GlobalTypes", "CNewParticleEffect", "m_LastMin" };
		inline netvar_t m_LastMax = /* 0x8c, Vector */ { "!GlobalTypes", "CNewParticleEffect", "m_LastMax" };
		inline netvar_t m_nSplitScreenUser = /* 0x98, CSplitScreenSlot */ { "!GlobalTypes", "CNewParticleEffect", "m_nSplitScreenUser" };
		inline netvar_t m_vecAggregationCenter = /* 0x9c, Vector */ { "!GlobalTypes", "CNewParticleEffect", "m_vecAggregationCenter" };
		inline netvar_t m_RefCount = /* 0xc0, int32 */ { "!GlobalTypes", "CNewParticleEffect", "m_RefCount" };
	}
	// !GlobalTypes
	// CParticleVecInput < CParticleInput
	namespace CParticleVecInput {
		using namespace CParticleInput;

		inline netvar_t m_nType = /* 0x10, ParticleVecType_t */ { "!GlobalTypes", "CParticleVecInput", "m_nType" };
		inline netvar_t m_vLiteralValue = /* 0x14, Vector */ { "!GlobalTypes", "CParticleVecInput", "m_vLiteralValue" };
		inline netvar_t m_LiteralColor = /* 0x20, Color */ { "!GlobalTypes", "CParticleVecInput", "m_LiteralColor" };
		inline netvar_t m_NamedValue = /* 0x28, CParticleNamedValueRef */ { "!GlobalTypes", "CParticleVecInput", "m_NamedValue" };
		inline netvar_t m_bFollowNamedValue = /* 0x68, bool */ { "!GlobalTypes", "CParticleVecInput", "m_bFollowNamedValue" };
		inline netvar_t m_nVectorAttribute = /* 0x6c, ParticleAttributeIndex_t */ { "!GlobalTypes", "CParticleVecInput", "m_nVectorAttribute" };
		inline netvar_t m_vVectorAttributeScale = /* 0x70, Vector */ { "!GlobalTypes", "CParticleVecInput", "m_vVectorAttributeScale" };
		inline netvar_t m_nControlPoint = /* 0x7c, int32 */ { "!GlobalTypes", "CParticleVecInput", "m_nControlPoint" };
		inline netvar_t m_nDeltaControlPoint = /* 0x80, int32 */ { "!GlobalTypes", "CParticleVecInput", "m_nDeltaControlPoint" };
		inline netvar_t m_vCPValueScale = /* 0x84, Vector */ { "!GlobalTypes", "CParticleVecInput", "m_vCPValueScale" };
		inline netvar_t m_vCPRelativePosition = /* 0x90, Vector */ { "!GlobalTypes", "CParticleVecInput", "m_vCPRelativePosition" };
		inline netvar_t m_vCPRelativeDir = /* 0x9c, Vector */ { "!GlobalTypes", "CParticleVecInput", "m_vCPRelativeDir" };
		inline netvar_t m_FloatComponentX = /* 0xa8, CParticleFloatInput */ { "!GlobalTypes", "CParticleVecInput", "m_FloatComponentX" };
		inline netvar_t m_FloatComponentY = /* 0x208, CParticleFloatInput */ { "!GlobalTypes", "CParticleVecInput", "m_FloatComponentY" };
		inline netvar_t m_FloatComponentZ = /* 0x368, CParticleFloatInput */ { "!GlobalTypes", "CParticleVecInput", "m_FloatComponentZ" };
		inline netvar_t m_FloatInterp = /* 0x4c8, CParticleFloatInput */ { "!GlobalTypes", "CParticleVecInput", "m_FloatInterp" };
		inline netvar_t m_flInterpInput0 = /* 0x628, float32 */ { "!GlobalTypes", "CParticleVecInput", "m_flInterpInput0" };
		inline netvar_t m_flInterpInput1 = /* 0x62c, float32 */ { "!GlobalTypes", "CParticleVecInput", "m_flInterpInput1" };
		inline netvar_t m_vInterpOutput0 = /* 0x630, Vector */ { "!GlobalTypes", "CParticleVecInput", "m_vInterpOutput0" };
		inline netvar_t m_vInterpOutput1 = /* 0x63c, Vector */ { "!GlobalTypes", "CParticleVecInput", "m_vInterpOutput1" };
		inline netvar_t m_Gradient = /* 0x648, CColorGradient */ { "!GlobalTypes", "CParticleVecInput", "m_Gradient" };
		inline netvar_t m_vRandomMin = /* 0x660, Vector */ { "!GlobalTypes", "CParticleVecInput", "m_vRandomMin" };
		inline netvar_t m_vRandomMax = /* 0x66c, Vector */ { "!GlobalTypes", "CParticleVecInput", "m_vRandomMax" };
	}
	// !GlobalTypes
	// IParticleSystemDefinition
	namespace IParticleSystemDefinition {
	}
	// !GlobalTypes
	// CParticleSystemDefinition < IParticleSystemDefinition
	namespace CParticleSystemDefinition {
		using namespace IParticleSystemDefinition;

		inline netvar_t m_nBehaviorVersion = /* 0x8, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nBehaviorVersion" };
		inline netvar_t m_PreEmissionOperators = /* 0x10, CUtlVector< CParticleFunctionPreEmission* > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_PreEmissionOperators" };
		inline netvar_t m_Emitters = /* 0x28, CUtlVector< CParticleFunctionEmitter* > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_Emitters" };
		inline netvar_t m_Initializers = /* 0x40, CUtlVector< CParticleFunctionInitializer* > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_Initializers" };
		inline netvar_t m_Operators = /* 0x58, CUtlVector< CParticleFunctionOperator* > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_Operators" };
		inline netvar_t m_ForceGenerators = /* 0x70, CUtlVector< CParticleFunctionForce* > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_ForceGenerators" };
		inline netvar_t m_Constraints = /* 0x88, CUtlVector< CParticleFunctionConstraint* > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_Constraints" };
		inline netvar_t m_Renderers = /* 0xa0, CUtlVector< CParticleFunctionRenderer* > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_Renderers" };
		inline netvar_t m_Children = /* 0xb8, CUtlVector< ParticleChildrenInfo_t > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_Children" };
		inline netvar_t m_nFirstMultipleOverride_BackwardCompat = /* 0x178, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nFirstMultipleOverride_BackwardCompat" };
		inline netvar_t m_nInitialParticles = /* 0x210, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nInitialParticles" };
		inline netvar_t m_nMaxParticles = /* 0x214, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nMaxParticles" };
		inline netvar_t m_nGroupID = /* 0x218, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nGroupID" };
		inline netvar_t m_BoundingBoxMin = /* 0x21c, Vector */ { "!GlobalTypes", "CParticleSystemDefinition", "m_BoundingBoxMin" };
		inline netvar_t m_BoundingBoxMax = /* 0x228, Vector */ { "!GlobalTypes", "CParticleSystemDefinition", "m_BoundingBoxMax" };
		inline netvar_t m_flDepthSortBias = /* 0x234, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flDepthSortBias" };
		inline netvar_t m_nSortOverridePositionCP = /* 0x238, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nSortOverridePositionCP" };
		inline netvar_t m_bInfiniteBounds = /* 0x23c, bool */ { "!GlobalTypes", "CParticleSystemDefinition", "m_bInfiniteBounds" };
		inline netvar_t m_bEnableNamedValues = /* 0x23d, bool */ { "!GlobalTypes", "CParticleSystemDefinition", "m_bEnableNamedValues" };
		inline netvar_t m_NamedValueDomain = /* 0x240, CUtlString */ { "!GlobalTypes", "CParticleSystemDefinition", "m_NamedValueDomain" };
		inline netvar_t m_NamedValueLocals = /* 0x248, CUtlVector< ParticleNamedValueSource_t* > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_NamedValueLocals" };
		inline netvar_t m_ConstantColor = /* 0x260, Color */ { "!GlobalTypes", "CParticleSystemDefinition", "m_ConstantColor" };
		inline netvar_t m_ConstantNormal = /* 0x264, Vector */ { "!GlobalTypes", "CParticleSystemDefinition", "m_ConstantNormal" };
		inline netvar_t m_flConstantRadius = /* 0x270, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flConstantRadius" };
		inline netvar_t m_flConstantRotation = /* 0x274, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flConstantRotation" };
		inline netvar_t m_flConstantRotationSpeed = /* 0x278, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flConstantRotationSpeed" };
		inline netvar_t m_flConstantLifespan = /* 0x27c, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flConstantLifespan" };
		inline netvar_t m_nConstantSequenceNumber = /* 0x280, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nConstantSequenceNumber" };
		inline netvar_t m_nConstantSequenceNumber1 = /* 0x284, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nConstantSequenceNumber1" };
		inline netvar_t m_nSnapshotControlPoint = /* 0x288, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nSnapshotControlPoint" };
		inline netvar_t m_hSnapshot = /* 0x290, CStrongHandle< InfoForResourceTypeIParticleSnapshot > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_hSnapshot" };
		inline netvar_t m_pszCullReplacementName = /* 0x298, CStrongHandle< InfoForResourceTypeIParticleSystemDefinition > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_pszCullReplacementName" };
		inline netvar_t m_flCullRadius = /* 0x2a0, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flCullRadius" };
		inline netvar_t m_flCullFillCost = /* 0x2a4, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flCullFillCost" };
		inline netvar_t m_nCullControlPoint = /* 0x2a8, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nCullControlPoint" };
		inline netvar_t m_hFallback = /* 0x2b0, CStrongHandle< InfoForResourceTypeIParticleSystemDefinition > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_hFallback" };
		inline netvar_t m_nFallbackMaxCount = /* 0x2b8, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nFallbackMaxCount" };
		inline netvar_t m_hLowViolenceDef = /* 0x2c0, CStrongHandle< InfoForResourceTypeIParticleSystemDefinition > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_hLowViolenceDef" };
		inline netvar_t m_hReferenceReplacement = /* 0x2c8, CStrongHandle< InfoForResourceTypeIParticleSystemDefinition > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_hReferenceReplacement" };
		inline netvar_t m_flPreSimulationTime = /* 0x2d0, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flPreSimulationTime" };
		inline netvar_t m_flStopSimulationAfterTime = /* 0x2d4, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flStopSimulationAfterTime" };
		inline netvar_t m_flMaximumTimeStep = /* 0x2d8, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flMaximumTimeStep" };
		inline netvar_t m_flMaximumSimTime = /* 0x2dc, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flMaximumSimTime" };
		inline netvar_t m_flMinimumSimTime = /* 0x2e0, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flMinimumSimTime" };
		inline netvar_t m_flMinimumTimeStep = /* 0x2e4, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flMinimumTimeStep" };
		inline netvar_t m_nMinimumFrames = /* 0x2e8, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nMinimumFrames" };
		inline netvar_t m_nMinCPULevel = /* 0x2ec, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nMinCPULevel" };
		inline netvar_t m_nMinGPULevel = /* 0x2f0, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nMinGPULevel" };
		inline netvar_t m_flNoDrawTimeToGoToSleep = /* 0x2f4, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flNoDrawTimeToGoToSleep" };
		inline netvar_t m_flMaxDrawDistance = /* 0x2f8, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flMaxDrawDistance" };
		inline netvar_t m_flStartFadeDistance = /* 0x2fc, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flStartFadeDistance" };
		inline netvar_t m_flMaxCreationDistance = /* 0x300, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flMaxCreationDistance" };
		inline netvar_t m_nAggregationMinAvailableParticles = /* 0x304, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nAggregationMinAvailableParticles" };
		inline netvar_t m_flAggregateRadius = /* 0x308, float32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_flAggregateRadius" };
		inline netvar_t m_bShouldBatch = /* 0x30c, bool */ { "!GlobalTypes", "CParticleSystemDefinition", "m_bShouldBatch" };
		inline netvar_t m_bShouldHitboxesFallbackToRenderBounds = /* 0x30d, bool */ { "!GlobalTypes", "CParticleSystemDefinition", "m_bShouldHitboxesFallbackToRenderBounds" };
		inline netvar_t m_bShouldHitboxesFallbackToSnapshot = /* 0x30e, bool */ { "!GlobalTypes", "CParticleSystemDefinition", "m_bShouldHitboxesFallbackToSnapshot" };
		inline netvar_t m_bShouldHitboxesFallbackToCollisionHulls = /* 0x30f, bool */ { "!GlobalTypes", "CParticleSystemDefinition", "m_bShouldHitboxesFallbackToCollisionHulls" };
		inline netvar_t m_nViewModelEffect = /* 0x310, InheritableBoolType_t */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nViewModelEffect" };
		inline netvar_t m_bScreenSpaceEffect = /* 0x314, bool */ { "!GlobalTypes", "CParticleSystemDefinition", "m_bScreenSpaceEffect" };
		inline netvar_t m_pszTargetLayerID = /* 0x318, CUtlSymbolLarge */ { "!GlobalTypes", "CParticleSystemDefinition", "m_pszTargetLayerID" };
		inline netvar_t m_nSkipRenderControlPoint = /* 0x320, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nSkipRenderControlPoint" };
		inline netvar_t m_nAllowRenderControlPoint = /* 0x324, int32 */ { "!GlobalTypes", "CParticleSystemDefinition", "m_nAllowRenderControlPoint" };
		inline netvar_t m_bShouldSort = /* 0x328, bool */ { "!GlobalTypes", "CParticleSystemDefinition", "m_bShouldSort" };
		inline netvar_t m_controlPointConfigurations = /* 0x370, CUtlVector< ParticleControlPointConfiguration_t > */ { "!GlobalTypes", "CParticleSystemDefinition", "m_controlPointConfigurations" };
	}
}