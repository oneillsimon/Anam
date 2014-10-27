#include "Script.h"
#include "Math3D.h"

static void injectIntoLua(lua_State* luaState, GameObject* t);

Script::Script(const std::string& script, float s) :
	scriptName("Engine/Scripts/" + script),
	m_speed(s)
{
	if(luaL_dofile(Lua::luaState, scriptName.c_str()))
	{
		const char* err = lua_tostring(Lua::luaState, -1);
		printf("%s\n", err);
	}

	m_speed = s;
}

Script::~Script()
{
}

void Script::input(const Input& input, float delta)
{
}

void Script::update(float delta)
{
	luabridge::setGlobal(Lua::luaState, m_parent->getTransform(), "transform");
	luabridge::setGlobal(Lua::luaState, m_speed, "n");

	lua_getglobal(Lua::luaState, "update");

	if(lua_isfunction(Lua::luaState, lua_gettop(Lua::luaState)))
	{
		lua_call(Lua::luaState, 0, 0);
	}

	Transform t = (Transform)luabridge::getGlobal(Lua::luaState, "transform");
	Vector3 p = t.getPosition();
	Quaternion r = t.getRotation();
	Vector3 s = t.getScale();
	
	m_parent->getTransform()->setPosition(p);
	m_parent->getTransform()->setRotation(r);
	m_parent->getTransform()->setScale(s);
}

void Script::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
}