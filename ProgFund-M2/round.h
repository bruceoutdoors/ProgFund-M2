/*************************************
Program: round.h
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

#include <cmath>

static float Round(float dbVal, int nPlaces = 1)
{
	const float dbShift = pow(10.0f, nPlaces);
	return  floor(dbVal * dbShift + 0.5f) / dbShift;
}