#include "../Core/Math3D.h"
#include "MappedValues.h"

void MappedValues::addVector3(std::string name, Vector3 v)
{
	vector3Map.insert(std::pair<std::string, Vector3>(name, v));
}

void MappedValues::addVector4(std::string name, Vector4 v)
{
	vector4Map.insert(std::pair<std::string, Vector4>(name, v));
}

void MappedValues::addFloat(std::string name, float f)
{
	floatMap.insert(std::pair<std::string, float>(name, f));
}

Vector3 MappedValues::getVector3(std::string name)
{
	std::map<std::string, Vector3>::const_iterator it = vector3Map.find(name);

	if(it != vector3Map.end())
	{
		return it->second;
	}

	return Vector3::ZERO;
}

Vector4 MappedValues::getVector4(std::string name)
{
	std::map<std::string, Vector4>::const_iterator it = vector4Map.find(name);

	if(it != vector4Map.end())
	{
		return it->second;
	}

	return Vector4(0, 0, 0, 0);
}

float MappedValues::getFloat(std::string name)
{
	std::map<std::string, float>::const_iterator it = floatMap.find(name);

	if(it != floatMap.end())
	{
		return it->second;
	}

	return 0.0f;
}