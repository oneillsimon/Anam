#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "Input.h"
#include "GameComponent.h"
#include "Lua.h"
#include "../Rendering/RenderingEngine.h"

enum FUNC_TYPE
{
	INPUT,
	UPDATE,
	RENDER,
	OTHER
};

class ScriptManager : public GameComponent
{
private:
	lua_State* m_L;
	std::string m_scriptName;
	std::ofstream m_finalScript;

	std::vector<std::string> m_otherCode;
	std::vector<std::string> m_localCode;
	std::vector<std::string> m_inputCode;
	std::vector<std::string> m_updateCode;
	std::vector<std::string> m_renderCode;

	void loadScript(const std::string& fileName);

public:
	ScriptManager(std::vector<std::string> scripts = {});
	~ScriptManager();

	virtual void initialise();
	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;

	void addScript(const std::string& script);
	void addFunctionCode(const std::string& code, int function);
	void addLocalCode(const std::string& code);

	void generateFunctionBody(std::ifstream& file, int function, const std::string& functionDeclartion = "");

	template<class T>
	void setGlobal(T t, const std::string& name)
	{
		luabridge::setGlobal(m_L, t, name.c_str());
	}

	void generateScriptName(void* object);

	void setFunctionCode(const std::string& code, int index, int function);
	void setLocalCode(const std::string& code, int index);

	lua_State* getL();

	std::string getScriptName() const;

	std::vector<std::string> getFunctionCode(int function) const;
	std::vector<std::string> getLocalCode() const;
};

#endif