#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include "../Core/GameObject.h"

class GameComponent
{
protected:
	GameObject* m_parent;

public:
	virtual ~GameComponent() {};

	virtual void initialise() {}
	virtual void input(const Input& input, float delta) {}
	virtual void update(float delta) {}
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const {}

	virtual void addToCoreEngine(CoreEngine* coreEngine) const {}

	virtual void setParent(GameObject* parent);
	Transform* getTransform();
	const Transform& getTransform() const;
};

#endif