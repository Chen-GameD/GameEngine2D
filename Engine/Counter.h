#pragma once

namespace Engine
{
	class Counter
	{
	public:
		size_t shareCount = 0;
		size_t weakCount = 0;
	};
}