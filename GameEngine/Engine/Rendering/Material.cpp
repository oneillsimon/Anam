#include "Material.h"

//TODO: change these base constructors to a more chain like structure

Material::Material(Texture* diffuse, float specularIntensity, float specularExponent)
{
	setTexture("diffuse", diffuse);
	setFloat("specularIntensity", specularIntensity);
	setFloat("specularExponent", specularExponent);
	setTexture("normalMap", new Texture("default_normal.jpg"));
	setTexture("displacementMap", new Texture("default_disp.png"));

	float baseBias = 0.0f / 2.0f;
	setFloat("displacementMapBias", -baseBias + baseBias * 0.0f);
}

Material::Material(Texture* diffuse, float specularIntensity, float specularExponent, Texture* normalMap)
{
	setTexture("diffuse", diffuse);
	setFloat("specularIntensity", specularIntensity);
	setFloat("specularExponent", specularExponent);
	setTexture("normalMap", normalMap);
	setTexture("displacementMap", new Texture("default_disp.png"));

	float baseBias = 0.0f / 2.0f;
	setFloat("displacementMapBias", -baseBias + baseBias * 0.0f);
}

Material::Material(Texture* diffuse, float specularIntensity, float specularExponent, Texture* normalMap, Texture* displacementMap, float displacementMapScale, float displacementMapOffset)
{
	setTexture("diffuse", diffuse);
	setFloat("specularIntensity", specularIntensity);
	setFloat("specularExponent", specularExponent);
	setTexture("normalMap", normalMap);
	setTexture("displacementMap", displacementMap);
	setFloat("displacementMapScale", displacementMapScale);

	float baseBias = displacementMapScale / 2.0f;
	setFloat("displacementMapBias", -baseBias + baseBias * displacementMapOffset);
}