/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "JsonParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Parser;

namespace JsonParserTests
{
    TEST_CLASS(JsonObjectIteratorTests)
    {
    public:

        TEST_METHOD(EmptyObject)
        {
            char json [] = "{}";
            JsonParser<1> parser;

            JsonHashTable a = parser.parse(json);

            int loopCount = 0;

            for (auto i : a)
            {
                loopCount++;
            }

            Assert::AreEqual(0, loopCount);
        }

        TEST_METHOD(EmptyJson)
        {
            char json[] = "";
            JsonParser<1> parser;

            JsonHashTable a = parser.parse(json);

            int loopCount = 0;

            for (auto i : a)
            {
                loopCount++;
            }

            Assert::AreEqual(0, loopCount);
        }

        TEST_METHOD(ThreeStrings)
        {
            char json[] = "{\"key1\":\"value1\",\"key2\":\"value2\",\"key3\":\"value3\"}";
            char* expectedKeys[] = {"key1", "key2", "key3"};
            char* expectedValues[] = {"value1", "value2", "value3"};
            JsonParser<7> parser;

            JsonHashTable a = parser.parse(json);

            int index = 0;

            for (auto i : a)
            {
                Assert::AreEqual(expectedKeys[index], i.key());
                Assert::AreEqual(expectedValues[index], (const char*) i.value());
                index++;
            }
        }
    };
}
