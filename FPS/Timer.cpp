#include "Timer.h"

Timer::Timer()
{
	startTime = std::chrono::steady_clock::now();
}

float Timer::DeltaTime()
{
	const auto old = startTime;
	startTime = std::chrono::steady_clock::now();
	const std::chrono::duration<float> diff = startTime - old;
	return diff.count();
}

float Timer::TotalTime()
{
	return std::chrono::duration<float>(
		std::chrono::steady_clock::now() - startTime).count();
}
