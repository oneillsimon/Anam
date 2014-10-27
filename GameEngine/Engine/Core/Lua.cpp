#include "Lua.h"

#include "Math3D.h"

lua_State* Lua::luaState = luaL_newstate();

void Lua::register_()
{
	Vector2 v2;
	Vector3 v3;
	Vector4 v4;

	//v2.registerMembers("Math", Lua::luaState);
	//v3.registerMembers("Math", Lua::luaState);
	//v4.registerMembers("Math", Lua::luaState);
}