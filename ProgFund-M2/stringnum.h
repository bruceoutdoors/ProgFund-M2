#pragma once

#include <sstream>

// copy pasted from http://www.cplusplus.com/articles/D9j2Nwbp/

template <typename T>
static string numToString(T Number)
{
	ostringstream ss;
	ss << Number;
	return ss.str();
}

template <typename T>
static T stringToNum(const string &Text)
{
	istringstream ss(Text);
	T result;
	return ss >> result ? result : 0;
}