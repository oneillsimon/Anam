#include "RenderingEngine.h"

const Matrix4 RenderingEngine::BIAS_MATRIX = Matrix4().initScale(0.5f, 0.5f, 0.5f) * Matrix4().initTranslation(1.0f, 1.0f, 1.0f);

RenderingEngine::RenderingEngine(const Window& window) :
	m_planeMesh(Mesh("plane.obj")),
	m_window(&window),
	m_tempTarget(window.getWidth(), window.getHeight(), 0, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0),
	m_planeMaterial("renderingEngine_filterPlane", m_tempTarget, 1, 8),
	m_defaultShader("forward-ambient"),
	m_shadowMapShader("shadowMapGenerator"),
	m_nullFilter(("filter-null")),
	m_gausBlurFilter("filter-gausBlur7x1"),
	m_fxaaFilter("filter-fxaa"),
	m_altCameraTransform(Vector3(), Quaternion(Vector3(0.0f, 1.0f, 0.0f), (float)toRadians(180.0f))),
	m_altCamera(Matrix4().initIdentity(), &m_altCameraTransform)
{
	setSamplerSlot("diffuse", 0);
	setSamplerSlot("normalMap", 1);
	setSamplerSlot("displacementMap", 2);
	setSamplerSlot("shadowMap", 3);

	setSamplerSlot("filterTexture", 0);

	//float ambience = 0.2f;
	float ambience = 1.0f;
	setVector3("ambient", Vector3(ambience, ambience, ambience));

	setFloat("fxaaSpanMax", 8.0f);
	setFloat("fxaaReduceMin", 1.0f / 128.0f);
	setFloat("fxaaReduceMul", 1.0f / 8.0f);

	setTexture("displayTexture", Texture(m_window->getWidth(), m_window->getHeight(), 0, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0));

	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	//glEnable(GL_MULTISAMPLE);

	m_planeTransform.setScale(1.0f);
	m_planeTransform.rotate(Quaternion(Vector3(1.0f, 0.0f, 0.0f), (float)toRadians(90.0f)));
	m_planeTransform.rotate(Quaternion(Vector3(0.0f, 0.0f, 1.0f), (float)toRadians(180.0f)));

	for(int i = 0; i < NUM_SHADOW_MAPS; i++)
	{
		int shadowMapSize = 1 << (i + 1);
		m_shadowMaps[i] = Texture(shadowMapSize, shadowMapSize, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0);
		m_shadowMapTempTargets[i] = Texture(shadowMapSize, shadowMapSize, 0, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0);
	}

	m_lightMatrix = Matrix4().initScale(0, 0, 0);
	this;
}

