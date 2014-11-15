#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <string>
#include <vector>

#include "Lua.h"

class ScriptManager
{
private:
	lua_State* m_L;
	std::string m_scriptName;

	std::vector<std::string> m_localCode;
	std::vector<std::string> m_updateCode;

public:
	ScriptManager();
	~ScriptManager();

	void addLocalCode(const std::string& code);
	void addUpdateCode(const std::string& code);

	template<class T>
	void setGlobal(T t, const std::string& name)
	{
		luabridge::setGlobal(L, t, name.c_str());
	}

	void generateScriptName(void* object);

	void setLocalCode(const std::string& code, int index);
	void setUpdateCode(const std::string& code, int index);

	lua_State* getL();

	std::string getScriptName() const;
	std::vector<std::string> getLocalCode() const;
	std::vector<std::string> getUpdateCode() const;
};

#endif