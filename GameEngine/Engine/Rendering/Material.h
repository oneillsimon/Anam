#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>

#include "MappedValues.h"
#include "Colour.h"

#define MATERIAL_DEFAULT Material("defaultMaterial", TEXTURE_BLANK)

class MaterialData : public ReferenceCounter, public MappedValues
{
public:
	Colour getColour(const std::string& name) const;
private:
};

class Material
{
protected:
	static std::map<std::string, MaterialData*> s_resourceMap;
	MaterialData* m_materialData;
	std::string m_materialName;
	Colour m_colour;

public:
	Material(const std::string& materialName = "");
	Material(const Material& material);
	Material(const std::string& materialName,
			 const Texture& diffuse,
			 Colour colour = Colour(255, 255, 255),
			 float specularIntensity = 1.0f,
			 float specularExponent = 8.0f,
			 const Texture& normalMap = Texture("default_normal.jpg"),
			 const Texture& displacementMap = Texture("default_disp.png"),
			 float dispMapScale = 0.0f,
			 float dispMapOffset = 0.0f);
	virtual ~Material();

	void setVector3(const std::string& name, const Vector3& value);
	void setFloat(const std::string& name, const float value);
	void setTexture(const std::string& name, const Texture& value);
	void setVector4(const std::string& name, const Vector4& colour);

	Vector3 getVector3(const std::string& name) const;
	float getFloat(const std::string& name) const;
	Texture getTexture(const std::string& name) const;
	Vector4 getVector4(const std::string& name) const;
	Colour getColour(const std::string& name) const;
	Texture getDiffuseTexture() const;

	void operator =(Material& material) {}
};

#endif