#include "Script.h"
#include "Math3D.h"

Script::Script(const std::string& script) :
	scriptName("Engine/Scripts/" + script)
{
	if(luaL_dofile(Lua::luaState, scriptName.c_str()))
	{
		const char* err = lua_tostring(Lua::luaState, -1);
		printf("%s\n", err);
	}
}

Script::~Script()
{
	//lua_close(Lua::luaState);
}

void Script::input(const Input& input, float delta)
{
}

void Script::update(float delta)
{
	lua_getglobal(Lua::luaState, "update");

	if(lua_isfunction(Lua::luaState, lua_gettop(Lua::luaState)))
	{
		lua_call(Lua::luaState, 0, 0);
	}

	lua_getglobal(Lua::luaState, "n");
	float n = (float)lua_tonumber(Lua::luaState, -1);
	m_parent->getTransform()->rotate(AXIS_Z, toRadians(n / 2.0f));
}

void Script::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
}