void RenderingEngine::render(const GameObject& object, const Camera& mainCamera)
{
	m_renderProfileTimer.startInvocation();

	getTexture("displayTexture").bindAsRenderTarget();

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	object.renderAll(m_defaultShader, *this, mainCamera);

	for(unsigned int i = 0; i < m_lights.size(); i++)
	{
		m_activeLight = m_lights[i];
		ShadowInfo shadowInfo = m_activeLight->getShadowInfo();

		int shadowMapIndex = 0;

		if(shadowInfo.getShadowMapSizeAsPowerOf2() != 0)
		{
			shadowMapIndex = shadowInfo.getShadowMapSizeAsPowerOf2() - 1;
		}

		setTexture("shadowMap", m_shadowMaps[shadowMapIndex]);
		m_shadowMaps[shadowMapIndex].bindAsRenderTarget();

		glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		if(shadowInfo.getShadowMapSizeAsPowerOf2() != 0)
		{
			m_altCamera.setProjection(shadowInfo.getProjection());

			ShadowCameraTransform shadowCameraTransform = m_activeLight->calculateShadowCameraTransform(mainCamera.getTransform().getTransformedPosition(),
																										mainCamera.getTransform().getTransformedRotation());

			m_altCamera.getTransform()->setPosition(shadowCameraTransform.position);
			m_altCamera.getTransform()->setRotation(shadowCameraTransform.rotation);

			m_lightMatrix = BIAS_MATRIX * m_altCamera.getViewProjection();

			setFloat("shadowVarianceMin", shadowInfo.getVarianceMin());
			setFloat("shadowLightBleedingReduction", shadowInfo.getLightBleedReductionAmount());
			
			bool flipFaces = shadowInfo.getFlipFaces();

			if(flipFaces)
			{
				glCullFace(GL_FRONT);
			}

			object.renderAll(m_shadowMapShader, *this, m_altCamera);

			if(flipFaces)
			{
				glCullFace(GL_BACK);
			}

			float shadowSoftness = shadowInfo.getShadowSoftness();

			if(shadowSoftness != 0)
			{
				blurShadowMap(shadowMapIndex, shadowSoftness);
			}
		}
		else
		{
			m_lightMatrix = Matrix4().initScale(0, 0, 0);
			setFloat("shadowVarianceMin", 0.00002f);
			setFloat("shadowLightBleedingReduction", 0.0f);
		}
		
		getTexture("displayTexture").bindAsRenderTarget();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		object.renderAll(m_activeLight->getShader(), *this, mainCamera);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}

	setVector3("inverseFilterTextureSize", Vector3(1.0f / getTexture("displayTexture").getWidth(), 1.0f / getTexture("displayTexture").getHeight(), 0.0f));

	m_renderProfileTimer.stopInvocation();

	m_windowSyncProfileTimer.startInvocation();
	applyFilter(m_fxaaFilter, getTexture("displayTexture"), 0);
	m_windowSyncProfileTimer.stopInvocation();
}

void RenderingEngine::addLight(const Light& light)
{
	m_lights.push_back(&light);
}

void RenderingEngine::blurShadowMap(int shadowMapIndex, float blur)
{
	setVector3("blurScale", Vector3(blur / m_shadowMaps[shadowMapIndex].getWidth(), 0.0f, 0.0f));
	applyFilter(m_gausBlurFilter, m_shadowMaps[shadowMapIndex], &m_shadowMapTempTargets[shadowMapIndex]);

	setVector3("blurScale", Vector3(0.0f, blur / m_shadowMaps[shadowMapIndex].getHeight(), 0.0f));
	applyFilter(m_gausBlurFilter, m_shadowMapTempTargets[shadowMapIndex], &m_shadowMaps[shadowMapIndex]);
}

void RenderingEngine::applyFilter(const Shader& filter, const Texture& source, const Texture* destination)
{
	if(&source == destination)
	{
		fprintf(stderr, "destination and source cannot be the same\n");
		return;
	}

	if(destination == 0)
	{
		m_window->bindAsRenderTarget();
	}
	else
	{
		destination->bindAsRenderTarget();
	}

	setTexture("filterTexture", source);

	m_altCamera.setProjection(Matrix4().initIdentity());
	m_altCamera.getTransform()->setPosition(Vector3());
	m_altCamera.getTransform()->setRotation(Quaternion(Vector3(0.0f, 1.0, 0.0f), (float)toRadians(180.0f)));

	glClear(GL_DEPTH_BUFFER_BIT);
	filter.bind();
	filter.updateUniforms(m_planeTransform, *this, m_altCamera, m_planeMaterial);
	m_planeMesh.draw();
	
	setTexture("filterTexture", 0);
}

double RenderingEngine::displayRenderTime(double divisor)
{
	return m_renderProfileTimer.displayAndReset("Render Time: ", divisor);
}

double RenderingEngine::displayWindowSyncTime(double divisor)
{
	return m_windowSyncProfileTimer.displayAndReset("Window Sync Time: ", divisor);
}

void RenderingEngine::setSamplerSlot(const std::string& name, unsigned int value)
{
	m_samplerMap[name] = value;
}

const Light& RenderingEngine::getActiveLight() const
{
	return *m_activeLight;
}

unsigned int RenderingEngine::getSamplerSlot(std::string& samplerName) const
{
	return m_samplerMap.find(samplerName)->second;
}

const Matrix4& RenderingEngine::getLightMatrix() const
{
	return m_lightMatrix;
}