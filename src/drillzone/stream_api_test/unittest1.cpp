#include "stdafx.h"
#include "CppUnitTest.h"
#include "CppUnitTestLogger.h"
#include "CppUnitTestAssert.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stream_api_test
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Console::WriteLine("here we go");
		}

	};
}