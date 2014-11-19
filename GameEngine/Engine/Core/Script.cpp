#include "Script.h"
#include "Math3D.h"
#include "Util.h"

#include <fstream>
#include <iostream>

ProfileTimer Script::m_scriptTimer = ProfileTimer();

std::string INPUT_STR = "input(delta)";
std::string UPDATE_STR = "update(delta)";
std::string RENDER_STR = "render()";
std::string FINAL_INPUT_STR = "function final_input(delta)";
std::string FINAL_UPDATE_STR = "function final_update(delta)";
std::string FINAL_RENDER_STR = "function final_render()";
std::string LUA_INPUT = "final_input";
std::string LUA_UPDATE = "final_update";
std::string LUA_RENDER = "final_render";

std::string addParts(std::vector<std::string> s);
void generateFinalScript(std::ofstream& file, ScriptManager& scriptManager);
void renameFunctionVariables(ScriptManager& scriptManager, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_);
void generateFunctionCode(std::ofstream& file, ScriptManager& scriptManager, int function, bool includeEnd);

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
	lua_getglobal(m_parent->getL(), LUA_UPDATE.c_str());

	if(lua_isfunction(m_parent->getL(), lua_gettop(m_parent->getL())))
	{
		lua_pushnumber(m_parent->getL(), delta);
		lua_call(m_parent->getL(), 1, 0);
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
				if(parts[1] == UPDATE_STR.c_str())
				{
					scriptManager.generateFunctionBody(fileIn, FUNC_TYPE::UPDATE);
				}
				else if(parts[1] == INPUT_STR.c_str())
				{
					scriptManager.generateFunctionBody(fileIn, FUNC_TYPE::INPUT);
				}
				else if(parts[1] == RENDER_STR.c_str())
				{
					scriptManager.generateFunctionBody(fileIn, FUNC_TYPE::RENDER);
				}
				else
				{
					scriptManager.generateFunctionBody(fileIn, FUNC_TYPE::OTHER, addParts(parts));
				}
			}

			renameFunctionVariables(scriptManager, FUNC_TYPE::OTHER, old_, new_);
			renameFunctionVariables(scriptManager, FUNC_TYPE::INPUT, old_, new_);
			renameFunctionVariables(scriptManager, FUNC_TYPE::UPDATE, old_, new_);
			renameFunctionVariables(scriptManager, FUNC_TYPE::RENDER, old_, new_);
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
	
	generateFunctionCode(file, scriptManager, FUNC_TYPE::OTHER, true);

	file << FINAL_INPUT_STR << "\n";
	generateFunctionCode(file, scriptManager, FUNC_TYPE::INPUT, false);

	file << FINAL_UPDATE_STR << "\n";
	generateFunctionCode(file, scriptManager, FUNC_TYPE::UPDATE, false);

	file << FINAL_RENDER_STR << "\n";
	generateFunctionCode(file, scriptManager, FUNC_TYPE::RENDER, false);
}

void renameFunctionVariables(ScriptManager& scriptManager, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_)
{
	for(int i = 0; i < scriptManager.getFunctionCode(function).size(); i++)
	{
		for(int j = 0; j < old_.size(); j++)
		{
			std::string s = scriptManager.getFunctionCode(function)[i];
			Util::findAndReplace(s, old_[j], new_[j]);
			scriptManager.setFunctionCode(s, i, function);
		}
	}
}

void generateFunctionCode(std::ofstream& file, ScriptManager& scriptManager, int function, bool includeAnyEnds)
{
	for(int i = 0; i < scriptManager.getFunctionCode(function).size(); i++)
	{
		if(scriptManager.getFunctionCode(function)[i] != "end")
		{
			file << scriptManager.getFunctionCode(function)[i] << "\n";
		}
		else
		{
			if(includeAnyEnds)
			{
				file << scriptManager.getFunctionCode(function)[i] << "\n";
			}
		}
	}

	if(!includeAnyEnds)
	{
		file << "end\n";
	}
}