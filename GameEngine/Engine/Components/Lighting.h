#ifndef LIGHTING_H
#define LIGHTING_H

#include "../Core/Math3D.h"
#include "../Rendering/Shader.h"
#include "GameComponent.h"

class CoreEngine;

class ShadowInfo
{
private:
	Matrix4 m_projection;
	bool m_flipFaces;
	float m_shadowSoftness;
	float m_lightBleedReductionAmount;
	float m_varianceMin;
	int m_shadowMapSizeAsPowerOf2;

public:
	ShadowInfo(const Matrix4& projection = Matrix4().initIdentity(), bool flipFaces = false, int shadowMapSizeAsPowerOf2 = 0, float shadowSoftness = 1.0f, float lightBleedReductionAmount = 0.3f, float minVariance = 0.00002f);

	Matrix4 getProjection() const;
	bool getFlipFaces() const;
	float getShadowSoftness() const;
	float getLightBleedReductionAmount() const;
	float getVarianceMin() const;
	int getShadowMapSizeAsPowerOf2() const;
};

struct ShadowCameraTransform
{
	Vector3 position;
	Quaternion rotation;
};

class Light : public GameComponent
{
private:
	Shader m_shader;
	ShadowInfo m_shadowInfo;
	Colour m_colour;
	float m_intensity;

	void operator =(Light& light) {}

protected:
	void setShadowInfo(const ShadowInfo& shadowInfo);

public:
	Light(const Colour& colour, float intensity, const Shader& shader);
	virtual ~Light();

	virtual void addToCoreEngine(CoreEngine* coreEngine) const;
	virtual ShadowCameraTransform calculateShadowCameraTransform(const Vector3& mainCameraPosition, const Quaternion& mainCamerRotation) const;

	const Shader& getShader() const;
	const ShadowInfo& getShadowInfo() const;
	const Colour getColour() const;
	const float getIntensity() const;

	void setColour(const Colour& colour);
	void setIntensity(float intensity);
};

class Attenuation
{
private:
	float m_constant;
	float m_linear;
	float m_exponent;

public:
	Attenuation(float constant = 0, float linear = 0, float exponent = 1);

	float getConstant() const;
	float getLinear() const;
	float getExponent() const;

	void setConstant(float constant);
	void setLinear(float linear);
	void setExponent(float exponent);
};

class DirectionalLight : public Light
{
private:
	float m_halfShadowArea;

public:
	DirectionalLight(const Colour& colour, float intensity, int shadowMapSizedAsPowerOf2 = 10, float shadowArea = 80, float shadowSoftness = 0.25f, float lightBleedReductionAmount = 0.2f, float minVariance = 0.00002f);

	virtual ShadowCameraTransform calculateShadowCameraTransform(const Vector3& mainCameraPosition, const Quaternion& mainCamerRotation) const;
};

class PointLight : public Light
{
private:
	Attenuation m_attenuation;
	float m_range;

protected:
	PointLight(const Colour& colour, float intensity, const Attenuation& attenuation, const Shader& shader);
public:
	PointLight(const Colour& colour, float intensity, const Attenuation& attenuation);

	Attenuation getAttenuation() const;
	float getRange() const;

	void setAttenuation(const Attenuation& attenuation);
	void setRange(float range);
};

class SpotLight : public PointLight
{
private:
	float m_fov;

public:
	SpotLight(const Colour& colour, float intensity, const Attenuation& attenuation, float fov, int shadowMapSizedAsPowerOf2 = 0, float shadowSoftness = 0.25f, float lightBleedReductionAmount = 0.2f, float minVariance = 0.00002f);

	float getCutoff() const;
};

#endif