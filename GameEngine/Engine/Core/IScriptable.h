#ifndef ISCRIPTABLE_H
#define ISCRIPTABLE_H

#include "Lua.h"

class IScriptable
{
	virtual void registerMembers(const std::string& namespace_, lua_State* luaState){};
};

#endif