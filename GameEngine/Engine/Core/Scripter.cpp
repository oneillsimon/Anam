#include <fstream>
#include <iostream>

#include "Scripter.h"
#include "Util.h"

std::string SCRIPT_DIR = "res/scripts/";
std::string SCRIPT_GEN = SCRIPT_DIR + "gen/";

std::string PREFIX = "final_";

std::string INIT_ARGS = "()";
std::string INPUT_ARGS = "(delta)";
std::string UPDATE_ARGS = "(delta)";
std::string RENDER_ARGS = "()";

std::string INIT_STR = "initialise";
std::string INPUT_STR = "input";
std::string UPDATE_STR = "update";
std::string RENDER_STR = "render";

std::string LUA_INIT = PREFIX + INIT_STR;
std::string LUA_INPUT = PREFIX + INPUT_STR;
std::string LUA_UPDATE = PREFIX + UPDATE_STR;
std::string LUA_RENDER = PREFIX + RENDER_STR;

std::string FINAL_INIT_STR = "function " + LUA_INIT + INIT_ARGS;
std::string FINAL_INPUT_STR = "function " + LUA_INPUT + INPUT_ARGS;
std::string FINAL_UPDATE_STR = "function " + LUA_UPDATE + UPDATE_ARGS;
std::string FINAL_RENDER_STR = "function " + LUA_RENDER + RENDER_ARGS;

void generateFinalScript(std::ofstream& file, Scripter& scripter);
void renameFunctionVariables(Scripter& scripter, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_);
void generateFunctionCode(std::ofstream& file, Scripter& scripter, int function, bool includeEnd);

Scripter::Scripter(std::initializer_list<std::string> scripts)
{
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	Lua::registerMembers(m_L);

	generateScriptName(this);

	std::initializer_list<std::string>::iterator i = scripts.begin();

	while(i != scripts.end())
	{
		addScript(i->c_str());
		i++;
	}
}

Scripter::~Scripter()
{
	remove(m_scriptName.c_str());
}

void Scripter::initialise()
{
	lua_getglobal(m_L, LUA_INIT.c_str());

	setGlobal("transform", m_parent->getTransform());

	if(lua_isfunction(m_L, lua_gettop(m_L)))
	{
		lua_call(m_L, 0, 0);
	}
}

void Scripter::input(const Input& input, float delta)
{
	lua_getglobal(m_L, LUA_INPUT.c_str());
	
	setGlobal("input", input);
	
	if(lua_isfunction(m_L, lua_gettop(m_L)))
	{
		lua_pushnumber(m_L, delta);
		lua_call(m_L, 1, 0);
	}
}

void Scripter::update(float delta)
{
	lua_getglobal(m_L, LUA_UPDATE.c_str());
	
	if(lua_isfunction(m_L, lua_gettop(m_L)))
	{
		lua_pushnumber(m_L, delta);
		lua_call(m_L, 1, 0);
	}

	Transform t = (Transform)luabridge::getGlobal(m_L, "transform");
	
	if(*m_parent->getTransform() != t)
	{
		setGlobal("transform", m_parent->getTransform());
		m_parent->setTransform(t);
	}
}

void Scripter::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	lua_getglobal(m_L, LUA_RENDER.c_str());

	if(lua_isfunction(m_L, lua_gettop(m_L)))
	{
		lua_call(m_L, 0, 0);
	}
}

void Scripter::loadScript(const std::string& fileName)
{
	std::ifstream fileIn;
	std::vector<std::string> old_;
	std::vector<std::string> new_;

	fileIn.open((SCRIPT_DIR + fileName).c_str());

	std::string line;
	int seed = random(0, INT_MAX - 1);
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
				if(parts[1] == (INIT_STR + INIT_ARGS).c_str())
				{
					generateFunctionBody(fileIn, FUNC_TYPE::INIT);
				}
				else if(parts[1] == (INPUT_STR + INPUT_ARGS).c_str())
				{
					generateFunctionBody(fileIn, FUNC_TYPE::INPUT);
				}
				else if(parts[1] == (UPDATE_STR + UPDATE_ARGS).c_str())
				{
					generateFunctionBody(fileIn, FUNC_TYPE::UPDATE);
				}
				else if(parts[1] == (RENDER_STR + RENDER_ARGS).c_str())
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
			renameFunctionVariables(*this, FUNC_TYPE::INIT, old_, new_);
			renameFunctionVariables(*this, FUNC_TYPE::INPUT, old_, new_);
			renameFunctionVariables(*this, FUNC_TYPE::UPDATE, old_, new_);
			renameFunctionVariables(*this, FUNC_TYPE::RENDER, old_, new_);
		}

		generateFinalScript(m_finalScript, *this);
	}
	else
	{
		std::cout << "Unable to load script \"" << fileName << "\"" << std::endl;
	}

	old_.clear();
	new_.clear();

	fileIn.close();
}

