#include "CppUnitTest.h"
#include "JsonParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Parser;


namespace JsonParserTests
{
	TEST_CLASS(JsonArrayIteratorTests)
	{
	public:
		
		TEST_METHOD(SimpleIteraton)
		{
            char json [] = "[1,2,3]";
            JsonParser<4> parser;

            JsonArray a = parser.parse(json);

            long expected = 1;

            for (auto i : a)
            {
                Assert::AreEqual(expected, (long)*i);
                expected++;
            }
		}

	};
}