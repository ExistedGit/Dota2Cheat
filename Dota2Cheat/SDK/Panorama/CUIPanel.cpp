#include "../../CheatSDK/Globals/include.h"
#include "CUIPanel.h"

void CUIPanel::AddClass(const char* class_) {
	CallVFunc<135>(Interfaces::UIEngine->MakeSymbol(class_));
}

void CUIPanel::RemoveClass(uint16_t class_) {
	CallVFunc<138, void>(class_);
}

void CUIPanel::RemoveClass(const char* class_) {
	RemoveClass(Interfaces::UIEngine->MakeSymbol(class_));
}
