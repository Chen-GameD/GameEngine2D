#include "pch.h"
#include "CppUnitTest.h"
#include "../Engine/SmartPtrs.h"
#include "../Engine/WeakPtrs.h"
#include "../Engine/Counter.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestSmartPtrs
{
	TEST_CLASS(UnitTestSmartPtrs)
	{
	public:
		
		TEST_METHOD(SmartPtr)
		{
			Engine::SmartPtrs<int> sp(new int(10));
			Engine::SmartPtrs<int> sp2(sp);
			Engine::SmartPtrs<int> sp3(new int(20));
			sp2 = sp3;
			std::cout << sp.use_count() << std::endl;
			std::cout << sp3.use_count() << std::endl;
		}
	};
}
