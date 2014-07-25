#include "GL/glew.h"
#include "Window.h"
#include "../Core/GameObject.h"
#include "Shader.h"
#include "RenderingEngine.h"

RenderingEngine::RenderingEngine()
{
	m_samplerMap.insert(std::pair<std::string, unsigned int>("diffuse", 0));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("normalMap", 1));

	float ambiance = 0.1f;
	addVector3("ambient", Vector3(ambiance, ambiance, ambiance));
	m_defaultShader = new Shader("forward-ambient");

	glClearColor(0, 0, 0, 0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
}

RenderingEngine::~RenderingEngine()
{
	if(m_defaultShader)
	{
		delete m_defaultShader;
	}
}

void RenderingEngine::render(GameObject* object)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	object->renderAll(m_defaultShader, this);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	for(unsigned int i = 0; i < m_lights.size(); i++)
	{
		m_activeLight = m_lights[i];
		object->renderAll(m_activeLight->getShader(), this);
	}

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
}

void RenderingEngine::addLight(Light* light)
{
	m_lights.push_back(light);
}

void RenderingEngine::addCamera(Camera* camera)
{
	m_mainCamera = camera;
}

Light* RenderingEngine::getActiveLight()
{
	return m_activeLight;
}

Camera& RenderingEngine::getMainCamera()
{
	return *m_mainCamera;
}

unsigned int RenderingEngine::getSamplerSlot(std::string& samplerName)
{
	return m_samplerMap[samplerName];
}