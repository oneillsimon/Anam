#include "MoveScript.h"

MoveScript::MoveScript(int up, int down, int left, int right, float speed, GameObject * p) :
m_up(up), m_down(down), m_left(left), m_right(right), m_speed(speed), Script("move.lua", p)
{
	luabridge::setGlobal(p->getL(), speed, "speed");
}

void MoveScript::input(const Input& input, float delta)
{
	luabridge::setGlobal(m_parent->getL(), 0, "key");

	if(input.getKey(m_up))
	{
		luabridge::setGlobal(m_parent->getL(), 1, "key");
	}

	if(input.getKey(m_down))
	{
		luabridge::setGlobal(m_parent->getL(), 2, "key");
	}

	if(input.getKey(m_left))
	{
		luabridge::setGlobal(m_parent->getL(), 3, "key");
	}

	if(input.getKey(m_right))
	{
		luabridge::setGlobal(m_parent->getL(), 4, "key");
	}
}

void MoveScript::update(float delta)
{
	Script::update(delta);
}