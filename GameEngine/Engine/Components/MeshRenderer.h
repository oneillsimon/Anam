#pragma once

#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"
#include "../Rendering/Shader.h"
#include "GameComponent.h"

class MeshRenderer : public GameComponent
{
private:
	Mesh* m_mesh;
	Material* m_material;

public:
	MeshRenderer(Mesh* mesh, Material* material);
	~MeshRenderer();

	virtual void render(Shader* shader, RenderingEngine* renderingEngine);
};