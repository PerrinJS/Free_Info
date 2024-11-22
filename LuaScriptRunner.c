//#include "LuaScriptRunner.h"
#include <stdlib.h>
#include <string.h>

#include "LuaScriptRunner.h"

lua_State *initLua() {
    lua_State *L = luaL_newstate();
    //use stdlib
    luaL_openlibs(L);
    return  L;
}

char *getStringFromLuaScript(lua_State *L, const char *luaFileLocation) {
    //int err = luaL_dofile(L, luaFileLocation);
    luaL_dofile(L, luaFileLocation);
    char *ret = NULL;

    //If lua fails to run the file it puts the error message on the stack as a string anyway
    //This isn't great error handling but for the moment it will do
    if (lua_isstring(L, -1)) {
        const char *result = lua_tostring(L, -1);
        //copy to a return value
        const int stringMemBlkSize = sizeof(char)*(strlen(result)+1);
        ret = (char*) malloc(stringMemBlkSize);
        memcpy(ret, result, stringMemBlkSize);
    }

    //clear lua stack
    lua_settop(L, 0);
    return ret;
}

