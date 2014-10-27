#include "../Core/CoreEngine.h"
#include "../Core/GameComponent.h"
#include "../Physics_/RigidBody.h"
#include "Collider.h"

class PhysicsComponent : public GameComponent
{
private:
	RigidBody m_rigidBody;
	Collider m_collider;

public:
	PhysicsComponent(const RigidBody& rigidBody, const Collider& collider);

	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera);

	RigidBody getRigidBody() const;
	Collider getCollider() const;
};