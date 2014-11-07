#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdio.h>

#include "CoreEngine.h"
#include "GameComponent.h"
#include "Lua.h"

class Script : public GameComponent
{
private:
	std::string scriptName;

protected:
	lua_State* L;

public:
	Script(const std::string& script);
	~Script();

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif