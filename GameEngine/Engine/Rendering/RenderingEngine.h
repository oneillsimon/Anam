#pragma once

#include <vector>
#include <map>
#include "../Components/Camera.h"
#include "../Components/Lighting.h"
#include "MappedValues.h"
#include "Material.h"

class GameObject;

class RenderingEngine : public MappedValues
{
private:
	Camera* m_mainCamera;
	Light* m_activeLight;
	Shader* m_defaultShader;

	std::vector<Light*> m_lights;
	std::map<std::string, unsigned int> m_samplerMap;

	void operator =(RenderingEngine& renderingEngine) {}

public:
	RenderingEngine();
	~RenderingEngine();

	void render(GameObject* object);
	void addLight(Light* light);
	void addCamera(Camera* camera);
	virtual void updateUniformStruct(const Transform& transform, Material& material, Shader* shader, std::string& uniformName, std::string& uniformType)
	{
		throw uniformType + " is not supported by the rendering engine";
	}

	Camera& getMainCamera();
	Light* getActiveLight();
	unsigned int getSamplerSlot(std::string& samplerName);
};