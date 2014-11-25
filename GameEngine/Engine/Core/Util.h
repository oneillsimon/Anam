#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace Util
{
	void sleep(int milliseconds);
	std::vector<std::string> split(const std::string& s, char delim);
	std::vector<std::string> split(const std::string& s, char delims[]);
	bool isAlphaNumeric(char c);
	std::string stringFromVector(std::vector<std::string> s, const std::string& pad);
	void findAndReplace(std::string& subject, const std::string& search, const std::string& replace, std::vector<char> regex = {});
};

#endif