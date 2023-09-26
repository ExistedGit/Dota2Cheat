#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../Base/Vector.h"

// Reversed by Morphling
// Used as substitute of WorldToScreen
class CRenderGameSystem : VClass {
public:
	auto WorldToProjectionMatrix() {
		return CallVFunc<53, float*>(0); // last vfunc
	}

	// Basically some complex math involving the WorldToProjection matrix
	bool GetVectorInScreenSpace(const Vector& point, Vector2D& screen);
};