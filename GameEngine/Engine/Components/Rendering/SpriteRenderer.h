#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "MeshRenderer.h"
#include "../../Rendering/SpriteSheet.h"

class SpriteRenderer : public MeshRenderer
{
public:
	SpriteRenderer(const SpriteSheet& spriteSheet);
	virtual void render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif