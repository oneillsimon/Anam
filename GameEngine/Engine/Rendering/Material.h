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
	Material(Texture* diffuse, float specularIntensity, float specularExponent, Texture* normalMap);
	Material(Texture* diffuse, float specularIntensity, float specularExponent, Texture* normalMap, Texture* displacementMap, float displacementMapScale, float displacementMapOffset);
	~Material();

	void addTexture(const std::string& name, Texture* texture);

	Texture* getTexture(const std::string& name) const;
};

#endif