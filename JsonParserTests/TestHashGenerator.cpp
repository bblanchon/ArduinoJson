#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ArduinoJsonParserTests
{
	TEST_CLASS(TestHashGenerator)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
            JsonArray<5> arr;
            arr.Add(1);
            arr.Add("Hi!");

            JsonHashTable<4> hash;
            hash.Add("key1", 1);
            hash.Add("key2", "Hello!");
            hash.Add("key3", arr);
		}

	};
}
