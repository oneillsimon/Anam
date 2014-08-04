#include "../Core/CoreEngine.h"
#include "../Rendering/RenderingEngine.h"
#include "Camera.h"

Camera::Camera(const Matrix4& projection, Transform* transform)
{
	m_projection = projection;
	m_transform = transform;
}

Matrix4 Camera::getViewProjection() const
{
	Matrix4 cameraRotation = getTransform().getTransformedRotation().conjugate().toRotationMatrix();
	Matrix4 cameraTranslation;

	cameraTranslation.initTranslation(getTransform().getTransformedPosition() * -1);

	return m_projection * cameraRotation * cameraTranslation;
}

Transform* Camera::getTransform()
{
	return m_transform;
}

const Transform& Camera::getTransform() const
{
	return *m_transform;
}

void Camera::setProjection(const Matrix4& projection)
{
	m_projection = projection;
}

void Camera::setTransform(Transform* transform)
{
	m_transform = transform;
}

void CameraComponent::addToCoreEngine(CoreEngine* engine) const
{
	engine->setCamera(m_camera);
}

Matrix4 CameraComponent::getViewProjection() const
{
	return m_camera.getViewProjection();
}

void CameraComponent::setProjection(const Matrix4& projection)
{
	m_camera.setProjection(projection);
}

void CameraComponent::setParent(GameObject* parent)
{
	GameComponent::setParent(parent);
	m_camera.setTransform(getTransform());
}