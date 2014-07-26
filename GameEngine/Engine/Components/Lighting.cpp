#include "../Core/CoreEngine.h"
#include "../Rendering/RenderingEngine.h"
#include "../Rendering/Shader.h"
#include "Lighting.h"

#define COLOUR_DEPTH 256

ShadowInfo::ShadowInfo(const Matrix4& projection)
{
	m_projection = projection;
}

Matrix4 ShadowInfo::getProjection()
{
	return m_projection;
}

Light::Light(const Colour& colour, float intensity)
{
	m_shader = 0;
	m_shadowInfo = 0;
	m_colour = colour;
	m_intensity = intensity;
}

Light::~Light()
{
	if(m_shader)
	{
		delete m_shader;
	}

	if(m_shadowInfo)
	{
		delete m_shadowInfo;
	}
}

void Light::addToCoreEngine(CoreEngine* coreEngine)
{
	coreEngine->getRenderingEngine()->addLight(this);
}

Shader* Light::getShader()
{
	return m_shader;
}

ShadowInfo* Light::getShadowInfo()
{
	return m_shadowInfo;
}

Colour Light::getColour() const
{
	return m_colour;
}

float Light::getIntensity() const
{
	return m_intensity;
}

void Light::setShader(Shader* shader)
{
	if(m_shader)
	{
		delete m_shader;
	}

	this->m_shader = shader;
}

void Light::setShadowInfo(ShadowInfo* shadowInfo)
{
	if(m_shadowInfo)
	{
		delete m_shadowInfo;
	}

	m_shadowInfo = shadowInfo;
}

void Light::setColour(const Colour& colour)
{
	m_colour = colour;
}

void Light::setIntensity(float intensity)
{
	m_intensity = intensity;
}

Attenuation::Attenuation(float constant, float linear, float exponent)
{
	m_constant = constant;
	m_linear = linear;
	m_exponent = exponent;
}

float Attenuation::getConstant() const
{
	return m_constant;
}

float Attenuation::getLinear() const
{
	return m_linear;
}

float Attenuation::getExponent() const
{
	return m_exponent;
}

void Attenuation::setConstant(float constant)
{
	m_constant = constant;
}

void Attenuation::setLinear(float linear)
{
	m_linear = linear;
}

void Attenuation::setExponent(float exponent)
{
	m_exponent = exponent;
}

DirectionalLight::DirectionalLight(const Colour& colour, float intensity)
	: Light(colour, intensity)
{
	setShader(new Shader("forward-directional"));
	setShadowInfo(new ShadowInfo(Matrix4().initOrthographic(-40, 40, -40, 40, -40, 40)));
}

PointLight::PointLight(const Colour& colour, float intensity, const Attenuation& attenuation)
	: Light(colour, intensity)
{
	m_attenuation = attenuation;

	float a = attenuation.getExponent();
	float b = attenuation.getLinear();
	float c = attenuation.getConstant() - COLOUR_DEPTH * intensity * colour.max();

	m_range = (-b + sqrtf(b * b - 4 * a * c)) / ( 2 * a);

	setShader(new Shader("forward-point"));
}

Attenuation PointLight::getAttenuation() const
{
	return m_attenuation;
}

float PointLight::getRange() const
{
	return m_range;
}

void PointLight::setAttenuation(const Attenuation& attenuation)
{
	m_attenuation = attenuation;
}

void PointLight::setRange(float range)
{
	m_range = range;
}

SpotLight::SpotLight(const Colour& colour, float intensity, const Attenuation& attenuation, float cutoff)
	: PointLight(colour, intensity, attenuation)
{
	m_cutoff = cutoff;

	setShader(new Shader("forward-spot"));
}

float SpotLight::getCutoff() const
{
	return m_cutoff;
}

void SpotLight::setCutoff(float cutoff)
{
	m_cutoff = cutoff;
}