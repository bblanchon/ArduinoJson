/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "JsonArray.h"
#include "JsonObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Generator;

namespace JsonGeneratorTests
{
    TEST_CLASS(JsonObject_Indexer_Tests)
    {
        JsonObject<2> object;

    public:

        TEST_METHOD(Empty)
        {
            mustNotContain("key");
        }

        TEST_METHOD(OneString)
        {
            object["key"] = "value";

            mustContain("key", "value");
        }


    private:

        void mustContain(const char* key, const char* expected)
        {
            Assert::IsTrue(object.containsKey(key));

            const char* actual = object[key];
            Assert::AreEqual(expected, actual);
        }

        void mustNotContain(const char* key)
        {
            Assert::IsFalse(object.containsKey(key));

            const char* actual = object[key];
            Assert::IsNull(actual);
        }
    };
}