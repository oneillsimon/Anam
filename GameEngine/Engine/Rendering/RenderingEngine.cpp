#include "GL/glew.h"
#include "Window.h"
#include "../Core/GameObject.h"
#include "Shader.h"
#include "RenderingEngine.h"
#include "Mesh.h"
#include "../Core/GameMath.h"
#include <cstring>
#include "../Core/Math3D.h"

//TEMP
//static Texture* t_tempTarget = 0;
//static Mesh* t_mesh = 0;
//static Transform t_transform;

const Matrix4 RenderingEngine::s_biasMatrix = Matrix4().initScale(0.5f, 0.5f, 0.5f) * Matrix4().initTranslation(1.0f, 1.0f, 1.0f);

RenderingEngine::RenderingEngine()
{
	m_samplerMap.insert(std::pair<std::string, unsigned int>("diffuse", 0));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("normalMap", 1));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("displacementMap", 2));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("shadowMap", 3));

	float ambiance = 0.2f;
	setVector3("ambient", Vector3(ambiance, ambiance, ambiance));
	setTexture("shadowMap", new Texture(1024, 1024, 0, GL_TEXTURE_2D, GL_NEAREST, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, true, GL_DEPTH_ATTACHMENT));
	m_defaultShader = new Shader("forward-ambient");
	m_shadowMapShader = new Shader("shadowMapGenerator");

	glClearColor(0, 0, 0, 0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);

	m_altCamera = new Camera(Matrix4().initIdentity());
	m_altCameraObj = new GameObject();
	m_altCameraObj->addComponent(m_altCamera);
	m_altCamera->getTransform().rotate(Vector3(0, 1, 0), GameMath::toRadians(180.0f));

	int width = Window::getWidth();
	int height = Window::getHeight();

	m_tempTarget = new Texture(width, height, 0, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);

	m_planeMaterial = new Material(m_tempTarget, 1, 8);
	m_planeTransform.rotate(Quaternion(Vector3(1, 0, 0), GameMath::toRadians(90.0f)));
	m_planeTransform.rotate(Quaternion(Vector3(0, 0, 1), GameMath::toRadians(180.0f)));
	m_planeMesh = new Mesh("./res/models/plane.obj");
}

RenderingEngine::~RenderingEngine()
{
	if(m_defaultShader)
	{
		delete m_defaultShader;
	}

	if(m_altCameraObj)
	{
		delete m_altCameraObj;
	}

	if(m_planeMaterial)
	{
		delete m_planeMaterial;
	}

	if(m_planeMesh)
	{
		delete m_planeMesh;
	}
}

void RenderingEngine::render(GameObject* object)
{
	Window::bindAsRenderTarget();
	//m_tempTarget->bindAsRenderTarget();
	glClearColor(0, 0, 0, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	object->renderAll(m_defaultShader, this);

	for(unsigned int i = 0; i < m_lights.size(); i++)
	{
		m_activeLight = m_lights[i];

		ShadowInfo* shadowInfo = m_lights[i]->getShadowInfo();

		getTexture("shadowMap")->bindAsRenderTarget();
		glClear(GL_DEPTH_BUFFER_BIT);

		if(shadowInfo)
		{
			m_altCamera->setProjection(shadowInfo->getProjection());
			m_altCamera->getTransform().setPosition(m_activeLight->getTransform().getTransformedPosition());
			m_altCamera->getTransform().setRotation(m_activeLight->getTransform().getTransformedRotation());

			m_lightMatrix = s_biasMatrix * m_altCamera->getViewProjection();

			setVector3("shadowTexelSize", Vector3(1.0f / 1024.0f, 1.0f / 1024.0f, 0.0f));
														   // Shadow map size
			setFloat("shadowMapBias", shadowInfo->getBias() / 1024.0f);
			
			bool flipFaces = shadowInfo->getFlipFaces();

			Camera* temp = m_mainCamera;
			m_mainCamera = m_altCamera;

			if(flipFaces)
			{
				glCullFace(GL_FRONT);
			}

			object->renderAll(m_shadowMapShader, this);

			if(flipFaces)
			{
				glCullFace(GL_BACK);
			}

			m_mainCamera = temp;
		}
		
		Window::bindAsRenderTarget();
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		object->renderAll(m_activeLight->getShader(), this);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}

	//TEMP
	/*Window::bindAsRenderTarget();
	
	Camera* temp = m_mainCamera;
	m_mainCamera = m_altCamera;
	
	glClearColor(0.0f,0.0f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_defaultShader->bind();
	m_defaultShader->updateUniforms(m_planeTransform, *m_planeMaterial, this);
	m_planeMesh->draw();
	
	m_mainCamera = temp;*/
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

Matrix4 RenderingEngine::getLightMatrix()
{
	return m_lightMatrix;
}