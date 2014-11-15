#include "ScriptHelper.h"

ScriptHelper::ScriptHelper()
{
}

ScriptHelper::~ScriptHelper()
{
}

void ScriptHelper::addLocalCode(const std::string& code)
{
	m_localCode.push_back(code);
}

void ScriptHelper::addUpdateCode(const std::string& code)
{
	m_updateCode.push_back(code);
}

std::vector<std::string> ScriptHelper::getLocalCode() const
{
	return m_localCode;
}

std::vector<std::string> ScriptHelper::getUpdateCode() const
{
	return m_updateCode;
}

void ScriptHelper::setLocalCode(const std::string& code, int index)
{
	m_localCode[index] = code;
}

void ScriptHelper::setUpdateCode(const std::string& code, int index)
{
	m_updateCode[index] = code;
}