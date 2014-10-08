#include "Lua.h"

#include "Math3D.h"

lua_State* Lua::luaState = luaL_newstate();

void Lua::register_()
{
	Vector3::registerMembers(Lua::luaState);
}