#ifndef SCRIPT_H
#define SCRIPT_H

#include <fstream>
#include <stdio.h>

#include "CoreEngine.h"
#include "GameComponent.h"
#include "Lua.h"

class Script : public GameComponent
{
private:
	std::string scriptName;
	std::ofstream finalScript;
	void loadScript(const std::string& fileName, ScriptManager& scriptManager);

protected:
public:
	static ProfileTimer m_scriptTimer;

	Script(const std::string& script, ScriptManager& scriptManager);
	~Script();

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif