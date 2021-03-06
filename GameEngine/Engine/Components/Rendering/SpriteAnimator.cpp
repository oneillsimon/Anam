#include "SpriteAnimator.h"

SpriteAnimator::SpriteAnimator(const SpriteSheet& spriteSheet, float every) :
	m_spriteSheet(spriteSheet),
	m_speed(every)
{
	m_timer = 0;
}

void SpriteAnimator::update(float delta)
{
	m_timer += delta * 1000.0f;

	if(m_timer > m_speed)
	{
		m_spriteSheet.cycleLeft();
		m_timer = 0;
	}
}