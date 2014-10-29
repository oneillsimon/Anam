#include "Transform.h"

void Transform::registerMembers(const std::string& namespace_, lua_State* luaState)
{
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace(namespace_.c_str())
		.beginClass<Transform>("Transform")
		.addConstructor<void(*)(const Vector3&, const Quaternion&, const float&)>()
		.addFunction("setPosition", &Transform::setPosition)
		.addFunction("getPosition", &Transform::getPositionLua)
		.addFunction("rotate", &Transform::rotateLua)
		.endClass()
		.endNamespace();
}

Vector3 Transform::getPositionLua() const
{
	return Transform::getPosition();
}

void Transform::rotateLua(const Vector3& v, float f)
{
	rotate(v, f);
}

Transform::Transform(const Vector3& position, const Quaternion& rotation, float scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = Vector3(scale, scale, scale);
	m_intialisedOldStuff = false;
	m_parent = 0;

	m_parentMatrix = Matrix4().initIdentity();
}

Transform::~Transform()
{
}

void Transform::update()
{
	if(m_intialisedOldStuff)
	{
		oldPosition = m_position;
		oldRotation = m_rotation;
		oldScale = m_scale;
	}
	else
	{
		oldPosition = m_position + Vector3(1.0f, 1.0f, 1.0f);
		oldRotation = m_rotation * 0.5f;
		oldScale = m_scale + 1;

		m_intialisedOldStuff = true;
	}
}

void Transform::rotate(const Vector3& axis, float angle)
{
	rotate(Quaternion(axis, angle));
}

void Transform::rotate(const Quaternion& rotation)
{
	Quaternion q = Quaternion((rotation * m_rotation).normalised());
	this->m_rotation = Quaternion((rotation * m_rotation).normalised());
}

bool Transform::hasChanged()
{
	if(m_parent != 0 && m_parent->hasChanged())
	{
		return true;
	}

	if(m_position != oldPosition)
	{
		return true;
	}

	if(m_rotation != oldRotation)
	{
		return true;
	}

	if(m_scale != oldScale)
	{
		return true;
	}

	return false;
}

void Transform::lookAt(const Vector3& point, const Vector3& up)
{
	m_rotation = getLookAtRotation(point, up);
}

Quaternion Transform::getLookAtRotation(const Vector3& point, const Vector3& up)
{
	return Quaternion(Matrix4().initRotation((point - m_position).normalised(), up));
}

Matrix4 Transform::getTransformation() const
{
	Matrix4 translationMatrix;
	Matrix4 scaleMatrix;

	translationMatrix.initTranslation(Vector3(m_position.getX(), m_position.getY(), m_position.getZ()));
	scaleMatrix.initScale(m_scale.getX(), m_scale.getY(), m_scale.getZ());

	Matrix4 result = translationMatrix * m_rotation.toRotationMatrix() * scaleMatrix;
	
	return getParentMatrix() * result;
}

Vector3 Transform::getTransformedPosition() const
{
	return Vector3(getParentMatrix().transform(m_position));
}

Vector3 Transform::getTransformedPoint(const Vector3& point) const
{
	return Vector3(getParentMatrix().transform(point));
}

Quaternion Transform::getTransformedRotation() const
{
	Quaternion parentRotation = Quaternion(0, 0, 0, 1);

	if(m_parent)
	{
		parentRotation = m_parent->getTransformedRotation();
	}

	return parentRotation * m_rotation;
}

Transform* Transform::getParent()
{
	return m_parent;
}

Matrix4 Transform::getParentMatrix() const
{
	if(m_parent != 0 && m_parent->hasChanged())
	{
		m_parentMatrix = m_parent->getTransformation();
	}

	return m_parentMatrix;
}

const Vector3& Transform::getPosition() const
{
	return m_position;
}

Vector3& Transform::getPosition()
{
	return m_position;
}

const Quaternion& Transform::getRotation() const
{
	return m_rotation;
}

Quaternion& Transform::getRotation()
{
	return m_rotation;
}

const Vector3& Transform::getScale() const
{
	return m_scale;
}

Vector3& Transform::getScale()
{
	return m_scale;
}

void Transform::setParent(Transform* parent)
{
	m_parent = parent;
}

void Transform::setPosition(const Vector3& position)
{
	m_position = position;
}

void Transform::setRotation(const Quaternion& rotation)
{
	m_rotation = rotation;
}

void Transform::setScale(const Vector3& scale)
{
	m_scale = scale;
}

void Transform::setScale(float scale)
{
	setScale(Vector3(scale, scale, scale));
}