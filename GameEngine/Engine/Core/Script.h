#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdio.h>

#include "CoreEngine.h"
#include "GameComponent.h"
#include "Lua.h"

class Script : public GameComponent
{
private:
	//lua_State* m_luaState;
	std::string scriptName;

public:
	static void registerMembers(const std::string& namespace_, lua_State* luaState)
	{
		luabridge::getGlobalNamespace(luaState)
			.beginNamespace(namespace_.c_str())
			.beginClass<Script>("Script")
			.endClass()
			.endNamespace();
	}

	Script(const std::string& script);
	~Script();

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif