#include "GL/glew.h"
#include "Window.h"
#include "../Core/GameObject.h"
#include "Shader.h"
#include "RenderingEngine.h"
#include "Mesh.h"
#include "../Core/GameMath.h"
#include <cstring>

//TEMP
static Texture* t_tempTarget = 0;
static Mesh* t_mesh = 0;
static Transform t_transform;
static Material* t_material;
static Camera* t_camerea = 0;
static GameObject* t_cameraObj = 0;

RenderingEngine::RenderingEngine()
{
	m_samplerMap.insert(std::pair<std::string, unsigned int>("diffuse", 0));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("normalMap", 1));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("displacementMap", 2));

	float ambiance = 0.5f;
	addVector3("ambient", Vector3(ambiance, ambiance, ambiance));
	m_defaultShader = new Shader("forward-ambient");

	glClearColor(0, 0, 0, 0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);

	//TEMP
	if(t_mesh) delete t_mesh;
	if(t_material) delete t_material;
	if(t_cameraObj) delete t_cameraObj;

	int width = Window::getWidth() / 3;
	int height = Window::getHeight() / 3;
	int dataSize = width * height * 4;

	unsigned char* data = new unsigned char[dataSize];
	memset(data, 0, dataSize);

	t_tempTarget = new Texture(width, height, data, GL_TEXTURE_2D, GL_NEAREST, GL_COLOR_ATTACHMENT0);
	delete[] data;

	Vertex vertices[] = { Vertex(Vector3(-1,-1,0),Vector2(1,0), Vector3::ZERO),
	                      Vertex(Vector3(-1,1,0),Vector2(1,1), Vector3::ZERO),
	                      Vertex(Vector3(1,1,0),Vector2(0,1), Vector3::ZERO),
	                      Vertex(Vector3(1,-1,0),Vector2(0,0), Vector3::ZERO) };
	int indices[] = { 2, 1, 0,
					  3, 2, 0 };

	t_material = new Material(t_tempTarget, 1, 8);
	t_transform.setScale(Vector3(0.9f, 0.9f, 0.9f));
	t_mesh = new Mesh(vertices, sizeof(vertices), indices, sizeof(indices) / sizeof(indices[0]), false);
	t_camerea = new Camera(Matrix4().initIdentity());
	t_cameraObj = new GameObject();
	t_cameraObj->addComponent(t_camerea);
	t_camerea->getTransform().rotate(Vector3(0, 1, 0), GameMath::toRadians(180.0f));
}

RenderingEngine::~RenderingEngine()
{
	if(m_defaultShader)
	{
		delete m_defaultShader;
	}

	//TEMP
	if(t_mesh) delete t_mesh;
	if(t_material) delete t_material;
	if(t_cameraObj) delete t_cameraObj;
}

void RenderingEngine::render(GameObject* object)
{
	//Window::bindAsRenderTarget();
	t_tempTarget->bindAsRenderTarget();
	glClearColor(0, 0, 0, 0);

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

	//TEMP
	Window::bindAsRenderTarget();
	
	Camera* temp = m_mainCamera;
	m_mainCamera = t_camerea;
	
	glClearColor(0.0f,0.0f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_defaultShader->bind();
	m_defaultShader->updateUniforms(t_transform, *t_material, this);
	t_mesh->draw();
	
	m_mainCamera = temp;
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