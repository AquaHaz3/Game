#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

namespace debug_util {

	static void panicf(const char* format, ...) 
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		exit(0);
	}

	bool isDebugBoxes();
	void switchDebugBoxes();

}

#include <math.h>

inline float MyVector2Angle(Vector2 v1, Vector2 v2) {
	return atan2f(v2.y - v1.y, v2.x - v1.x);
}