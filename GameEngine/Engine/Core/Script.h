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
	ScriptManager& m_scriptManager;
	std::string m_scriptName;
	std::ofstream m_finalScript;

	void loadScript(const std::string& fileName, ScriptManager& scriptManager);

public:
	static ProfileTimer m_scriptTimer;

	Script(const std::string& script, ScriptManager& scriptManager);
	~Script();

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif