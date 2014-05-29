#pragma once

#include <string>
#include <map>
#include "../Core/3DMath.h"

class MappedValues
{
private:
	std::map<std::string, Vector3> vector3Map;
	std::map<std::string, float> floatMap;

public:
	void addVector3(std::string name, Vector3 v);
	void addFloat(std::string name, float f);

	Vector3 getVector3(std::string name);
	float getFloat(std::string name);
};