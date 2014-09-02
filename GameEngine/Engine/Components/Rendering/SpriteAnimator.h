#ifndef SPRITEANIMATOR_H
#define SPRITEANIMATOR_H

#include "../../Rendering/SpriteSheet.h"
#include "../GameComponent.h"

class SpriteAnimator : public GameComponent
{
private:
	SpriteSheet m_spriteSheet;
	float m_speed;
	float m_timer;

public:
	SpriteAnimator(const SpriteSheet& spriteSheet, float every, bool upCondition);

	virtual void update(float delta);
};

#endif