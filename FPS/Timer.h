#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	~Timer() = default;

	float DeltaTime();
	float TotalTime();

private:
	std::chrono::steady_clock::time_point startTime;
};