void Scripter::addScript(const std::string& script)
{
	m_finalScript.open(m_scriptName);
	loadScript(script);
	m_finalScript.close();

	if(luaL_dofile(m_L, m_scriptName.c_str()))
	{
		const char* err = lua_tostring(m_L, -1);
		printf("%s\n", err);
	}
}

void Scripter::addFunctionCode(const std::string& code, int function)
{
	switch(function)
	{
	case FUNC_TYPE::INIT:
		m_initCode.push_back(code);
		break;
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

void Scripter::addLocalCode(const std::string& code)
{
	m_localCode.push_back(code);
}

void Scripter::generateFunctionBody(std::ifstream& file, int function, const std::string& functionDeclartion)
{
	std::string s;

	addFunctionCode(functionDeclartion, function);

	while(s != "end")
	{
		getline(file, s);
		addFunctionCode(s, function);
	}
}

void Scripter::generateScriptName(void* object)
{
	std::ostringstream address;
	address << SCRIPT_GEN << (void const *)object << ".lua";
	m_scriptName = address.str();
}

void Scripter::setFunctionCode(const std::string& code, int index, int function)
{
	switch(function)
	{
	case FUNC_TYPE::INIT:
		m_initCode[index] = code;
		break;
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

void Scripter::setLocalCode(const std::string& code, int index)
{
	m_localCode[index] = code;
}

lua_State* Scripter::getL()
{
	return m_L;
}

std::string Scripter::getScriptName() const
{
	return m_scriptName;
}

std::vector<std::string> Scripter::getFunctionCode(int function) const
{
	switch(function)
	{
	case FUNC_TYPE::INIT:
		return m_initCode;
		break;
	case FUNC_TYPE::INPUT:
		return m_inputCode;
		break;
	case FUNC_TYPE::UPDATE:
		return m_updateCode;
		break;
	case FUNC_TYPE::RENDER:
		return m_renderCode;
		break;
	default:
		return m_otherCode;
		break;
	}
}

std::vector<std::string> Scripter::getLocalCode() const
{
	return m_localCode;
}

void generateFinalScript(std::ofstream& file, Scripter& scripter)
{
	for(unsigned i = 0; i < scripter.getLocalCode().size(); i++)
	{
		file << scripter.getLocalCode()[i] << "\n";
	}

	generateFunctionCode(file, scripter, FUNC_TYPE::OTHER, true);

	file << FINAL_INIT_STR << "\n";
	generateFunctionCode(file, scripter, FUNC_TYPE::INIT, false);

	file << FINAL_INPUT_STR << "\n";
	generateFunctionCode(file, scripter, FUNC_TYPE::INPUT, false);

	file << FINAL_UPDATE_STR << "\n";
	generateFunctionCode(file, scripter, FUNC_TYPE::UPDATE, false);

	file << FINAL_RENDER_STR << "\n";
	generateFunctionCode(file, scripter, FUNC_TYPE::RENDER, false);
}

void renameFunctionVariables(Scripter& scripter, int function, const std::vector<std::string>& old_, const std::vector<std::string>& new_)
{
	for(unsigned i = 0; i < scripter.getFunctionCode(function).size(); i++)
	{
		for(unsigned j = 0; j < old_.size(); j++)
		{
			std::string s = scripter.getFunctionCode(function)[i];
			std::vector<char> regex = { ':', '.' };
			Util::findAndReplace(s, old_[j], new_[j], regex);
			scripter.setFunctionCode(s, i, function);
		}
	}
}

void generateFunctionCode(std::ofstream& file, Scripter& scripter, int function, bool includeAnyEnds)
{
	for(unsigned i = 0; i < scripter.getFunctionCode(function).size(); i++)
	{
		if(scripter.getFunctionCode(function)[i] != "end")
		{
			file << scripter.getFunctionCode(function)[i] << "\n";
		}
		else
		{
			if(includeAnyEnds)
			{
				file << scripter.getFunctionCode(function)[i] << "\n";
			}
		}
	}

	if(!includeAnyEnds)
	{
		file << "end\n";
	}
}