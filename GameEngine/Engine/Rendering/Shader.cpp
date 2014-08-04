#include <cassert>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <GL\glew.h>
#include <cstring>
#include <sstream>

#include "../Core/Util.h"
#include "Shader.h"
#include "../Components/Lighting.h"
#include "RenderingEngine.h"

std::map<std::string, ShaderData*> Shader::m_resourceMap;

static void checkShaderError(int shader, int flag, bool isProgram, const std::string& errorMessage);
static std::vector<UniformStruct> findUniformStructs(const std::string& shaderText);
static std::string findUniformStructName(const std::string& structOpeningBrace);
static std::vector<TypedData> findUniformStructComponents(const std::string& structClosingBrace);
static std::string loadShader(const std::string& fileName);

ShaderData::ShaderData(const std::string& fileName)
{
	m_program = glCreateProgram();

	if(m_program == 0)
	{
		fprintf(stderr, "Error in shader program");
		exit(1);
	}

	std::string vertexShaderText = loadShader(fileName + ".vs");
	std::string fragmentShaderText = loadShader(fileName + ".fs");

	addVertexShader(vertexShaderText);
	addFragmentShader(fragmentShaderText);

	addAllAttributes(vertexShaderText);

	compileShader();

	addShaderUniforms(vertexShaderText);
	addShaderUniforms(fragmentShaderText);
}

ShaderData::~ShaderData()
{
	for(std::vector<int>::iterator it = m_shaders.begin(); it != m_shaders.end(); it++)
	{
		glDetachShader(m_program, *it);
		glDeleteShader(*it);
	}

	glDeleteProgram(m_program);
}

std::vector<std::string>& ShaderData::getUniformNames()
{
	return m_uniformNames;
}

std::vector<std::string>& ShaderData::getUniformTypes()
{
	return m_uniformTypes;
}

std::map<std::string, unsigned int>& ShaderData::getUniformMap()
{
	return m_uniformMap;
}

Shader::Shader(const std::string& fileName)
{
	this->m_fileName = fileName;

	std::map<std::string, ShaderData*>::const_iterator it = m_resourceMap.find(fileName);

	if(it != m_resourceMap.end())
	{
		m_shaderData = it->second;
		m_shaderData->addReference();
	}
	else
	{
		m_shaderData = new ShaderData(fileName);
		m_resourceMap.insert(std::pair<std::string, ShaderData*>(fileName, m_shaderData));
	}
}

Shader::Shader(const Shader& shader) :
	m_shaderData(shader.m_shaderData),
	m_fileName(shader.m_fileName)
{
	m_shaderData->addReference();
}

Shader::~Shader()
{
	if(m_shaderData && m_shaderData->removeReference())
	{
		if(m_fileName.length() > 0)
		{
			m_resourceMap.erase(m_fileName);
		}

		delete m_shaderData;
	}
}

void Shader::bind() const
{
	glUseProgram(m_shaderData->getProgram());
}

