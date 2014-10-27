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
	float m_speed;

public:
	Script(const std::string& script, float s);
	~Script();

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif