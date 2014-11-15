#include "Script.h"
#include "Math3D.h"
#include "Util.h"

#include <fstream>
#include <iostream>

static void injectIntoLua(lua_State* luaState, GameObject* t);
ProfileTimer Script::m_scriptTimer = ProfileTimer();

std::string addParts(std::vector<std::string> s);

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

	m_parent->setTransform((Transform)luabridge::getGlobal(m_parent->getL(), "transform"));

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
				parts[0] = "";
				scriptManager.addLocalCode(addParts(parts));
			}
			else if(parts[0] == "function")
			{
				if(parts[1] == "update()")
				{
					std::string s;
					while(s != "end")
					{
						getline(fileIn, s);
						scriptManager.addUpdateCode(s);
					}
				}
			}

			for(int i = 0; i < scriptManager.getUpdateCode().size(); i++)
			{
				for(int j = 0; j < old_.size(); j++)
				{
					std::string update = scriptManager.getUpdateCode()[i];
					Util::findAndReplace(update, old_[j], new_[j]);
					
					scriptManager.setUpdateCode(update, i);
				}
			}
		}

		for(int i = 0; i < scriptManager.getLocalCode().size(); i++)
		{
			m_finalScript << scriptManager.getLocalCode()[i] << "\n";
		}

		m_finalScript << "function final_update()\n";

		for(int i = 0; i < scriptManager.getUpdateCode().size(); i++)
		{
			if(scriptManager.getUpdateCode()[i] != "end")
			{
				m_finalScript << scriptManager.getUpdateCode()[i] << "\n";
			}
		}
		m_finalScript << "end\n";
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