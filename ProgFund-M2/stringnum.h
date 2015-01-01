/*************************************
Program: stringnum.h
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