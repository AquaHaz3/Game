#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

namespace debug_util {

	static void panicf(const char* format, ...) {
		va_list list;
		vprintf(format, list);
		exit(0);
	}

	bool isDebugBoxes();
	void switchDebugBoxes();

}