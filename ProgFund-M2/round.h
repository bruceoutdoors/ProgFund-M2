#pragma once

#include <cmath>

static float Round(float dbVal, int nPlaces = 1)
{
	const float dbShift = pow(10.0f, nPlaces);
	return  floor(dbVal * dbShift + 0.5f) / dbShift;
}