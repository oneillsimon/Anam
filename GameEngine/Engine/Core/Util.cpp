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

std::vector<std::string> Util::split(const std::string& s, char delims[])
{
	std::vector<std::string> res;
	std::string temp = s;

	for(int j = 0; j < sizeof(delims); j++)
	{
		res = split(temp, delims[j]);

		for(unsigned i = 0; i < res.size(); i++)
		{
			temp = stringFromVector(res, std::string(1, 127));
		}
	}

	return split(temp, 127);
}

void Util::findAndReplace(std::string& subject, const std::string& search, const std::string& replace, std::vector<char> regex)
{
	char before;
	char after;
	size_t pos = 0;

	while((pos = subject.find(search, pos)) != std::string::npos)
	{
		if(pos == 0)
		{
			before = 127;
		}
		else
		{
			before = subject.substr(pos - 1, 1).c_str()[0];
		}

		after = subject.substr(pos + search.length(), 1).c_str()[0];

		if(regex.size() == 0)
		{
			if(!isAlphaNumeric(before) && !isAlphaNumeric(after))
			{
				subject.replace(pos, search.length(), replace);
			}
		}
		else
		{
			for(unsigned i = 0; i < regex.size(); i++)
			{
				if(before == regex[i] || after == regex[i])
				{
					break;
				}
				
				if(!(isAlphaNumeric(before) || isAlphaNumeric(after) || before == regex[i] || after == regex[i]))
				{
					subject.replace(pos, search.length(), replace);
					break;
				}
			}
		}

		pos += replace.length();
	}
}

std::string Util::formatToLength(const std::string& s, int l, const std::string& append)
{
	int diff = l - s.length();

	if(diff < 0)
	{
		return s.substr(0, l);
	}

	std::string res = s;

	for(int i = 0; i < diff; i++)
	{
		res.append(append);
	}

	return res;
}

bool Util::isAlphaNumeric(char c)
{
	return (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

std::string Util::stringFromVector(std::vector<std::string> s, const std::string& pad)
{
	std::string line;
	for(unsigned i = 0; i < s.size(); i++)
	{
		line += s[i] + pad;
	}

	return line;
}