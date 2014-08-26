#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "Mesh.h"
#include "Material.h"

class SpriteSheet : public Mesh
{
private:
	IndexedModel m_indexedModel;
	Material m_material;
	int m_spriteCountX;
	int m_spriteCountY;
	float m_spriteWidth;
	float m_spriteHeight;
	Texture m_diffuseTexture;
	Vector2 m_currentTexCoords;

public:
	SpriteSheet(const std::string& spriteName, const Material& material, int spritesX, int spritesY);
	
	void cycleUp();
	void cycleDown();
	void cycleLeft();
	void cycleRight();

	Material getMaterial() const;
};

#endif