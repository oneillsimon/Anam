#include "Material.h"

Material::Material(Texture* diffuse, float specularIntensity, float specularExponent)
{
	addTexture("diffuse", diffuse);
	addFloat("specularIntensity", specularIntensity);
	addFloat("specularExponent", specularExponent);
	addTexture("normalMap", new Texture("default_normal.jpg"));
}

Material::Material(Texture* diffuse, float specularIntensity, float specularExponent, Texture* normalMap)
{
	addTexture("diffuse", diffuse);
	addFloat("specularIntensity", specularIntensity);
	addFloat("specularExponent", specularExponent);
	addTexture("normalMap", normalMap);
}

Material::~Material()
{
	std::map<std::string, Texture*>::iterator it;

	for(it = textureMap.begin(); it != textureMap.end(); it++)
	{
		if(it->second)
		{
			delete it->second;
		}
	}
}

void Material::addTexture(const std::string& name, Texture* texture)
{
	textureMap.insert(std::pair<std::string, Texture*>(name, texture));
}

Texture* Material::getTexture(const std::string& name) const
{
	std::map<std::string, Texture*>::const_iterator it = textureMap.find(name);

	if(it != textureMap.end())
	{
		return it->second;
	}

	return 0;
}