#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <RefCountedPtr.h>

#include "Math3D.h"

class Transform
{
private:
	Transform* m_parent;
	mutable Matrix4 m_parentMatrix;

	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;

	mutable Vector3 oldPosition;
	mutable Quaternion oldRotation;
	mutable Vector3 oldScale;

	mutable bool m_intialisedOldStuff;

public:
	static void registerMembers(const std::string& namespace_, lua_State* luaState);

	Transform(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(0, 0, 0, 1), float scale = 1.0f);
	~Transform();

	bool hasChanged();
	void update();
	void rotate(const Vector3& axis, float angle);
	void rotate(const Quaternion& rotation);
	void lookAt(const Vector3& point, const Vector3& up);

	Quaternion getLookAtRotation(const Vector3& point, const Vector3& up);
	Matrix4 getTransformation() const;
	Vector3 getTransformedPosition() const;
	Vector3 getTransformedPoint(const Vector3& point) const;
	Quaternion getTransformedRotation() const;

	Transform* getParent();
	Matrix4 getParentMatrix() const;

	const Vector3& getPosition() const;
	Vector3& getPosition();
	const Quaternion& getRotation() const;
	Quaternion& getRotation();
	const Vector3& getScale() const;
	Vector3& getScale();

	// TEMP
	Vector3 getPositionLua() const;

	void setParent(Transform* parent);
	void setParentMatrix(Matrix4 parentMatrix);
	void setPosition(const Vector3& position);
	void setRotation(const Quaternion& rotation);
	void setScale(const Vector3& scale);
	void setScale(float scale);
};

#endif