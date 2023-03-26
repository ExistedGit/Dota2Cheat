#pragma once
#include "../Base/NormalClass.h"
#include "../Base/VClass.h"

class CResourceSystemResource : public NormalClass {
public:
    const char* GetName() {
        uintptr_t* Base = MemberInline<uintptr_t>(0x8);
        if (!Base)
            return nullptr;
        return *(const char**)(*Base);
    }
};

class CResourceSystem : VClass {
public:
    CResourceSystemResource* FindResourceById(uintptr_t ResourceId_t) {
        return CallVFunc<32, CResourceSystemResource*>(ResourceId_t, 0);
    }

    const char* GetResourceName(uintptr_t ResourceId_t) {
        CResourceSystemResource* Resource = FindResourceById(ResourceId_t);
        if (!Resource)
            return nullptr;

        return Resource->GetName();
    }
};