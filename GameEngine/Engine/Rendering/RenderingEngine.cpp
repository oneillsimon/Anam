#include "GL/glew.h"
#include "Window.h"
#include "../Core/GameObject.h"
#include "Shader.h"
#include "RenderingEngine.h"
#include "Mesh.h"
#include "../Core/GameMath.h"
#include <cstring>
#include "../Core/Math3D.h"

const Matrix4 RenderingEngine::s_biasMatrix = Matrix4().initScale(0.5f, 0.5f, 0.5f) * Matrix4().initTranslation(1.0f, 1.0f, 1.0f);

RenderingEngine::RenderingEngine()
{
	m_samplerMap.insert(std::pair<std::string, unsigned int>("diffuse", 0));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("normalMap", 1));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("displacementMap", 2));
	m_samplerMap.insert(std::pair<std::string, unsigned int>("shadowMap", 3));

	float ambiance = 0.2f;
	setVector3("ambient", Vector3(ambiance, ambiance, ambiance));
	m_defaultShader = new Shader("forward-ambient");
	m_shadowMapShader = new Shader("shadowMapGenerator");
	m_nullFilter = new Shader("filter-null");
	m_gausBlurFilter = new Shader("filter-gausBlur7x1");

	glClearColor(0, 0, 0, 0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_MULTISAMPLE);

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

	for(int i = 0; i < s_numShadowMaps; i++)
	{
		int shadowMapSize = 1 << (i + 1);
		m_shadowMaps[i] = new Texture(shadowMapSize, shadowMapSize, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0);
		m_shadowMapsTempTargets[i] = new Texture(shadowMapSize, shadowMapSize, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0);
	}

	m_lightMatrix = Matrix4().initScale(0, 0, 0);
}

RenderingEngine::~RenderingEngine()
{
	setTexture("shadowMap", 0);

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

	if(m_shadowMapShader)
	{
		delete m_shadowMapShader;
	}

	if(m_nullFilter)
	{
		delete m_nullFilter;
	}

	if(m_gausBlurFilter)
	{
		delete m_gausBlurFilter;
	}

	for(int i = 0; i < s_numShadowMaps; i++)
	{
		if(m_shadowMaps[i])
		{
			delete m_shadowMaps[i];
		}

		if(m_shadowMapsTempTargets[i])
		{
			delete m_shadowMapsTempTargets[i];
		}
	}
}

void RenderingEngine::render(GameObject* object)
{
	Window::bindAsRenderTarget();

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	object->renderAll(m_defaultShader, this);

	for(unsigned int i = 0; i < m_lights.size(); i++)
	{
		m_activeLight = m_lights[i];
		ShadowInfo* shadowInfo = m_activeLight->getShadowInfo();

		int shadowMapIndex = 0;

		if(shadowInfo)
		{
			shadowMapIndex = shadowInfo->getShadowMapSizeAsPowerOf2() - 1;
		}

		setTexture("shadowMap", m_shadowMaps[shadowMapIndex]);
		m_shadowMaps[shadowMapIndex]->bindAsRenderTarget();
		glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		if(shadowInfo)
		{
			m_altCamera->setProjection(shadowInfo->getProjection());

			ShadowCameraTransform shadowCameraTransform = m_activeLight->calculateShadowCameraTransform(m_mainCamera->getTransform().getTransformedPosition(),
																										m_mainCamera->getTransform().getTransformedRotation());

			m_altCamera->getTransform().setPosition(shadowCameraTransform.position);
			m_altCamera->getTransform().setRotation(shadowCameraTransform.rotation);

			m_lightMatrix = s_biasMatrix * m_altCamera->getViewProjection();
			setFloat("shadowVarianceMin", shadowInfo->getVarianceMin());
			setFloat("shadowLightBleedingReduction", shadowInfo->getLightBleedReductionAmount());
			
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

			float shadowSoftness = shadowInfo->getShadowSoftness();

			if(shadowSoftness != 0)
			{
				blurShadowMap(shadowMapIndex, shadowSoftness);
			}
		}
		else
		{
			m_lightMatrix = s_biasMatrix * m_altCamera->getViewProjection();
			setFloat("shadowVarianceMin", 0.00002f);
			setFloat("shadowLightBleedingReduction", 0);
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
}

void RenderingEngine::addLight(Light* light)
{
	m_lights.push_back(light);
}

void RenderingEngine::addCamera(Camera* camera)
{
	m_mainCamera = camera;
}

void RenderingEngine::blurShadowMap(int shadowMapIndex, float blur)
{
	setVector3("blurScale", Vector3(blur / m_shadowMaps[shadowMapIndex]->getWidth(), 0.0f, 0.0f));
	applyFilter(m_gausBlurFilter, m_shadowMaps[shadowMapIndex], m_shadowMapsTempTargets[shadowMapIndex]);

	setVector3("blurScale", Vector3(0.0f, blur / m_shadowMaps[shadowMapIndex]->getHeight(), 0.0f));
	applyFilter(m_gausBlurFilter, m_shadowMapsTempTargets[shadowMapIndex], m_shadowMaps[shadowMapIndex]);
}

void RenderingEngine::applyFilter(Shader* filter, Texture* source, Texture* destination)
{
	if(source == destination)
	{
		fprintf(stderr, "destination and source cannot be the same\n");
		return;
	}

	if(destination == 0)
	{
		Window::bindAsRenderTarget();
	}
	else
	{
		destination->bindAsRenderTarget();
	}

	setTexture("filterTexture", source);

	m_altCamera->setProjection(Matrix4().initIdentity());
	m_altCamera->getTransform().setPosition(Vector3::ZERO);
	m_altCamera->getTransform().setRotation(Quaternion(Vector3(0, 1, 0), GameMath::toRadians(180.0f)));
	
	Camera* temp = m_mainCamera;
	m_mainCamera = m_altCamera;

	glClear(GL_DEPTH_BUFFER_BIT);
	filter->bind();
	filter->updateUniforms(m_planeTransform, *m_planeMaterial, this);
	m_planeMesh->draw();
	
	m_mainCamera = temp;
	setTexture("filterTexture", 0);
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