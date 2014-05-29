#pragma once

#include "../Core/Transform.h"
#include "../Core/GameObject.h"

class Shader;
class RenderingEngine;

class GameComponent
{
private:
	GameObject* m_parent;

public:
	virtual ~GameComponent() {};

	virtual void input(float delta) {}
	virtual void update(float delta) {}
	virtual void render(Shader* shader, RenderingEngine* renderingEngine) {}

	virtual void addToCoreEngine(CoreEngine* coreEngine) {}

	void setParent(GameObject* parent);
	Transform& getTransform();
	const Transform& getTransform() const;

	/*inline void setParent(GameObject* parent) { m_parent = parent; }
	inline Transform& getTransform() { return m_parent->getTransform(); }
	inline const Transform& getTransform() const { return m_parent->getTransform(); }*/
};