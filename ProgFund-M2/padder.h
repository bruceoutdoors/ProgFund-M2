#pragma once

#include <string>

using namespace std;

static string addPadding(const string &str, size_t padding)
{
	string result = str;
	int pads = padding - str.size();
	while (pads-- > 0) result.append(" ");

	return result;
}