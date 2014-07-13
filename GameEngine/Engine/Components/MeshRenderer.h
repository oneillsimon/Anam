#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"
#include "../Rendering/Colour.h"
#include "GameComponent.h"

class MeshRenderer : public GameComponent
{
private:
	Mesh* m_mesh;
	Material* m_material;
	Colour* m_colour;

public:
	MeshRenderer();
	MeshRenderer(Mesh* mesh, Material* material);
	MeshRenderer(Mesh* mesh, Colour* colour);
	MeshRenderer(Mesh* mesh, Material* material, Colour* colour);
	~MeshRenderer();

	virtual void render(Shader* shader, RenderingEngine* renderingEngine);
};

#endif