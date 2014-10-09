#include "Lua.h"

#include "Math3D.h"

lua_State* Lua::luaState = luaL_newstate();

void Lua::register_()
{
	Vector2::registerMembers("Math", Lua::luaState);
	Vector3::registerMembers("Math", Lua::luaState);
	Vector4::registerMembers("Math", Lua::luaState);
}