void Shader::updateUniforms(const Transform& transform, const Material& material, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	Matrix4 worldMatrix = transform.getTransformation();
	Matrix4 projectedMatrix = camera.getViewProjection() * worldMatrix;

	for(unsigned int i = 0; i < m_shaderData->getUniformNames().size(); i++)
	{
		std::string uniformName = m_shaderData->getUniformNames()[i];
		std::string uniformType = m_shaderData->getUniformTypes()[i];

		if(uniformName.substr(0, 2) == "R_")
		{
			std::string unprefixedName = uniformName.substr(2, uniformName.length());

			if(unprefixedName == "lightMatrix")
			{
				setUniform(uniformName, renderingEngine.getLightMatrix() * worldMatrix);
			}
			else if(uniformType == "sampler2D")
			{
				int samplerSlot = renderingEngine.getSamplerSlot(unprefixedName);
				renderingEngine.getTexture(unprefixedName).bind(samplerSlot);
				setUniform(uniformName, samplerSlot);
			}
			else if(uniformType == "vec3")
			{
				setUniform(uniformName, renderingEngine.getVector3(unprefixedName));
			}
			else if(uniformType == "float")
			{
				setUniform(uniformName, renderingEngine.getFloat(unprefixedName));
			}
			else if(uniformType == "DirectionalLight")
			{
				setUniformDirectionalLight(uniformName, *(const DirectionalLight*)&renderingEngine.getActiveLight());
			}
			else if(uniformType == "PointLight")
			{
				setUniformPointLight(uniformName, *(const PointLight*)&renderingEngine.getActiveLight());
			}
			else if(uniformType == "SpotLight")
			{
				setUniformSpotLight(uniformName, *(const SpotLight*)&renderingEngine.getActiveLight());
			}
			else
			{
				renderingEngine.updateUniformStruct(transform, material, *this, uniformName, uniformType);
			}
		}
		else if(uniformType == "sampler2D")
		{
			int samplerSlot = renderingEngine.getSamplerSlot(uniformName);
			material.getTexture(uniformName).bind(samplerSlot);
			setUniform(uniformName, samplerSlot);
		}
		else if(uniformName.substr(0, 2) == "T_")
		{
			if(uniformName == "T_MVP")
			{
				setUniform(uniformName, projectedMatrix);
			}
			else if(uniformName == "T_model")
			{
				setUniform(uniformName, worldMatrix);
			}
			else
			{
				throw "Invalid transform uniform: " + uniformName;
			}
		}
		else if(uniformName.substr(0, 2) == "C_")
		{
			if(uniformName == "C_eyePos")
			{
				setUniform(uniformName, camera.getTransform().getTransformedPosition());
			}
			else
			{
				throw "invalid camera uniform: " + uniformName;
			}
		}
		else
		{
			if(uniformType == "vec3")
			{
				setUniform(uniformName, material.getVector3(uniformName));
			}
			else if(uniformType == "float")
			{
				setUniform(uniformName, material.getFloat(uniformName));
			}
			else
			{
				throw uniformType + " is not supported by the Material class";
			}
		}
	}
}

void Shader::setUniform(const std::string& uniformName, int value) const
{
	glUniform1i(m_shaderData->getUniformMap().at(uniformName), value);
}

void Shader::setUniform(const std::string& uniformName, float value) const
{
	glUniform1f(m_shaderData->getUniformMap().at(uniformName), value);
}

void Shader::setUniform(const std::string& uniformName, const Matrix4& value) const
{
	glUniformMatrix4fv(m_shaderData->getUniformMap().at(uniformName), 1, GL_FALSE, &(value[0][0]));
}

void Shader::setUniform(const std::string& uniformName, const Vector3& value) const
{
	glUniform3f(m_shaderData->getUniformMap().at(uniformName), value.getX(), value.getY(), value.getZ());
}

void Shader::setUniform(const std::string& uniformName, const Vector4& value) const
{
	glUniform4f(m_shaderData->getUniformMap().at(uniformName), value.getX(), value.getY(), value.getZ(), value.getW());
}

void Shader::setUniformDirectionalLight(const std::string& uniformName, const DirectionalLight& light) const
{
	setUniform(uniformName + ".direction", light.getTransform().getTransformedRotation().getForward());
	setUniform(uniformName + ".base.color", light.getColour().getRGB());
	setUniform(uniformName + ".base.intensity", light.getIntensity());
}

void Shader::setUniformPointLight(const std::string& uniformName, const PointLight& light) const
{
	setUniform(uniformName + ".base.color", light.getColour().getRGB());
	setUniform(uniformName + ".base.intensity", light.getIntensity());
	setUniform(uniformName + ".atten.constant", light.getAttenuation().getConstant());
	setUniform(uniformName + ".atten.linear", light.getAttenuation().getLinear());
	setUniform(uniformName + ".atten.exponent", light.getAttenuation().getExponent());
	setUniform(uniformName + ".position", light.getTransform().getTransformedPosition());
	setUniform(uniformName + ".range", light.getRange());
}

