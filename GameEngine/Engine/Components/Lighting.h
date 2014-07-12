#pragma once

#include "../Core/3DMath.h"
#include "GameComponent.h"

class CoreEngine;

class Light : public GameComponent
{
private:
	Shader* m_shader;
	void operator =(Light& light) {}

public:
	Vector3 m_colour;
	float m_intensity;

	Light(const Vector3& colour, float intensity);
	virtual ~Light();

	virtual void addToCoreEngine(CoreEngine* coreEngine);

	Shader* getShader();
	void setShader(Shader* shader);
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;

	Attenuation(float constant = 0, float linear = 0, float exponent = 1) : constant(constant), linear(linear), exponent(exponent){}
};

struct DirectionalLight : public Light
{
	DirectionalLight(const Vector3& colour, float intensity);
};

struct PointLight : public Light
{
	Attenuation attenuation;
	float range;

	PointLight(const Vector3& colour, float intensity, const Attenuation& attenuation);
};

struct SpotLight : public PointLight
{
	float cutoff;

	SpotLight(const Vector3& colour, float intensity, const Attenuation& attenuation, float cutoff);
};