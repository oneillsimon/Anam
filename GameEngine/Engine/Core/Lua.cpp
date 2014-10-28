#include "Lua.h"

#include "Math3D.h"
#include "Transform.h"
#include "Script.h"

lua_State* Lua::L = luaL_newstate();

void Lua::register_()
{
	Math3D::registerMembers("Math", Lua::L);
	Transform::registerMembers("Core", Lua::L);
}