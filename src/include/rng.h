#ifndef __RNG__
#define __RNG__

#include <time.h> //rng
#include <chrono> //rng
#include <random> //rng 

static std::mt19937 rng(
		std::chrono::steady_clock::now().time_since_epoch().count()
		);

#endif
