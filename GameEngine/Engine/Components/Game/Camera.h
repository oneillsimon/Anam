#ifndef CAMERA_H
#define CAMERA_H

#include "../../Core/GameComponent.h"

class Camera
{
private:
	Matrix4 m_projection;
	Transform* m_transform;

public:
	Camera(const Matrix4& projection = Matrix4().initIdentity(), Transform* transform = 0);

	Matrix4 getViewProjection() const;
	Transform* getTransform();
	const Transform& getTransform() const;

	void setProjection(const Matrix4& projection);
	void setTransform(Transform* transform);
};

class CameraComponent : public GameComponent
{
private:
	Camera m_camera;

public:
	CameraComponent(const Matrix4& projection) :
		m_camera(projection, 0) {}

	virtual void addToCoreEngine(CoreEngine* engine) const;

	Matrix4 getViewProjection() const;

	void setProjection(const Matrix4& projection);
	virtual void setParent(GameObject* parent);
};

#endif