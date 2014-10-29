#ifndef LUA_H
#define LUA_H

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge.h>

class Lua
{
public:
//	static lua_State* L;
	static void register_();
};

#endif