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
	Vector4* m_colour;

public:
	MeshRenderer(Mesh* mesh, Material* material);
	MeshRenderer(Mesh* mesh, Vector4* colour);
	MeshRenderer(Mesh* mesh, Material* material, Vector4* colour);
	~MeshRenderer();

	virtual void render(Shader* shader, RenderingEngine* renderingEngine);
};