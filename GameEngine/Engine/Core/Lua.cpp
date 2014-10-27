#include "Lua.h"

#include "Math3D.h"
#include "Transform.h"
#include "Script.h"

lua_State* Lua::luaState = luaL_newstate();

void Lua::register_()
{
	Math3D::registerMembers("Math", Lua::luaState);
	Transform::registerMembers("Core", Lua::luaState);
}