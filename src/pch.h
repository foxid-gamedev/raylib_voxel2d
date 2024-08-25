#pragma once

// standard library
#include <iostream>
#include <algorithm>
#include <string>
#include <array>
#include <memory>
#include <cassert>
#include <cfenv>
#include <functional>
#include <cmath>
#include <time.h>

// raylib
#include <raylib.h>

// macros
#define DO_NOTHING_FUNC [](){}

// utility functions
namespace util
{
	__forceinline int sign(float value) { return value >= 0 ? 1 : -1; }

	__forceinline float remap(float value, float astart, float aend, float bstart, float bend) {
		assert(astart - aend != 0);
		assert(bstart - bend != 0);
		float fac = value / (astart - aend);
		return fac * (bstart - bend);
	}
}