void Shader::setUniformSpotLight(const std::string& uniformName, const SpotLight& light) const
{
	setUniform(uniformName + ".pointLight.base.color", light.getColour().getRGB());
	setUniform(uniformName + ".pointLight.base.intensity", light.getIntensity());
	setUniform(uniformName + ".pointLight.atten.constant", light.getAttenuation().getConstant());
	setUniform(uniformName + ".pointLight.atten.linear", light.getAttenuation().getLinear());
	setUniform(uniformName + ".pointLight.atten.exponent", light.getAttenuation().getExponent());
	setUniform(uniformName + ".pointLight.position", light.getTransform().getTransformedPosition());
	setUniform(uniformName + ".pointLight.range", light.getRange());
	setUniform(uniformName + ".direction", light.getTransform().getTransformedRotation().getForward());
	setUniform(uniformName + ".cutoff", light.getCutoff());
}

int ShaderData::getProgram()
{
	return m_program;
}

void ShaderData::addVertexShader(const std::string& text)
{
	addProgram(text, GL_VERTEX_SHADER);
}

void ShaderData::addGeometryShader(const std::string& text)
{
	addProgram(text, GL_GEOMETRY_SHADER);
}

void ShaderData::addFragmentShader(const std::string& text)
{
	addProgram(text, GL_FRAGMENT_SHADER);
}

void ShaderData::addProgram(const std::string& text, int type)
{
	int shader = glCreateShader(type);

	if(shader == 0)
	{
		fprintf(stderr, "error creating shader");
		exit(1);
	}

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		fprintf(stderr, "could not create shader");
		exit(1);
	}

	glAttachShader(m_program, shader);
	m_shaders.push_back(shader);
}

void ShaderData::addAllAttributes(const std::string& text)
{
	const std::string ATTRIBUTE_KEY = "attribute";

	int currentAttribLoc = 0;
	size_t attribLoc = text.find(ATTRIBUTE_KEY);

	while(attribLoc != std::string::npos)
	{
		bool isCommented = false;
		size_t lastLineEnd = text.rfind(";", attribLoc);

		if(lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = text.substr(lastLineEnd, attribLoc - lastLineEnd);
			isCommented = potentialCommentSection.find("//") != std::string::npos;
		}

		if(!isCommented)
		{
			size_t begin = attribLoc + ATTRIBUTE_KEY.length();
			size_t end = text.find(";", begin);

			std::string attribLine = text.substr(begin + 1, end - begin - 1);

			begin = attribLine.find(" ");
			std::string attribName = attribLine.substr(begin + 1);

			glBindAttribLocation(m_program, currentAttribLoc, attribName.c_str());
			currentAttribLoc++;
		}

		attribLoc = text.find(ATTRIBUTE_KEY, attribLoc + ATTRIBUTE_KEY.length());
	}
}

void ShaderData::addShaderUniforms(const std::string& text)
{
	const std::string UNIFORM_KEY = "uniform";

	std::vector<UniformStruct> structs = findUniformStructs(text);

	size_t uniformLoc = text.find(UNIFORM_KEY);

	while(uniformLoc != std::string::npos)
	{
		bool isCommented = false;

		size_t lastLineEnd = text.rfind(";", uniformLoc);

		if(lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = text.substr(lastLineEnd, uniformLoc - lastLineEnd);
			isCommented = potentialCommentSection.find("//") != std::string::npos;
		}

		if(!isCommented)
		{
			size_t begin = uniformLoc + UNIFORM_KEY.length();
			size_t end = text.find(";", begin);

			std::string uniformLine = text.substr(begin + 1, end - begin - 1);

			begin = uniformLine.find(" ");
			std::string uniformName = uniformLine.substr(begin + 1);
			std::string uniformType = uniformLine.substr(0, begin);

			m_uniformNames.push_back(uniformName);
			m_uniformTypes.push_back(uniformType);
			addUniform(uniformName, uniformType, structs);
		}

		uniformLoc = text.find(UNIFORM_KEY, uniformLoc + UNIFORM_KEY.length());
	}
}

void ShaderData::addUniform(const std::string& uniformName, const std::string& uniformType, const std::vector<UniformStruct>& structs)
{
	bool addThis = true;

	for(unsigned int i = 0; i < structs.size(); i++)
	{
		if(structs[i].name.compare(uniformType) == 0)
		{
			addThis = false;

			for(unsigned int j = 0; j < structs[i].memberNames.size(); j++)
			{
				addUniform(uniformName + "." + structs[i].memberNames[j].name, structs[i].memberNames[j].type, structs);
			}
		}
	}

	if(!addThis)
	{
		return;
	}

	unsigned int location = glGetUniformLocation(m_program, uniformName.c_str());

	assert(location != GL_INVALID_VALUE);

	m_uniformMap.insert(std::pair<std::string, unsigned int>(uniformName, location));

	if(location > 100)
	{
		int brl = 0;
	}
}

