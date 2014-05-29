#include "../Core/CoreEngine.h"
#include "../Rendering/RenderingEngine.h"
#include "../Rendering/Shader.h"
#include "Lighting.h"

#define COLOUR_DEPTH 256

Light::~Light()
{
	if(m_shader)
	{
		delete m_shader;
	}
}

void Light::addToCoreEngine(CoreEngine* coreEngine)
{
	coreEngine->getRenderingEngine()->addLight(this);
}

void Light::setShader(Shader* shader)
{
	if(m_shader)
	{
		delete m_shader;
	}

	this->m_shader = shader;
}

DirectionalLight::DirectionalLight(const Vector3& colour, float intensity)
	: Light(colour, intensity)
{
	setShader(new Shader("forward-directional"));
}

PointLight::PointLight(const Vector3& colour, float intensity, const Attenuation& attenuation)
	: Light(colour, intensity)
{
	float a = attenuation.exponent;
	float b = attenuation.linear;
	float c = attenuation.constant - COLOUR_DEPTH * intensity * colour.max();

	range = (-b + sqrtf(b * b - 4 * a * c)) / ( 2 * a);

	setShader(new Shader("forward-point"));
}

SpotLight::SpotLight(const Vector3& colour, float intensity, const Attenuation& attenuation, float cutoff)
	: PointLight(colour, intensity, attenuation), cutoff(cutoff)
{
	setShader(new Shader("forward-spot"));
}

Shader* Light::getShader()
{
	return m_shader;
}