#include "OctreeRenderer.h"

OctreeRenderer::OctreeRenderer(Octree* tree, const Colour& colour) :
ColliderRenderer(false, new ColliderBox(tree->m_max - tree->m_centre), colour),
m_octree(tree)
{
}

void OctreeRenderer::initialise()
{
	if(m_octree->m_hasChildren)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++)
				{
					m_parent->addComponent(new OctreeRenderer(m_octree->m_children[x][y][z], m_defaultColour));
				}
			}
		}
	}
}

void OctreeRenderer::render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const
{
	Transform t = getTransform();
	t.setScale(m_octree->m_max - m_octree->m_centre);
	t.setPosition(m_octree->m_centre);
	m_mesh.getWireFrameShader().bind();
	m_mesh.getWireFrameShader().updateUniforms(t, renderingEngine, camera, m_material);
	m_mesh.getWireFrameShader().setUniform("wireFrameColour", m_defaultColour);
	m_mesh.drawWireFrame();
}