void ShaderData::compileShader()
{
	glLinkProgram(m_program);
	checkShaderError(m_program, GL_LINK_STATUS, true, "error linking shader program");

	glValidateProgram(m_program);
	checkShaderError(m_program, GL_VALIDATE_STATUS, true, "invalid shader program");
}

static void checkShaderError(int shader, int flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if(isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if(!success)
	{
		if(!isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		fprintf(stderr, "%s: '%s'\n", errorMessage.c_str(), error);
	}
}

static std::string loadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open(("./res/shaders/" + fileName).c_str());

	std::string output;
	std::string line;

	if(file.is_open())
	{
		while(file.good())
		{
			getline(file, line);

			if(line.find("#include") == std::string::npos)
			{
				output.append(line + "\n");
			}
			else
			{
				std::string includeFileName = Util::split(line, ' ')[1];
				includeFileName = includeFileName.substr(1,includeFileName.length() - 2);

				std::string toAppend = loadShader(includeFileName);
				output.append(toAppend + "\n");
			}
		}
	}
	else
	{
		std::cout << "Unable to load shader " << fileName << std::endl;
	}

	return output;
}

static std::vector<TypedData> findUniformStructComponents(const std::string& openingBraceToClosingBrace)
{
	static const char charsToIgnore[] = { ' ', '\n', '\t', '{' };
	static const size_t UNSIGNED_NEG_ONE = (size_t)-1;

	std::vector<TypedData> result;
	std::vector<std::string> structLines = Util::split(openingBraceToClosingBrace, ';');

	for(unsigned int i = 0; i < structLines.size(); i++)
	{
		size_t nameBegin = UNSIGNED_NEG_ONE;
		size_t nameEnd = UNSIGNED_NEG_ONE;

		for(unsigned int j = 0; j < structLines[i].length(); j++)
		{
			bool isIgnoreChar = false;

			for(int k = 0; k < sizeof(charsToIgnore) / sizeof(char); k++)
			{
				if(structLines[i][j] == charsToIgnore[k])
				{
					isIgnoreChar = true;
					break;
				}
			}

			if(nameBegin == UNSIGNED_NEG_ONE && !isIgnoreChar)
			{
				nameBegin = j;
			}
			else if(nameBegin != UNSIGNED_NEG_ONE && isIgnoreChar)
			{
				nameEnd = j;
				break;
			}
		}

		if(nameBegin == UNSIGNED_NEG_ONE || nameEnd == UNSIGNED_NEG_ONE)
		{
			continue;
		}

		TypedData newData;
		newData.type = structLines[i].substr(nameBegin, nameEnd - nameBegin);
		newData.name = structLines[i].substr(nameEnd + 1);

		result.push_back(newData);
	}

	return result;
}

static std::string findUniformStructName(const std::string& structStartToOpeningBrace)
{
	return Util::split(Util::split(structStartToOpeningBrace, ' ')[0], '\n')[0];
}

static std::vector<UniformStruct> findUniformStructs(const std::string& text)
{
	static const std::string STRUCT_KEY = "struct";

	std::vector<UniformStruct> result;
	size_t structLoc = text.find(STRUCT_KEY);

	while(structLoc != std::string::npos)
	{
		structLoc += STRUCT_KEY.length() + 1;

		size_t braceOpen = text.find("{", structLoc);
		size_t braceClose = text.find("}", braceOpen);

		UniformStruct newStruct;
		newStruct.name = findUniformStructName(text.substr(structLoc, braceOpen - structLoc));
		newStruct.memberNames = findUniformStructComponents(text.substr(braceOpen, braceClose - braceOpen));

		result.push_back(newStruct);
		structLoc = text.find(STRUCT_KEY, structLoc);
	}

	return result;
}