#include "Script.h"
#include "Math3D.h"

static void injectIntoLua(lua_State* luaState, GameObject* t);

Script::Script(const std::string& script) :
	scriptName("Engine/Scripts/" + script)
{
	if(luaL_dofile(Lua::luaState, scriptName.c_str()))
	{
		const char* err = lua_tostring(Lua::luaState, -1);
		printf("%s\n", err);
	}

	GameObject* tetsP = new GameObject(Vector3(101, 102, 103));
	//luabridge::setGlobal(Lua::luaState, tetsP, "testP");
	//luabridge::push<GameObject * const>(Lua::luaState, tetsP);
	//lua_setglobal(Lua::luaState, "testP");
	
	injectIntoLua(Lua::luaState, tetsP);
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

static void injectIntoLua(lua_State* luaState, GameObject* t)
{
	// push pointer to class in global registry
	//lua_pushlightuserdata(luaState, (void*)&t);
	lua_pushlightuserdata(luaState, "testP");

	// push value
	GameObject g;
	g.getTransform()->getPosition().setX(449);
	luabridge::push(Lua::luaState, &g);
	lua_setglobal(Lua::luaState, "gg");

	//luabridge::setGlobal(Lua::luaState, g, "gg");

	//lua_pushlightuserdata(luaState, (void*)i);
	//luabridge::setGlobal(Lua::luaState, i, "pushh");

	//lua_settable(luaState, LUA_REGISTRYINDEX);
}