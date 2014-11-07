#include "MoveScript.h"

MoveScript::MoveScript(int up, int down, int left, int right, float speed) :
m_up(up), m_down(down), m_left(left), m_right(right), m_speed(speed), Script("move.lua")
{
	luabridge::setGlobal(L, speed, "speed");
}

void MoveScript::input(const Input& input, float delta)
{
	luabridge::setGlobal(L, 0, "key");

	if(input.getKey(m_up))
	{
		luabridge::setGlobal(L, 1, "key");
	}

	if(input.getKey(m_down))
	{
		luabridge::setGlobal(L, 2, "key");
	}

	if(input.getKey(m_left))
	{
		luabridge::setGlobal(L, 3, "key");
	}

	if(input.getKey(m_right))
	{
		luabridge::setGlobal(L, 4, "key");
	}
}

void MoveScript::update(float delta)
{
	Script::update(delta);
}