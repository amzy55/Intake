#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	t0 = high_resolution_clock::now();
	t1 = t0;

	elapsedTime = 0.0;
	totalTime = 0.0;
}

Timer& Timer::Get()
{
	static Timer t;
	return t;
}

void Timer::Tick()
{
	t1 = high_resolution_clock::now();
	auto delta = t1 - t0;
	t0 = t1;

	elapsedTime = static_cast<double>(delta.count());
	totalTime += elapsedTime;
}

float Timer::ElapsedMilliSeconds() const
{
	return static_cast<float>(elapsedTime * 1e-6);
}

float Timer::ElapsedSeconds() const
{
	return static_cast<float>(elapsedTime * 1e-9);
}

float Timer::TotalTimeSeconds() const
{
	return static_cast<float>(totalTime * 1e-9);
}

float Timer::TotalTimeMilliseconds() const
{
	return static_cast<float>(totalTime * 1e-6);
}
