#ifndef COLLIDERRENDERER_H
#define COLLIDERRENDERER_H

#include "../../Physics/Collider.h"
#include "../Rendering/MeshRenderer.h"

class ColliderRenderer : public MeshRenderer
{
private:
	Colour m_defaultColour;
	Colour m_collidingColour;
	Collider* m_collider;

public:
	ColliderRenderer(Collider* collider, const Colour& defaultColour = COLOUR_YELLOW, const Colour& collidingColour = COLOUR_RED);
	ColliderRenderer(ColliderPlane* collider, const Colour& defaultColour = COLOUR_YELLOW, const Colour& collidingColour = COLOUR_RED);

	virtual void render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const;

	std::string getMeshFromCollider(int type);
};

#endif