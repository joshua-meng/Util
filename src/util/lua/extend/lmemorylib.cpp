#include "lmemorylib.hpp"
#include "util/luaextend.hpp"

namespace util
{

const std::string kByteMemoryHandle = "Memory*";

typedef Memory<char> ByteMemory;

static int createMemory(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "create",
        luaGetTop(plua_state) > 0 && LuaNumber == luaGetType(plua_state, 1), "create memory with incorrect size");

    size_t memory_size = luaGetInteger(plua_state, 1);    
    LuaObject<ByteMemory>* p = luaNewEmptyObject<ByteMemory>(plua_state, kByteMemoryHandle);
    p->setData(new ByteMemory(memory_size));

    return 1;
}

static int destroy(lua_State* plua_state)
{
    return luaObjectDestroy<ByteMemory>(plua_state, kByteMemoryHandle);
}

static int toString(lua_State* plua_state)
{
    return luaObjectToString<ByteMemory>(plua_state, kByteMemoryHandle);
}

static int size(lua_State* plua_state)
{    
    ByteMemory* pmemory = luaGetObjectData<ByteMemory>(plua_state, kByteMemoryHandle);
    luaPushInteger(plua_state, pmemory->size());

    return 1;
}

static int buf(lua_State* plua_state)
{
    ByteMemory* pmemory = luaGetObjectData<ByteMemory>(plua_state, kByteMemoryHandle);
    luaPushLightUserData(plua_state, pmemory->buf());

    return 1;
}

static int clear(lua_State* plua_state)
{
    ByteMemory* pmemory = luaGetObjectData<ByteMemory>(plua_state, kByteMemoryHandle);
    pmemory->clear();

    return 0;
}

static int offset(lua_State* plua_state)
{
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "offset", p, "address is null");

    luaPushLightUserData(plua_state, (void*)(p + luaGetInteger(plua_state, 2, 0)));

    return 1;
}

static int memstr(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memstr", luaGetTop(plua_state) == 2,
        "parameter count should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memstr", p, "address is null");

    luaPushString(plua_state, memoryStr(p, luaGetInteger(plua_state, 2)));

    return 1;
}

static int tostring(lua_State* plua_state)
{
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "tostring", p, "c-string address is null");

    luaPushString(plua_state, toString(p));

    return 1;
}

static int memset(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memset", 3 == luaGetTop(plua_state),
        "parameter count should be 3");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memset", p, "address is null");

    ::memset(p, luaGetInteger(plua_state, 2), luaGetInteger(plua_state, 3));

    return 0;
}

static int memcpy(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memcpy", 3 == luaGetTop(plua_state),
        "parameter count should be 3");
    char* p1 = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memcpy", p1,
        "dest address is null");

    char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memcpy", p2,
        "src address is null");

    ::memcpy(p1, p2, luaGetInteger(plua_state, 3, 0));

    return 0;
}

static int memmove(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memmove", 3 == luaGetTop(plua_state),
        "parameter count should be 3");
    char* p1 = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memmove", p1,
        "dest address is null");

    char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "memmove", p2,
        "src address is null");

    ::memmove(p1, p2, luaGetInteger(plua_state, 3, 0));

    return 0;
}

static int strcpy(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "strcpy", 2 == luaGetTop(plua_state),
        "parameter count should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "strcpy", p,
        "dest address is null");

    if(LuaLightUserData == luaGetType(plua_state, 2))
    {
        char* p2 = static_cast<char*>(luaGetLightUserData(plua_state, 2));
        if(p2)
            ::strcpy(p, p2);
    }
    else if (LuaString == luaGetType(plua_state, 2))
    {
        std::string str = luaGetString(plua_state, 2);
        ::strcpy(p, str.c_str());
    }

    return 0;
}

template <typename T>
static int setUInt(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "setUnit", 2 == luaGetTop(plua_state),
        "parameter count should be 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "setUnit", p,
        "address is null");

    *(T*)p = luaGetInteger(plua_state, 2);

    return 0;
}

template <typename T>
static int getUInt(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "getUnit", 1 == luaGetTop(plua_state),
        "parameter count should be 1");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "getUnit", p,
        "address is null");

    luaPushInteger(plua_state, *(T*)p);

    return 1;
}

static int setBytes(lua_State* plua_state)
{
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "setBytes", luaGetTop(plua_state) > 1,
        "parameter count should not less than 2");
    char* p = static_cast<char*>(luaGetLightUserData(plua_state, 1, 0));
    luaExtendAssert(plua_state, kLuaExtendLibUtil, "setBytes", p,
        "address is null");

    for (size_t i=2; i<=(size_t)luaGetTop(plua_state); ++i)
    {
        luaExtendAssert(plua_state, kLuaExtendLibUtil, "setBytes", luaGetInteger(plua_state, i) <= 255,
            "byte value should not be more than 255");
        char* p1 = p + (i-2);
        *(uint8_t*)(p1) = luaGetInteger(plua_state, i);
    }

    return 0;
}

static const LuaReg memory_lib[] =
{
    {"createMemory", createMemory},
        
    {"offset", offset},
    {"memset", memset},
    {"memcpy", memcpy},
    {"memmove", memmove},
    {"strcpy", strcpy},
    {"memstr", memstr},
    {"tostring", tostring},

    {"setUInt8", setUInt<uint8_t>},
    {"setUInt16", setUInt<uint16_t>},
    {"setUInt32", setUInt<uint32_t>},
    {"getUInt8", getUInt<uint8_t>},
    {"getUInt16", getUInt<uint16_t>},
    {"getUInt32", getUInt<uint32_t>},
    {"setBytes", setBytes},

    {0, 0}
};

static const LuaReg memory_obj_lib[] =
{
    {"destroy", destroy},
    {"size", size},
    {"clear", clear},
    {"buf", buf},
    {"__gc", destroy},
    {"__tostring", toString},
    {0, 0}
};

void extendMemory(lua_State* plua_state) 
{
    LuaRegCombUtilLib::getInstance().addRegArray((LuaReg*)memory_lib);
    luaCreateMeta(plua_state, kByteMemoryHandle, (LuaReg*)memory_obj_lib);
}

}
