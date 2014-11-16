#include "Script.h"
#include "Math3D.h"
#include "Util.h"

#include <fstream>
#include <iostream>

ProfileTimer Script::m_scriptTimer = ProfileTimer();

std::string addParts(std::vector<std::string> s);
void generateFinalScript(std::ofstream& file, ScriptManager& scriptManager);

Script::Script(const std::string& script, ScriptManager& scriptManager) :
	m_scriptManager(scriptManager),
	m_scriptName("res/scripts/gen/" + scriptManager.getScriptName())
{
	m_finalScript.open(m_scriptName);
	loadScript(script, scriptManager);

	if(luaL_dofile(scriptManager.getL(), m_scriptName.c_str()));
	{
		const char* err = lua_tostring(scriptManager.getL(), -1);
		printf("%s\n", err);
	}
}

Script::~Script()
{
	remove(m_scriptName.c_str());
}

void Script::input(const Input& input, float delta)
{
}

void Script::update(float delta)
{
	m_scriptTimer.startInvocation();
	lua_getglobal(m_parent->getL(), "final_update");
	
	if(lua_isfunction(m_parent->getL(), lua_gettop(m_parent->getL())))
	{
		lua_call(m_parent->getL(), 0, 0);
	}

	Transform t = (Transform)luabridge::getGlobal(m_parent->getL(), "transform");

	if(*m_parent->getTransform() != t)
	{
		m_scriptManager.setGlobal(m_parent->getTransform(), "transform");
	}

	m_parent->setTransform(t);

	m_scriptTimer.stopInvocation();
}

void Script::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
}

void Script::loadScript(const std::string& fileName, ScriptManager& scriptManager)
{
	std::ifstream fileIn;
	std::vector<std::string> old_;
	std::vector<std::string> new_;

	fileIn.open(("./res/scripts/" + fileName).c_str());

	std::string line;

	if(fileIn.is_open())
	{
		while(fileIn.good())
		{
			getline(fileIn, line);
			
			std::vector<std::string> parts = Util::split(line, ' ');

			if(parts[0] == "local")
			{
				old_.push_back(parts[1]);
				parts[1] = Util::split(fileName, '.')[0] + "_" + parts[1];
				new_.push_back(parts[1]);
				parts.erase(parts.begin());

				scriptManager.addLocalCode(addParts(parts));
			}
			else if(parts[0] == "function")
			{
				if(parts[1] == "update()")
				{
					scriptManager.generateFunctionBody(fileIn, FUNC_TYPE::UPDATE);
				}
				else
				{
					scriptManager.generateFunctionBody(fileIn, FUNC_TYPE::OTHER, addParts(parts));
				}
			}

			for(int i = 0; i < scriptManager.getFunctionCode(FUNC_TYPE::UPDATE).size(); i++)
			{
				for(int j = 0; j < old_.size(); j++)
				{
					std::string update = scriptManager.getFunctionCode(FUNC_TYPE::UPDATE)[i];
					Util::findAndReplace(update, old_[j], new_[j]);
					
					scriptManager.setFunctionCode(update, i, FUNC_TYPE::UPDATE);
				}
			}
		}

		generateFinalScript(m_finalScript, scriptManager);
	}
	else
	{
		std::cout << "Unable to load script " << fileName << std::endl;
	}

	old_.clear();
	new_.clear();

	fileIn.close();
	m_finalScript.close();
}

std::string addParts(std::vector<std::string> s)
{
	std::string line;
	for(int i = 0; i < s.size(); i++)
	{
		line += s[i] + " ";
	}

	return line;
}

void generateFinalScript(std::ofstream& file, ScriptManager& scriptManager)
{
	for(int i = 0; i < scriptManager.getLocalCode().size(); i++)
	{
		file << scriptManager.getLocalCode()[i] << "\n";
	}

	for(int i = 0; i < scriptManager.getFunctionCode(FUNC_TYPE::OTHER).size(); i++)
	{
		file << scriptManager.getFunctionCode(FUNC_TYPE::OTHER)[i] << "\n";
	}

	file << "function final_update()\n";

	for(int i = 0; i < scriptManager.getFunctionCode(FUNC_TYPE::UPDATE).size(); i++)
	{
		if(scriptManager.getFunctionCode(FUNC_TYPE::UPDATE)[i] != "end")
		{
			file << scriptManager.getFunctionCode(FUNC_TYPE::UPDATE)[i] << "\n";
		}
	}
	file << "end\n";
}