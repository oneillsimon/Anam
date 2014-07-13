#ifndef LINE_H
#define LINE_H

#include "../Core/Math3D.h"
#include "../Components/GameComponent.h"
#include "../Components/MeshRenderer.h"

class Line : public GameComponent
{
private:
	Vector3 point0;
	Vector3 point1;

	Mesh* m_mesh;
	MeshRenderer* m_meshRenderer;
	Colour* m_colour;
	Material* m_material;

public:
	Line(const Vector3& point0, const Vector3& point1, Colour* colour);
	
	virtual void update(float delta);
	virtual void render(Shader* shader, RenderingEngine* renderingEngine);
};

#endif