#pragma once

#include <stdint.h>

namespace Engine
{
	namespace Timing
	{
		typedef uint64_t tick_t;

		static size_t FPS = 60;

		void Init();

		tick_t GetCurrentTickCounter();
		float GetTimeDiff_ms(tick_t i_StartTime, tick_t i_EndTime = GetCurrentTickCounter());

		float GetDeltaTime();

		void SetFPS(size_t n_FPS);

		inline float CvtSecondsToMilliseconds(float i_Seconds);
		inline float CvtPerSecondToPerMillisecond(float i_PerSecond);
	}
}