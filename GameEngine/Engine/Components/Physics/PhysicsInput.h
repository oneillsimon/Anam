#ifndef PHYSICSINPUT_H
#define PHYSICSINPUT_H

#include "../../Core/GameComponent.h"
#include "../../Physics/PhysicsComponent.h"

class PhysicsInput : public GameComponent
{
private:
	PhysicsComponent m_component;
	int m_key1, m_key2, m_key3, m_key4;

public:
	PhysicsInput(const PhysicsComponent& componet, int key1 = 0, int key2 = 0, int key3 = 0, int key4 = 0);

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
};

#endif