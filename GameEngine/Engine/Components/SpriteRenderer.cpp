#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const SpriteSheet& spriteSheet) :
	MeshRenderer(spriteSheet, spriteSheet.getMaterial())
{
}