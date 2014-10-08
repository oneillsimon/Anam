#ifndef SPRITEANIMATOR_H
#define SPRITEANIMATOR_H

#include "../../Core/GameComponent.h"
#include "../../Rendering/SpriteSheet.h"

class SpriteAnimator : public GameComponent
{
private:
	SpriteSheet m_spriteSheet;
	float m_speed;
	float m_timer;

public:
	SpriteAnimator(const SpriteSheet& spriteSheet, float every);

	virtual void update(float delta);
};

#endif