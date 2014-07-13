#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <vector>
#include <string>

#include "ReferenceCounter.h"
#include "../Core/Math3D.h"
#include "Material.h"
#include "RenderingEngine.h"
#include "Colour.h"
#include "../Components/Lighting.h"
#include "../Core/Transform.h"

struct TypedData
{
	std::string name;
	std::string type;
};

struct UniformStruct
{
	std::string name;
	std::vector<TypedData> memberNames;
};

class ShaderData : public ReferenceCounter
{
private:
	int m_program;
	std::vector<int> m_shaders;
	std::vector<std::string> m_uniformNames;
	std::vector<std::string> m_uniformTypes;
	std::map<std::string, unsigned int> m_uniformMap;

	void addVertexShader(const std::string& text);
	void addGeometryShader(const std::string& text);
	void addFragmentShader(const std::string& text);
	void addProgram(const std::string& text, int type);

	void addAllAttributes(const std::string& shaderText);
	void addShaderUniforms(const std::string& shaderText);
	void addUniform(const std::string& uniformName, const std::string& uniformType, std::vector<UniformStruct> structs);
	void compileShader();
public:
	ShaderData(const std::string& fileName);
	~ShaderData();

	int getProgram();
	std::vector<int>& getShaders();
	std::vector<std::string>& getUniformNames();
	std::vector<std::string>& getUniformTypes();
	std::map<std::string, unsigned int>& getUniformMap();
};

class Shader
{
private:
	static std::map<std::string, ShaderData*> m_resourceMap;

	ShaderData* m_shaderData;
	std::string m_fileName;

	void setUniformDirectionalLight(const std::string& uniformName, const DirectionalLight& light);
	void setUniformPointLight(const std::string& uniformName, const PointLight& light);
	void setUniformSpotLight(const std::string& uniformName, const SpotLight& light);

public:
	Shader(const std::string& filenName);
	virtual ~Shader();

	void bind();
	void updateUniforms(const Transform& transform, Material& material, Vector4& colour, RenderingEngine* renderingEngine);

	void setUniform(const std::string& fileName, int value);
	void setUniform(const std::string& fileName, float value);
	void setUniform(const std::string& fileName, const Matrix4& value);
	void setUniform(const std::string& fileName, const Vector3& value);
	void setUniform(const std::string& fileName, const Vector4& value);
};

#endif