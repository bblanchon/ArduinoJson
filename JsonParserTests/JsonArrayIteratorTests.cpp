#include "CppUnitTest.h"
#include "JsonParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Parser;


namespace JsonParserTests
{
	TEST_CLASS(JsonArrayIteratorTests)
	{
	public:
		
		TEST_METHOD(ThreeIntegers)
		{
            char json [] = "[1,2,3]";
            long expected [] = { 1, 2, 3 };
            JsonParser<4> parser;

            JsonArray a = parser.parse(json);

            int index = 0;

            for (long i : a)
            {
                Assert::AreEqual(expected[index++], i);
            }
		}

	};
}