#include "OctreeRenderer.h"

OctreeRenderer::OctreeRenderer(Octree* octree) :
	m_octree(octree)
{
	lastCount = 0;
}

void OctreeRenderer::update(float delta)
{
	return;
	//printf("lastCount %d\n", lastCount);
	//if(lastCount != Octree::partitions.size())
	//{
	//	meshes.clear();
	//	for(int i = 0; i < Octree::partitions.size(); i++)
	//	{
	//		meshes.push_back(Mesh("cube.obj"));
	//	}
	//}
	//
	//lastCount = Octree::partitions.size();
}

void OctreeRenderer::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	//for(int i = 0; i < Octree::partitions.size(); i++)
	//{
	//	Vector3 c = Octree::partitions[i].m_halfExtents - (Octree::partitions[i].m_halfExtents / 2);
	//	Transform t = Transform(c, Quaternion(), abs(Octree::partitions[i].m_halfExtents.getX()));
	//	Mesh m("cube.obj");
	//	m.getWireFrameShader().bind();
	//	m.getWireFrameShader().updateUniforms(t, renderingEngine, camera, MATERIAL_DEFAULT);
	//	m.getWireFrameShader().setUniform("wireFrameColour", COLOUR_RED);
	//	m.drawWireFrame();
	//}
}