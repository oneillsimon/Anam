#ifndef MAPPEDVALUES_H
#define MAPPEDVALUES_H

#include <map>

#include "../Core/Math3D.h"
#include "Texture.h"

class MappedValues
{
protected:
	std::map<std::string, Vector3> vector3Map;
	std::map<std::string, Vector4> vector4Map;
	std::map<std::string, float> floatMap;
	std::map<std::string, Texture> textureMap;

public:
	void setVector3(const std::string& name, Vector3 v);
	void setVector4(const std::string& name, Vector4 v);
	void setFloat(const std::string& name, float f);
	void setTexture(const std::string& name, const Texture& t);

	const Vector3& getVector3(const std::string& name) const;
	const Vector4& getVector4(const std::string& name) const;
	float getFloat(const std::string& name) const;
	const Texture& getTexture(const std::string& name) const;
};

#endif