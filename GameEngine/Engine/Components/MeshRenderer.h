#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "../Rendering/Mesh.h"
#include "../Rendering/Shader.h"
#include "GameComponent.h"

class MeshRenderer : public GameComponent
{
protected:
	Mesh m_mesh;
	Material m_material;

public:
	MeshRenderer(const Mesh& mesh, const Material& material) :
		m_mesh(mesh), m_material(material) {};

	virtual void render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif