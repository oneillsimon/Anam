#include "ScriptManager.h"

ScriptManager::ScriptManager()
{
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	Lua::registerMembers(m_L);
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
	m_scriptName = address.str();
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
	return m_L;
}

std::string ScriptManager::getScriptName() const
{
	return m_scriptName;
}

std::vector<std::string> ScriptManager::getLocalCode() const
{
	return m_localCode;
}

std::vector<std::string> ScriptManager::getUpdateCode() const
{
	return m_updateCode;
}