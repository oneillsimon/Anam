#include "ScriptManager.h"

ScriptManager::ScriptManager()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	Lua::registerMembers(L);
}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::addLocalCode(const std::string& code)
{
	m_localCode.push_back(code);
}

void ScriptManager::addUpdateCode(const std::string& code)
{
	m_updateCode.push_back(code);
}

void ScriptManager::generateScriptName(void* object)
{
	std::ostringstream address;
	address << (void const *)object << ".lua";
	scriptName = address.str();
}

void ScriptManager::setLocalCode(const std::string& code, int index)
{
	m_localCode[index] = code;
}

void ScriptManager::setUpdateCode(const std::string& code, int index)
{
	m_updateCode[index] = code;
}

lua_State* ScriptManager::getL()
{
	return L;
}

std::string ScriptManager::getScriptName() const
{
	return scriptName;
}

std::vector<std::string> ScriptManager::getLocalCode() const
{
	return m_localCode;
}

std::vector<std::string> ScriptManager::getUpdateCode() const
{
	return m_updateCode;
}