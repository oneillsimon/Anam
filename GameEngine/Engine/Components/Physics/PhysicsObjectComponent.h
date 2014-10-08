#ifndef PHYSICSOBJECTCOMPONENT
#define PHYSICSOBJECTCOMPONENT

#include "../../Core/GameComponent.h"
#include "../../Physics/PhysicsEngine.h"

class PhysicsObjectComponent : public GameComponent
{
private:
	const PhysicsObject* m_physicsObject;

public:
	PhysicsObjectComponent(const PhysicsObject* object);

	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif