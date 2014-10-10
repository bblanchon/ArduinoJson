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
    TEST_CLASS(JsonObject_PrintTo_Tests)
    {
        JsonObject<2> object;

    public:

        TEST_METHOD(Empty)
        {
            outputMustBe("{}");
        }

        TEST_METHOD(OneString)
        {
            object["key"] = "value";

            outputMustBe("{\"key\":\"value\"}");
        }

        TEST_METHOD(TwoStrings)
        {
            object["key1"] = "value1";
            object["key2"] = "value2";

            outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(RemoveFirst)
        {
            object["key1"] = "value1";
            object["key2"] = "value2";
            object.remove("key1");

            outputMustBe("{\"key2\":\"value2\"}");
        }

        TEST_METHOD(RemoveLast)
        {
            object["key1"] = "value1";
            object["key2"] = "value2";
            object.remove("key2");

            outputMustBe("{\"key1\":\"value1\"}");
        }

        TEST_METHOD(RemoveUnexistingKey)
        {
            object["key1"] = "value1";
            object["key2"] = "value2";
            object.remove("key3");

            outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(ReplaceExistingKey)
        {
            object["key"] = "value1";
            object["key"] = "value2";

            outputMustBe("{\"key\":\"value2\"}");
        }

        TEST_METHOD(OneStringOverCapacity)
        {
            object["key1"] = "value1";
            object["key2"] = "value2";
            object["key3"] = "value3";

            outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(OneInteger)
        {
            object["key"] = 1;
            outputMustBe("{\"key\":1}");
        }

        TEST_METHOD(OneDoubleFourDigits)
        {
            object["key"].set<4>(3.14159265358979323846);
            outputMustBe("{\"key\":3.1416}");
        }

        TEST_METHOD(OneDoubleDefaultDigits)
        {
            object["key"] = 3.14159265358979323846;
            outputMustBe("{\"key\":3.14}");
        }

        TEST_METHOD(OneNull)
        {
            object["key"] = static_cast<char*>(0);
            outputMustBe("{\"key\":null}");
        }

        TEST_METHOD(OneTrue)
        {
            object["key"] = true;
            outputMustBe("{\"key\":true}");
        }

        TEST_METHOD(OneFalse)
        {
            object["key"] = false;
            outputMustBe("{\"key\":false}");
        }

        TEST_METHOD(OneEmptyNestedArray)
        {
            auto nestedArray = JsonArray<1>();

            object["key"] = nestedArray;
            
            outputMustBe("{\"key\":[]}");
        }

        TEST_METHOD(OneEmptyNestedObject)
        {
            auto nestedObject = JsonObject<1>();

            object["key"] = nestedObject;

            outputMustBe("{\"key\":{}}");
        }

    private:

        void outputMustBe(const char* expected)
        {
            char buffer[256];
            size_t result;
            
            result = object.printTo(buffer, sizeof(buffer));
            
            Assert::AreEqual(strlen(expected), result);
            Assert::AreEqual(expected, buffer);
        }
    };
}
