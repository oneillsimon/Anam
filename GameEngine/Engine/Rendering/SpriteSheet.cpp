#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(const std::string& spriteName, const Material& material, int spritesX, int spritesY) :
	Mesh(),
	m_material(material),
	m_spriteCountX(spritesX),
	m_spriteCountY(spritesY),
	m_diffuseTexture(material.getDiffuseTexture())
{
	Vector2 _3_00(0.0f / (float)spritesX, 0.0f / (float)spritesY);
	Vector2 _1_10(1.0f / (float)spritesX, 0.0f / (float)spritesY);
	Vector2 _2_01(0.0f / (float)spritesX, 1.0f / (float)spritesY);
	Vector2 _0_11(1.0f / (float)spritesX, 1.0f / (float)spritesY);

	m_indexedModel.addVertex(1.0f, 1.0f, 0.0f);
	m_indexedModel.addVertex(1.0f, -1.0f, 0.0f);
	m_indexedModel.addVertex(-1.0f, 1.0f, 0.0f);
	m_indexedModel.addVertex(-1.0f, -1.0f, 0.0f);
	
	m_indexedModel.addTexCoord(_0_11);
	m_indexedModel.addTexCoord(_1_10);
	m_indexedModel.addTexCoord(_2_01);
	m_indexedModel.addTexCoord(_3_00);
	
	m_indexedModel.addFace(1, 0, 3);
	m_indexedModel.addFace(2, 3, 0);
	
	m_indexedModel = m_indexedModel.finalize();

	m_spriteWidth = (float)m_diffuseTexture.getWidth() / (float)m_spriteCountX;
	m_spriteHeight = (float)m_diffuseTexture.getHeight() / (float)m_spriteCountY;

	m_meshData = new MeshData(m_indexedModel);
	resourceMap.insert(std::pair<std::string, MeshData*>(spriteName, m_meshData));
}

void SpriteSheet::cycleUp(float delta)
{
	Vector2 tL = m_indexedModel.getTexCoords()[0];
	Vector2 amount = Vector2(0.0f, m_spriteHeight / m_diffuseTexture.getHeight());

	if(tL.getY() >= (m_spriteCountY - 1) * m_spriteHeight)
	{
		amount = amount - 1;
	}

	m_indexedModel.addToTexCoords(0, amount);
	m_indexedModel.addToTexCoords(1, amount);
	m_indexedModel.addToTexCoords(2, amount);
	m_indexedModel.addToTexCoords(3, amount);

	m_meshData->updateTextureBuffer(m_indexedModel);
}

Material SpriteSheet::getMaterial() const
{
	return m_material;
}