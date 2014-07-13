#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include "Texture.h"
#include "MappedValues.h"
#include "../Core/Math3D.h"

class Material : public MappedValues
{
private:
	std::map<std::string, Texture*> textureMap;

	void operator =(Material& material) {}

public:
	Material(Texture* diffuse, float specularIntensity, float specularExponent);
	~Material();

	void addTexture(std::string name, Texture* texture);

	Texture* getTexture(std::string name);
};

#endif