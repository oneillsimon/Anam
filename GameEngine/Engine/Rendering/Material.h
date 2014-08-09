#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include "Texture.h"
#include "MappedValues.h"
#include "../Core/Math3D.h"

class MaterialData : public ReferenceCounter, public MappedValues
{
public:
private:
};

class Material
{
private:
	static std::map<std::string, MaterialData*> s_resourceMap;
	MaterialData* m_materialData;
	std::string m_materialName;

public:
	Material(const std::string& materialName = "");
	Material(const Material& material);
	Material(const std::string& materialName,
			 const Texture& diffuse,
			 float specularIntensity,
			 float specularExponent,
			 const Texture& normalMap = Texture("default_normal.jpg"),
			 const Texture& displacementMap = Texture("default_disp.png"),
			 float dispMapScale = 0.0f,
			 float dispMapOffset = 0.0f);
	virtual ~Material();

	void setVector3(const std::string& name, const Vector3& value);
	void setFloat(const std::string& name, const float value);
	void setTexture(const std::string& name, const Texture& value);

	const Vector3& getVector3(const std::string& name) const;
	float getFloat(const std::string& name) const;
	const Texture& getTexture(const std::string& name) const;

	void operator =(Material& material) {}
};

#endif