#ifndef SCRIPTHELPER_H
#define SCRIPTHELPER_H

#include <string>
#include <vector>

class ScriptHelper
{
private:
	std::vector<std::string> m_localCode;
	std::vector<std::string> m_updateCode;

public:
	ScriptHelper();
	~ScriptHelper();

	std::vector<std::string> getLocalCode() const;
	std::vector<std::string> getUpdateCode() const;

	void addLocalCode(const std::string& code);
	void addUpdateCode(const std::string& code);

	void setLocalCode(const std::string& code, int index);
	void setUpdateCode(const std::string& code, int index);
};

#endif