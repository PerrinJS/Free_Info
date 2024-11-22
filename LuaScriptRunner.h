#ifndef LUA_SCRIPT_RUNNER
#define LUA_SCRIPT_RUNNER

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

lua_State *initLua();
char *getStringFromLuaScript(lua_State *L, const char *luaFileLocation);

#endif
