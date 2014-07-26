#ifndef LIGHTING_H
#define LIGHTING_H

#include "../Core/Math3D.h"
#include "../Rendering/Colour.h"
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
	ShadowInfo(const Matrix4& projection, bool flipFaces, int shadowMapSizeAsPowerOf2, float shadowSoftness, float lightBleedReductionAmount, float minVariance);

	Matrix4 getProjection();
	bool getFlipFaces();
	float getShadowSoftness();
	float getLightBleedReductionAmount();
	float getVarianceMin();
	int getShadowMapSizeAsPowerOf2();
};

struct ShadowCameraTransform
{
	Vector3 position;
	Quaternion rotation;
};

class Light : public GameComponent
{
private:
	Shader* m_shader;
	ShadowInfo* m_shadowInfo;
	Colour m_colour;
	float m_intensity;

	void operator =(Light& light) {}

protected:
	void setShadowInfo(ShadowInfo* shadowInfo);
	void setShader(Shader* shader);

public:
	Light(const Colour& colour, float intensity);
	virtual ~Light();

	virtual void addToCoreEngine(CoreEngine* coreEngine);
	virtual ShadowCameraTransform calculateShadowCameraTransform(const Vector3& mainCameraPosition, const Quaternion& mainCamerRotation);

	Shader* getShader();
	ShadowInfo* getShadowInfo();
	Colour getColour() const;
	float getIntensity() const;

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

	virtual ShadowCameraTransform calculateShadowCameraTransform(const Vector3& mainCameraPosition, const Quaternion& mainCamerRotation);
};

class PointLight : public Light
{
private:
	Attenuation m_attenuation;
	float m_range;

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
	//, true, 0.25f, 0.2f, 0.00002f))
	SpotLight(const Colour& colour, float intensity, const Attenuation& attenuation, float fov, int shadowMapSizedAsPowerOf2 = 0, float shadowSoftness = 0.25f, float lightBleedReductionAmount = 0.2f, float minVariance = 0.00002f);

	float getCutoff() const;
};

#endif