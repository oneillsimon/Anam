#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const SpriteSheet& spriteSheet) :
	MeshRenderer(spriteSheet, spriteSheet.getMaterial())
{
}

void SpriteRenderer::render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const
{
	glDisable(GL_CULL_FACE);
	MeshRenderer::render(shader, renderingEngine, camera);
	glEnable(GL_CULL_FACE);
}