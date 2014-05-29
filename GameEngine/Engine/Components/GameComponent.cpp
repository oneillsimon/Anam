#include "GameComponent.h"

void GameComponent::setParent(GameObject* parent)
{
	m_parent = parent;
}

Transform& GameComponent::getTransform()
{
	return m_parent->getTransform();
}

const Transform& GameComponent::getTransform() const
{
	return m_parent->getTransform();
}