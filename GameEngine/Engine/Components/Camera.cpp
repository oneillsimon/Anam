#include "../Core/CoreEngine.h"
#include "../Rendering/RenderingEngine.h"
#include "Camera.h"

Camera::Camera(Matrix4& projection) : m_projection(projection)
{
}

void Camera::setProjection(const Matrix4& projection)
{
	m_projection = projection;
}

void Camera::addToCoreEngine(CoreEngine* coreEngine)
{
	coreEngine->getRenderingEngine()->addCamera(this);
}

Matrix4 Camera::getViewProjection()
{
	Matrix4 cameraRotation = getTransform().getTransformedRotation().conjugate().toRotationMatrix();
	Matrix4 cameraTranslation;

	Transform t = getTransform();

	cameraTranslation.initTranslation(t.getTransformedPosition() * -1);

	return m_projection * cameraRotation * cameraTranslation;
}