#include "MappedValues.h"

void MappedValues::setVector3(const std::string& name, Vector3 v)
{
	vector3Map[name] = v;
}

void MappedValues::setVector4(const std::string& name, Vector4 v)
{
	vector4Map[name] = v;
}

void MappedValues::setFloat(const std::string& name, float f)
{
	floatMap[name] = f;
}

void MappedValues::setTexture(const std::string& name, const Texture& t)
{
	textureMap[name] = t;
}

const Vector3& MappedValues::getVector3(const std::string& name) const
{
	std::map<std::string, Vector3>::const_iterator it = vector3Map.find(name);

	if(it != vector3Map.end())
	{
		return it->second;
	}

	return Vector3::ZERO;
}

const Vector4& MappedValues::getVector4(const std::string& name) const
{
	std::map<std::string, Vector4>::const_iterator it = vector4Map.find(name);

	if(it != vector4Map.end())
	{
		return it->second;
	}
	
	return Vector4();
}

float MappedValues::getFloat(const std::string& name) const
{
	std::map<std::string, float>::const_iterator it = floatMap.find(name);

	if(it != floatMap.end())
	{
		return it->second;
	}

	return 0.0f;
}

const Texture& MappedValues::getTexture(const std::string& name) const
{
	std::map<std::string, Texture>::const_iterator it = textureMap.find(name);

	if(it != textureMap.end())
	{
		return it->second;
	}

	return 0;
}