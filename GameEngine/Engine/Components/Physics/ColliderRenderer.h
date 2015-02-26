#ifndef COLLIDERRENDERER_H
#define COLLIDERRENDERER_H

#include "../../Physics/PhysicsComponent.h"
#include "../Rendering/MeshRenderer.h"

class ColliderRenderer : public MeshRenderer
{
protected:
	Colour m_colour;
	PhysicsComponent* m_component;

public:
	ColliderRenderer(PhysicsComponent* component, const Colour& colour = COLOUR_YELLOW);

	virtual void render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const;

	std::string getMeshFromCollider(int type);
};

#endif