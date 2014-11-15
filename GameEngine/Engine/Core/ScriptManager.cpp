#include "ScriptManager.h"

ScriptManager::ScriptManager()
{
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

std::vector<std::string> ScriptManager::getLocalCode() const
{
	return m_localCode;
}

std::vector<std::string> ScriptManager::getUpdateCode() const
{
	return m_updateCode;
}

void ScriptManager::setLocalCode(const std::string& code, int index)
{
	m_localCode[index] = code;
}

void ScriptManager::setUpdateCode(const std::string& code, int index)
{
	m_updateCode[index] = code;
}