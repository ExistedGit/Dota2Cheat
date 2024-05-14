#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../Base/Vector.h"

// Reversed by Morphling
class CRenderGameSystem : public VClass {
public:
	auto WorldToProjectionMatrix() {
		return GetVFunc(VMI::CRenderGameSystem::WorldToProjectionMatrix).Call<float*>(0);
	}

	// Used as substitute of WorldToScreen
	// Basically some complex math involving the world-to-projection matrix
	bool GetVectorInScreenSpace(const Vector& point, Vector2D& screen);
};