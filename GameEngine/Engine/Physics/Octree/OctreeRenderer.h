#ifndef OCTREE_RENDERER_H
#define OCTREE_RENDERER_H

#include "../../Core/GameComponent.h"
#include "Octree.h"
#include "../../Rendering/Mesh.h"

class OctreeRenderer : public GameComponent
{
private:
	Octree* m_octree;
	std::vector<Mesh> meshes;
	int lastCount;
public:
	OctreeRenderer(Octree* octree);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif