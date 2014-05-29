#include <SDL2/SDL.h>
#include "Util.h"

void Util::sleep(int milliseconds)
{
	SDL_Delay(milliseconds);
}

std::vector<std::string> Util::split(const std::string& s, char delim)
{
	std::vector<std::string> elements;

	const char* cstr = s.c_str();
	int strLen = s.length();
	int start = 0;
	int end = 0;

	while(end <= strLen)
	{
		while(end <= strLen)
		{
			if(cstr[end] == delim)
			{
				break;
			}
			end++;
		}

		elements.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elements;
}