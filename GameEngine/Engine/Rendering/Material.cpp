#include "Material.h"

Material::Material(Texture* diffuse, float specularIntensity, float specularExponent)
{
	addTexture("diffuse", diffuse);
	addFloat("specularIntensity", specularIntensity);
	addFloat("specularExponent", specularExponent);
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

void Material::addTexture(std::string name, Texture* texture)
{
	textureMap.insert(std::pair<std::string, Texture*>(name, texture));
}

Texture* Material::getTexture(std::string name)
{
	std::map<std::string, Texture*>::const_iterator it = textureMap.find(name);

	if(it != textureMap.end())
	{
		return it->second;
	}

	return 0;
}