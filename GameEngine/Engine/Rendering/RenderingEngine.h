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

class GameObject;

class RenderingEngine : public MappedValues
{
private:
	//static const int s_numShadowMaps = 10;

	//const Window* m_window;
	//const Camera* m_mainCamera;
	//const Light* m_activeLight;
	//Shader m_defaultShader;
	//Shader m_shadowMapShader;
	//Shader m_nullFilter;
	//Shader m_gausBlurFilter;
	//Matrix4 m_lightMatrix;

	//Camera m_altCamera;
	//GameObject* m_altCameraObj;

	//Material* m_planeMaterial;
	//Transform m_planeTransform;
	//Mesh* m_planeMesh;
	////Texture* m_tempTarget;

	//Texture m_shadowMaps[s_numShadowMaps];
	//Texture m_shadowMapsTempTargets[s_numShadowMaps];

	//std::vector<const Light*> m_lights;
	//std::map<std::string, unsigned int> m_samplerMap;

	//static const Matrix4 s_biasMatrix;

	//void operator =(RenderingEngine& renderingEngine) {}

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

	void blurShadowMap(int shadowMapIndex, float blur);
	void applyFilter(const Shader& filter, const Texture& source, const Texture* destination);

protected:
	void setSamplerSlot(const std::string& name, unsigned int value);

public:
	RenderingEngine(const Window& window);
	virtual ~RenderingEngine() {}

	void render(const GameObject& object, const Camera& mainCamera);
	void addLight(const Light& light);
	//void addCamera(const Camera& camera);

	virtual void updateUniformStruct(const Transform& transform, const Material& material, const Shader& shader, std::string& uniformName, std::string& uniformType) const
	{
		throw uniformType + " is not supported by the rendering engine";
	}

	//const Camera& getMainCamera() const;
	const Light& getActiveLight() const;
	unsigned int getSamplerSlot(std::string& samplerName) const;
	const Matrix4& getLightMatrix() const;
};

#endif