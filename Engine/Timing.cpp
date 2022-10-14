#include "Timing.h"

#include <windows.h>
#include <assert.h>
#include <limits.h>

namespace Engine
{
	namespace Timing
	{
		unsigned int LastFrame_ms;

		LARGE_INTEGER PerformanceFrequency = { 0 };

		void Init()
		{
			BOOL bSuccess = QueryPerformanceFrequency(&PerformanceFrequency);
			assert(bSuccess);
		}

		tick_t GetCurrentTickCounter()
		{
			LARGE_INTEGER CurrentFrameCounter;

			BOOL success = QueryPerformanceCounter(&CurrentFrameCounter);
			assert(success == TRUE);

			return CurrentFrameCounter.QuadPart;
		}

		float GetTimeDiff_ms(tick_t i_StartTime, tick_t i_EndTime)
		{
			assert(PerformanceFrequency.QuadPart != 0);
			return (1000.0f * static_cast<float>(i_EndTime - i_StartTime)) / PerformanceFrequency.QuadPart;
		}

		float GetDeltaTime()
		{
			static tick_t tickCounterBefore = 0;
			float deltaTime = 0;

			tick_t tickCounterAfter = GetCurrentTickCounter();
			deltaTime = (tickCounterBefore == 0) ? (1.0f / FPS) : (GetTimeDiff_ms(tickCounterBefore, tickCounterAfter));

			tickCounterBefore = tickCounterAfter;

			return deltaTime;
		}

		void SetFPS(size_t n_FPS)
		{
			FPS = n_FPS;
		}
	}
}