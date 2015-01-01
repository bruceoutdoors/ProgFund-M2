/*************************************
Program: padder.h
Course:  Programming Fundamentals
Year:    2014/10 Trimester 1
Name:    LEE ZHEN YONG
ID:      1122702848
Lecture: TC01
Lab:     TT01
Email:   bruceoutdoors@gmail.com
Phone:   016-3188854
*************************************/

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