#include <fstream>
#include <iostream>

#include "ScriptManager.h"
#include "Util.h"

std::string SCRIPT_DIR = "res/scripts/";
std::string SCRIPT_GEN = SCRIPT_DIR + "gen/";

std::string INPUT_STR = "input(delta)";
std::string UPDATE_STR = "update(delta)";
std::string RENDER_STR = "render()";
std::string FINAL_INPUT_STR = "function final_input(delta)";
std::string FINAL_UPDATE_STR = "function final_update(delta)";
std::string FINAL_RENDER_STR = "function final_render()";
std::string LUA_INPUT = "final_input";
std::string LUA_UPDATE = "final_update";
std::string LUA_RENDER = "final_render";

void generateFinalScript(std::ofstream& file, ScriptManager& scriptManager);
void renameFunctionVariables(ScriptManager& scriptManager, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_);
void generateFunctionCode(std::ofstream& file, ScriptManager& scriptManager, int function, bool includeEnd);

ScriptManager::ScriptManager(std::vector<std::string> scripts)
{
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	Lua::registerMembers(m_L);

	generateScriptName(this);

	for(unsigned int i = 0; i < scripts.size(); i++)
	{
		addScript(scripts[i]);
	}
}

ScriptManager::~ScriptManager()
{
	remove(m_scriptName.c_str());
}

void ScriptManager::initialise()
{
	setGlobal(m_parent->getTransform(), "transform");
}

void ScriptManager::input(const Input& input, float delta)
{
	lua_getglobal(getL(), LUA_INPUT.c_str());
	
	setGlobal(input, "input");
	
	if(lua_isfunction(getL(), lua_gettop(getL())))
	{
		lua_pushnumber(getL(), delta);
		lua_call(getL(), 1, 0);
	}
}

void ScriptManager::update(float delta)
{
	lua_getglobal(getL(), LUA_UPDATE.c_str());
	
	if(lua_isfunction(getL(), lua_gettop(getL())))
	{
		lua_pushnumber(getL(), delta);
		lua_call(getL(), 1, 0);
	}

	Transform t = (Transform)luabridge::getGlobal(getL(), "transform");
	
	if(*m_parent->getTransform() != t)
	{
		setGlobal(m_parent->getTransform(), "transform");
		m_parent->setTransform(t);
	}
}

void ScriptManager::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
}

void ScriptManager::loadScript(const std::string& fileName)
{
	std::ifstream fileIn;
	std::vector<std::string> old_;
	std::vector<std::string> new_;

	fileIn.open((SCRIPT_DIR + fileName).c_str());

	std::string line;
	int seed = random(0, INT_MAX);
	std::string seedPrefix = "s" + std::to_string(seed);

	if(fileIn.is_open())
	{
		while(fileIn.good())
		{
			getline(fileIn, line);

			std::vector<std::string> parts = Util::split(line, ' ');

			if(parts[0] == "local")
			{
				old_.push_back(parts[1]);
				parts[1] = seedPrefix + parts[1];
				new_.push_back(parts[1]);
				parts.erase(parts.begin());

				addLocalCode(Util::stringFromVector(parts, " "));
			}
			else if(parts[0] == "function")
			{
				if(parts[1] == UPDATE_STR.c_str())
				{
					generateFunctionBody(fileIn, FUNC_TYPE::UPDATE);
				}
				else if(parts[1] == INPUT_STR.c_str())
				{
					generateFunctionBody(fileIn, FUNC_TYPE::INPUT);
				}
				else if(parts[1] == RENDER_STR.c_str())
				{
					generateFunctionBody(fileIn, FUNC_TYPE::RENDER);
				}
				else
				{
					parts.erase(parts.begin());
					std::string line_ = Util::stringFromVector(parts, " ");
					int paramsStart = line_.find('(');
					std::string params = line_.substr(paramsStart, line_.length() - paramsStart);
					line_ = line_.substr(0, paramsStart);
					old_.push_back(line_);
					line_ = seedPrefix + line_;
					new_.push_back(line_);

					generateFunctionBody(fileIn, FUNC_TYPE::OTHER, "function " + line_ + params);
				}
			}

			renameFunctionVariables(*this, FUNC_TYPE::OTHER, old_, new_);
			renameFunctionVariables(*this, FUNC_TYPE::INPUT, old_, new_);
			renameFunctionVariables(*this, FUNC_TYPE::UPDATE, old_, new_);
			renameFunctionVariables(*this, FUNC_TYPE::RENDER, old_, new_);
		}

		generateFinalScript(m_finalScript, *this);
	}
	else
	{
		std::cout << "Unable to load script " << fileName << std::endl;
	}

	old_.clear();
	new_.clear();

	fileIn.close();
}

void ScriptManager::addScript(const std::string& script)
{
	m_finalScript.open(m_scriptName);
	loadScript(script);
	m_finalScript.close();

	if(luaL_dofile(getL(), m_scriptName.c_str()))
	{
		const char* err = lua_tostring(m_L, -1);
		printf("%s\n", err);
	}
}

void ScriptManager::addFunctionCode(const std::string& code, int function)
{
	switch(function)
	{
	case FUNC_TYPE::INPUT:
		m_inputCode.push_back(code);
		break;
	case FUNC_TYPE::UPDATE:
		m_updateCode.push_back(code);
		break;
	case FUNC_TYPE::RENDER:
		m_renderCode.push_back(code);
		break;
	case FUNC_TYPE::OTHER:
		m_otherCode.push_back(code);
		break;
	}
}

void ScriptManager::addLocalCode(const std::string& code)
{
	m_localCode.push_back(code);
}

void ScriptManager::generateFunctionBody(std::ifstream& file, int function, const std::string& functionDeclartion)
{
	std::string s;

	addFunctionCode(functionDeclartion, function);

	while(s != "end")
	{
		getline(file, s);
		addFunctionCode(s, function);
	}
}

void ScriptManager::generateScriptName(void* object)
{
	std::ostringstream address;
	address << "res/scripts/gen/" << (void const *)object << ".lua";
	m_scriptName = address.str();
}

void ScriptManager::setFunctionCode(const std::string& code, int index, int function)
{
	switch(function)
	{
	case FUNC_TYPE::INPUT:
		m_inputCode[index] = code;
		break;
	case FUNC_TYPE::UPDATE:
		m_updateCode[index] = code;
		break;
	case FUNC_TYPE::RENDER:
		m_renderCode[index] = code;
		break;
	case FUNC_TYPE::OTHER:
		m_otherCode[index] = code;
		break;
	}
}

void ScriptManager::setLocalCode(const std::string& code, int index)
{
	m_localCode[index] = code;
}

lua_State* ScriptManager::getL()
{
	return m_L;
}

std::string ScriptManager::getScriptName() const
{
	return m_scriptName;
}

std::vector<std::string> ScriptManager::getFunctionCode(int function) const
{
	switch(function)
	{
	case FUNC_TYPE::INPUT:
		return m_inputCode;
		break;
	case FUNC_TYPE::UPDATE:
		return m_updateCode;
		break;
	case FUNC_TYPE::RENDER:
		return m_renderCode;
		break;
	case FUNC_TYPE::OTHER:
		return m_otherCode;
		break;
	}
}

std::vector<std::string> ScriptManager::getLocalCode() const
{
	return m_localCode;
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
			std::vector<char> regex = { ':', '.' };
			Util::findAndReplace(s, old_[j], new_[j], regex);
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