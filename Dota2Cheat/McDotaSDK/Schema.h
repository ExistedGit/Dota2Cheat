#pragma once

#include <cstdint>
#include <array>

struct CSchemaClassBinding {
    CSchemaClassBinding* parent;
    const char* binaryName; // ex: C_World
    const char* className; // ex: client
    void *classInfoOldSynthesized;
    void *classInfoN;
    void *thisModuleBindingPointer;
    void *pSchemaType;

    static void BindLua(sol::state& lua) {
        sol::usertype<CSchemaClassBinding> type = lua.new_usertype<CSchemaClassBinding>("CSchemaClassBinding");
        type["parent"] = &CSchemaClassBinding::parent;
        type["binaryName"] = &CSchemaClassBinding::binaryName; // ex: C_World
        type["className"] = &CSchemaClassBinding::className; // ex: client
        type["classInfoOldSynthesized"] = &CSchemaClassBinding::classInfoOldSynthesized;
        type["classInfoN"] = &CSchemaClassBinding::classInfoN;
        type["thisModuleBindingPointer"] = &CSchemaClassBinding::thisModuleBindingPointer;
        type["pSchemaType"] = &CSchemaClassBinding::pSchemaType;
    }
};


struct Netvar // Size: 0x138
{
    const char *netvarName; // ex: m_nMinGPULevel
    void *_pad[2]; // 0x8
    uint32_t offset; // 0x18
    uint32_t something; // 0x1C
    void *_pad2[10];
    const char *typeName; // ex: uint8
    void *_pad3[5];
};

struct NetvarWrapper
{
    Netvar *netVar;
    char sevenByteSwag[7];
} ;

struct SchemaRecvTable {
    const char *varName;
    void *_pad;
    int32_t numOfVars;
    int32_t something;
    NetvarWrapper *netVarsArray;
};