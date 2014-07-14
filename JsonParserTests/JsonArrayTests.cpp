/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "JsonParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Parser;

namespace ArduinoJsonParserTests
{
	TEST_CLASS(JsonArrayTests)
	{
		JsonParser<32> parser;
        JsonArray array;
        char json[256];

	public:
		
		TEST_METHOD(EmptyString)
		{
            whenInputIs("");
            parseMustFail();             
		}

		TEST_METHOD(EmptyArray)
		{
            whenInputIs("[]");
            parseMustSucceed();
            lengthMustBe(0);
		}

		TEST_METHOD(TooFewClosingBrackets)
		{
            whenInputIs("[[]");
            parseMustFail();
		}

		TEST_METHOD(TooManyClosingBrackets)
		{
            whenInputIs("[]]");
            parseMustFail();
		}
		
		TEST_METHOD(TwoIntegers)
		{
            whenInputIs("[1,2]");
            parseMustSucceed();
            lengthMustBe(2);
            itemMustBe(0, 1L);
            itemMustBe(1, 2L);
		}

        TEST_METHOD(TwoBooleans)
        {
            whenInputIs("[true,false]");
            parseMustSucceed();
            lengthMustBe(2);
            itemMustBe(0, true);
            itemMustBe(1, false);
        }

        TEST_METHOD(TwoStrings)
        {
            whenInputIs("[\"hello\",\"world\"]");
            parseMustSucceed();
            lengthMustBe(2);
            itemMustBe(0, "hello");
            itemMustBe(1, "world");
        }

		TEST_METHOD(TwoDimensionsArray)
		{
            whenInputIs("[[1,2],[3,4]]");
            parseMustSucceed();
            lengthMustBe(2);
            itemMustBe(0, 0, 1L);
            itemMustBe(0, 1, 2L);
            itemMustBe(1, 0, 3L);
            itemMustBe(1, 1, 4L);
		}

        TEST_METHOD(ThreeDimensionsArray)
        {
            whenInputIs("[[[1,2],[3,4]],[[5,6],[7,8]]]");
            parseMustSucceed();
            lengthMustBe(2);
            itemMustBe(0, 0, 0, 1L);
            itemMustBe(0, 0, 1, 2L);
            itemMustBe(0, 1, 0, 3L);
            itemMustBe(0, 1, 1, 4L);
            itemMustBe(1, 0, 0, 5L);
            itemMustBe(1, 0, 1, 6L);
            itemMustBe(1, 1, 0, 7L);
            itemMustBe(1, 1, 1, 8L);
        }        
		
    private:

        void whenInputIs(const char* input)
        {
            strcpy(json, input);
            array = parser.parseArray(json);
        }

        void parseMustFail()
        {
            Assert::IsFalse(array.success());
            lengthMustBe(0);
        }

        void parseMustSucceed()
        {
            Assert::IsTrue(array.success());
        }

        void lengthMustBe(int expected)
        {
            Assert::AreEqual(expected, array.getLength());
        }

        void itemMustBe(int index, long expected)
        {
            Assert::AreEqual(expected, array.getLong(index));
        }

        void itemMustBe(int index, bool expected)
        {
            Assert::AreEqual(expected, array.getBool(index));
        }

        void itemMustBe(int index, const char* expected)
        {
            Assert::AreEqual(expected, array.getString(index));
        }

        void itemMustBe(int index0, int index1, long expected)
        {
            Assert::AreEqual(expected, array.getArray(index0).getLong(index1));
        }

        void itemMustBe(int index0, int index1, int index2, long expected)
        {
            Assert::AreEqual(expected, array.getArray(index0).getArray(index1).getLong(index2));
        }
	};
}