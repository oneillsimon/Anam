#pragma once

#include "../Core/3DMath.h"
#include "../Rendering/Colour.h"
#include "GameComponent.h"

class CoreEngine;

class Light : public GameComponent
{
private:
	Shader* m_shader;
	Colour m_colour;
	float m_intensity;

	void operator =(Light& light) {}

public:
	Light(const Colour& colour, float intensity);
	virtual ~Light();

	virtual void addToCoreEngine(CoreEngine* coreEngine);

	Shader* getShader();
	Colour getColour() const;
	float getIntensity() const;

	void setShader(Shader* shader);
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
public:
	DirectionalLight(const Colour& colour, float intensity);
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
	float m_cutoff;

public:
	SpotLight(const Colour& colour, float intensity, const Attenuation& attenuation, float cutoff);

	float getCutoff() const;
	
	void setCutoff(float cutoff);
};