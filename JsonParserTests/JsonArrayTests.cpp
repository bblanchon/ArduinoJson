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
        JsonArray array;
        char json[256];
        jsmntok_t tokens[32];
        JsonParserBase parser = JsonParserBase(tokens, 32);

    public:
        
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
        
        TEST_METHOD(EmptyArray)
        {
            whenInputIs("[]");
            parseMustSucceed();
            lengthMustBe(0);
        }

        TEST_METHOD(NotEnoughTokens)
        {
            setTokenCountTo(2);

            whenInputIs("[1,2]");

            parseMustFail();
            itemMustNotExist(0);
        }

        TEST_METHOD(TwoIntegers)
        {
            setTokenCountTo(3);

            whenInputIs("[1,2]");

            parseMustSucceed();
            lengthMustBe(2);
            itemMustBe(0, 1L);
            itemMustBe(1, 2L);
            itemMustNotExist(2);
        }

        TEST_METHOD(TwoBooleans)
        {
            setTokenCountTo(3);

            whenInputIs("[true,false]");

            parseMustSucceed();
            lengthMustBe(2);
            itemMustBe(0, true);
            itemMustBe(1, false);
            itemMustNotExist(2);
        }

        TEST_METHOD(TwoStrings)
        {
            setTokenCountTo(3);

            whenInputIs("[\"hello\",\"world\"]");

            parseMustSucceed();
            lengthMustBe(2);
            itemMustBe(0, "hello");
            itemMustBe(1, "world");
            itemMustNotExist(2);
        }

        TEST_METHOD(TwoDimensionsArray)
        {
            setTokenCountTo(7);

            whenInputIs("[[1,2],[3,4]]");

            parseMustSucceed();
            lengthMustBe(2);
            itemMustBe(0, 0, 1L);
            itemMustBe(0, 1, 2L);
            itemMustBe(1, 0, 3L);
            itemMustBe(1, 1, 4L);
            itemMustNotExist(2);
        }

        TEST_METHOD(ThreeDimensionsArray)
        {
            setTokenCountTo(15);

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
            itemMustNotExist(2);
        }        
        
    private:

        void setTokenCountTo(int n)
        {
            parser = JsonParserBase(tokens, n);
        }

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

        void itemMustNotExist(int index)
        {
            Assert::IsFalse(array.getHashTable(index).success());
            Assert::IsFalse(array.getArray(index).success());
            Assert::IsFalse(array.getBool(index));
            Assert::AreEqual(0.0, array.getDouble(index));
            Assert::AreEqual(0L, array.getLong(index));
            Assert::IsNull(array.getString(index));
        }
    };
}
