#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include <vector>
#include <map>
#include "../Components/Camera.h"
#include "../Components/Lighting.h"
#include "MappedValues.h"
#include "Material.h"
#include "Mesh.h"
#include "Window.h"
#include "../Core/Profiling.h"

class GameObject;

class RenderingEngine : public MappedValues
{
private:
	static const int NUM_SHADOW_MAPS = 10;
	static const Matrix4 BIAS_MATRIX;

	Transform m_planeTransform;
	Mesh m_planeMesh;

	const Window* m_window;
	Texture m_tempTarget;
	Material m_planeMaterial;
	Texture m_shadowMaps[NUM_SHADOW_MAPS];
	Texture m_shadowMapTempTargets[NUM_SHADOW_MAPS];

	Shader m_defaultShader;
	Shader m_shadowMapShader;
	Shader m_nullFilter;
	Shader m_gausBlurFilter;
	Shader m_fxaaFilter;
	Matrix4 m_lightMatrix;

	Transform m_altCameraTransform;
	Camera m_altCamera;
	const Light* m_activeLight;
	std::vector<const Light*> m_lights;
	std::map<std::string, unsigned int> m_samplerMap;

	ProfileTimer m_renderProfileTimer;
	ProfileTimer m_windowSyncProfileTimer;

	void blurShadowMap(int shadowMapIndex, float blur);
	void applyFilter(const Shader& filter, const Texture& source, const Texture* destination);

protected:
	void setSamplerSlot(const std::string& name, unsigned int value);

public:
	RenderingEngine(const Window& window);
	virtual ~RenderingEngine() {}

	void render(const GameObject& object, const Camera& mainCamera);
	void addLight(const Light& light);

	virtual void updateUniformStruct(const Transform& transform, const Material& material, const Shader& shader, std::string& uniformName, std::string& uniformType) const
	{
		throw uniformType + " is not supported by the rendering engine";
	}

	double displayRenderTime(double divisor);
	double displayWindowSyncTime(double divisor);

	const Light& getActiveLight() const;
	unsigned int getSamplerSlot(std::string& samplerName) const;
	const Matrix4& getLightMatrix() const;
};

#endif