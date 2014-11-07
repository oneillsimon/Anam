#ifndef MOVESCRIPT_H
#define MOVESCRIPT_H

#include "../Core/Script.h"

class MoveScript : public Script
{
private:
	int m_up;
	int m_down;
	int m_left;
	int m_right;
	float m_speed;
	int key;

public:
	MoveScript(int up, int down, int left, int right, float speed);
	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
};

#endif