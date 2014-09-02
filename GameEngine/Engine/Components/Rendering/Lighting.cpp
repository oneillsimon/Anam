#include "../../Core/CoreEngine.h"
#include "Lighting.h"

#define COLOUR_DEPTH 256

ShadowInfo::ShadowInfo(const Matrix4& projection, bool flipFaces, int shadowMapSizeAsPowerOf2, float shadowSoftness, float lightBleedReductionAmount, float minVariance)
{
	m_projection = projection;
	m_flipFaces = flipFaces;
	m_shadowSoftness = shadowSoftness;
	m_lightBleedReductionAmount = lightBleedReductionAmount;
	m_varianceMin = minVariance;
	m_shadowMapSizeAsPowerOf2 = shadowMapSizeAsPowerOf2;
}

Matrix4 ShadowInfo::getProjection() const
{
	return m_projection;
}

bool ShadowInfo::getFlipFaces() const
{
	return m_flipFaces;
}

float ShadowInfo::getShadowSoftness() const
{
	return m_shadowSoftness;
}

float ShadowInfo::getLightBleedReductionAmount() const
{
	return m_lightBleedReductionAmount;
}

float ShadowInfo::getVarianceMin() const
{
	return m_varianceMin;
}

int ShadowInfo::getShadowMapSizeAsPowerOf2() const
{
	return m_shadowMapSizeAsPowerOf2;
}

Light::Light(const Colour& colour, float intensity, const Shader& shader) :
	m_colour(colour),
	m_intensity(intensity),
	m_shader(shader),
	m_shadowInfo(ShadowInfo())
{
}

Light::~Light()
{
}

void Light::addToCoreEngine(CoreEngine* coreEngine) const
{
	coreEngine->getRenderingEngine()->addLight(*this);
}

ShadowCameraTransform Light::calculateShadowCameraTransform(const Vector3& mainCameraPosition, const Quaternion& mainCameraRotation) const
{
	ShadowCameraTransform result;
	result.position = getTransform().getTransformedPosition();
	result.rotation = getTransform().getTransformedRotation();
	return result;
}

const Shader& Light::getShader() const
{
	return m_shader;
}

const ShadowInfo& Light::getShadowInfo() const
{
	return m_shadowInfo;
}

const Colour Light::getColour() const
{
	return m_colour;
}

const float Light::getIntensity() const
{
	return m_intensity;
}

void Light::setShadowInfo(const ShadowInfo& shadowInfo)
{
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

DirectionalLight::DirectionalLight(const Colour& colour, float intensity, int shadowMapSizedAsPowerOf2, float shadowArea, float shadowSoftness, float lightBleedReductionAmount, float minVariance)
	: Light(colour, intensity, Shader("forward-directional"))
{
	m_halfShadowArea = shadowArea / 2;

	if(shadowMapSizedAsPowerOf2 != 0)
	{
		setShadowInfo(ShadowInfo(Matrix4().initOrthographic(-m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea), true, shadowMapSizedAsPowerOf2, shadowSoftness, lightBleedReductionAmount, minVariance));
	}
}

ShadowCameraTransform DirectionalLight::calculateShadowCameraTransform(const Vector3& mainCameraPosition, const Quaternion& mainCameraRotation) const
{
	ShadowCameraTransform result;
	result.position = mainCameraPosition + mainCameraRotation.getForward() * m_halfShadowArea;
	result.rotation = getTransform().getTransformedRotation();

	float worldSpaceShadowMapTexelSize = (m_halfShadowArea * 2) / (float)(1 << getShadowInfo().getShadowMapSizeAsPowerOf2());

	Vector3 altCameraPos = result.position.rotate(result.rotation.conjugate());
	
	altCameraPos.setX(worldSpaceShadowMapTexelSize * floor(altCameraPos.getX() / worldSpaceShadowMapTexelSize));
	altCameraPos.setY(worldSpaceShadowMapTexelSize * floor(altCameraPos.getY() / worldSpaceShadowMapTexelSize));

	result.position = altCameraPos.rotate(result.rotation);

	return result;
}

PointLight::PointLight(const Colour& colour, float intensity, const Attenuation& attenuation)
	: Light(colour, intensity, Shader("forward-point"))
{
	m_attenuation = attenuation;

	float a = attenuation.getExponent();
	float b = attenuation.getLinear();
	float c = attenuation.getConstant() - COLOUR_DEPTH * intensity * colour.max();

	m_range = (-b + sqrtf(b * b - 4 * a * c)) / ( 2 * a);
}

PointLight::PointLight(const Colour& colour, float intensity, const Attenuation& attenuation, const Shader& shader)
	: Light(colour, intensity, shader)
{
	m_attenuation = attenuation;

	float a = attenuation.getExponent();
	float b = attenuation.getLinear();
	float c = attenuation.getConstant() - COLOUR_DEPTH * intensity * colour.max();

	m_range = (-b + sqrtf(b * b - 4 * a * c)) / ( 2 * a);
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

SpotLight::SpotLight(const Colour& colour, float intensity, const Attenuation& attenuation, float fov, int shadowMapSizedAsPowerOf2, float shadowSoftness, float lightBleedReductionAmount, float minVariance)
	: PointLight(colour, intensity, attenuation, Shader("forward-spot"))
{
	m_fov = fov;

	if(shadowMapSizedAsPowerOf2 != 0)
	{
		setShadowInfo(ShadowInfo(Matrix4().initPerspective(m_fov, 1.0f, 0.1f, getRange()), false, shadowMapSizedAsPowerOf2, shadowSoftness, lightBleedReductionAmount, minVariance));
	}
}

float SpotLight::getCutoff() const
{
	return cos(m_fov / 2);
}