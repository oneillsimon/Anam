#include<GL\glew.h>

#include "../../Physics/Collider.h"
#include "PhysicsObjectComponent.h"

PhysicsObjectComponent::PhysicsObjectComponent(const PhysicsObject* object) :
	m_physicsObject(object)
{
}

void PhysicsObjectComponent::update(float delta)
{
	getTransform()->setPosition(m_physicsObject->getPosition());
}

void PhysicsObjectComponent::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	Transform t = getTransform();
	t.setScale(m_physicsObject->getCollider().getScale());

	m_physicsObject->getCollider().getMesh().getWireFrameShader().bind();
	m_physicsObject->getCollider().getMesh().getWireFrameShader().updateUniforms(t, renderingEngine, camera);
	m_physicsObject->getCollider().getMesh().getWireFrameShader().setUniform("wireFrameColour", COLOUR_YELLOW.toGLSLVec4());
	m_physicsObject->getCollider().getMesh().drawWireFrame();
}