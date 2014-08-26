#include "Colour.h"
#include "Material.h"

std::map<std::string, MaterialData*> Material::s_resourceMap;

const Colour& MaterialData::getColour(const std::string& name) const
{
	std::map<std::string, Vector4>::const_iterator it = vector4Map.find(name);

	if(it != vector4Map.end())
	{
		Vector4 v = it->second;
		return Colour(v.getX(), v.getY(), v.getZ(), v.getW());
	}
	
	return Colour();
}

Material::Material(const std::string& materialName) :
	m_materialName(materialName)
{
	if(materialName.length() > 0)
	{
		std::map<std::string, MaterialData*>::const_iterator it = s_resourceMap.find(materialName);

		if(it == s_resourceMap.end())
		{
			fprintf(stderr, "Material has not been initialised");
		}

		m_materialData = it->second;
		m_materialData->addReference();
	}
}

Material::Material(const Material& material)
{
	m_materialData = material.m_materialData;
	m_materialName = material.m_materialName;

	m_materialData->addReference();
}

Material::Material(const std::string& materialName,
				   const Texture& diffuse,
				   float specularIntensity,
				   float specularExponent,
				   const Texture& normalMap,
				   const Texture& displacementMap,
				   float dispMapScale,
				   float dispMapOffset,
				   Colour colour)
{
	m_materialName = materialName;

	m_materialData = new MaterialData();
	s_resourceMap[m_materialName] = m_materialData;

	m_materialData->setTexture("diffuse", diffuse);
	m_materialData->setFloat("specularIntensity", specularIntensity);
	m_materialData->setFloat("specularExponent", specularExponent);
	m_materialData->setTexture("normalMap", normalMap);
	m_materialData->setTexture("displacementMap", displacementMap);

	float bias = dispMapScale / 2.0f;
	m_materialData->setFloat("displacementMapScale", dispMapScale);
	m_materialData->setFloat("displacementMapBias", -bias + bias * dispMapOffset);
	m_materialData->setVector4("colour", colour);
}

Material::~Material()
{
	if(m_materialData && m_materialData->removeReference())
	{
		if(m_materialName.length() > 0)
		{
			s_resourceMap.erase(m_materialName);
		}

		delete m_materialData;
	}
}

void Material::setVector3(const std::string& name, const Vector3& value)
{
	m_materialData->setVector3(name, value);
}

void Material::setFloat(const std::string& name, float value)
{
	m_materialData->setFloat(name, value);
}

void Material::setTexture(const std::string& name, const Texture& value)
{
	m_materialData->setTexture(name, value);
}

void Material::setVector4(const std::string& name, const Vector4& value)
{
	m_materialData->setVector4(name, value);
}

const Vector3& Material::getVector3(const std::string& name) const
{
	return m_materialData->getVector3(name);
}

float Material::getFloat(const std::string& name) const
{
	return m_materialData->getFloat(name);
}

const Texture& Material::getTexture(const std::string& name) const
{
	return m_materialData->getTexture(name);
}

const Vector4& Material::getVector4(const std::string& name) const
{
	return m_materialData->getVector4(name);
}

const Colour& Material::getColour(const std::string& name) const
{
	return m_materialData->getColour(name);
}

const Texture& Material::getDiffuseTexture() const
{
	return getTexture("diffuse");
}