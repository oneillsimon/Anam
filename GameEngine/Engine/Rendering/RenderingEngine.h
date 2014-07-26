#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include <vector>
#include <map>
#include "../Components/Camera.h"
#include "../Components/Lighting.h"
#include "MappedValues.h"
#include "Material.h"

class Mesh;
class GameObject;

class RenderingEngine : public MappedValues
{
private:
	static const int s_numShadowMaps = 10;

	Camera* m_mainCamera;
	Light* m_activeLight;
	Shader* m_defaultShader;
	Shader* m_shadowMapShader;
	Shader* m_nullFilter;
	Shader* m_gausBlurFilter;
	Matrix4 m_lightMatrix;

	Camera* m_altCamera;
	GameObject* m_altCameraObj;

	Material* m_planeMaterial;
	Transform m_planeTransform;
	Mesh* m_planeMesh;
	Texture* m_tempTarget;

	Texture* m_shadowMaps[s_numShadowMaps];
	Texture* m_shadowMapsTempTargets[s_numShadowMaps];

	std::vector<Light*> m_lights;
	std::map<std::string, unsigned int> m_samplerMap;

	static const Matrix4 s_biasMatrix;

	void operator =(RenderingEngine& renderingEngine) {}

public:
	RenderingEngine();
	~RenderingEngine();

	void render(GameObject* object);
	void addLight(Light* light);
	void addCamera(Camera* camera);

	void blurShadowMap(int shadowMapIndex, float blur);
	void applyFilter(Shader* filter, Texture* source, Texture* destination);

	virtual void updateUniformStruct(const Transform& transform, Material& material, Shader* shader, std::string& uniformName, std::string& uniformType)
	{
		throw uniformType + " is not supported by the rendering engine";
	}

	Camera& getMainCamera();
	Light* getActiveLight();
	unsigned int getSamplerSlot(std::string& samplerName);
	Matrix4 getLightMatrix();
};

#endif