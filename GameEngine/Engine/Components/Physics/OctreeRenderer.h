#ifndef OCTREE_RENDERER_H
#define OCTREE_RENDERER_H

#include "../Rendering/MeshRenderer.h"
#include "../../Physics/Collider.h"
#include "../../Physics/Octree.h"
#include "ColliderRenderer.h"

class OctreeRenderer : public ColliderRenderer
{
private:
	Octree* m_octree;

public:
	OctreeRenderer(Octree* octree, const Colour& defaultColour = COLOUR_GREEN_YELLOW);

	virtual void initialise();
	virtual void render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif