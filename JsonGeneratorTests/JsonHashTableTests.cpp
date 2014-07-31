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
    TEST_CLASS(JsonHashTableTests)
    {
        JsonHashTable<2> hash;
        char buffer[256];

    public:
        
        TEST_METHOD(Empty)
        {
            outputMustBe("{}");
        }

        TEST_METHOD(OneString)
        {
            add("key", "value");
            outputMustBe("{\"key\":\"value\"}");
        }

        TEST_METHOD(TwoStrings)
        {
            add("key1", "value1");
            add("key2", "value2");
            outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(ReplaceExistingKey)
        {
            add("key", "value1");
            add("key", "value2");
            outputMustBe("{\"key\":\"value2\"}");
        }

        TEST_METHOD(OneStringOverCapacity)
        {
            add("key1", "value1");
            add("key2", "value2");
            add("key3", "value3");

            outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(OneInteger)
        {
            add("key", 1);
            outputMustBe("{\"key\":1}");
        }

        TEST_METHOD(OneDoubleFourDigits)
        {
            add<4>("key", 3.14159265358979323846);
            outputMustBe("{\"key\":3.1416}");
        }

        TEST_METHOD(OneDoubleDefaultDigits)
        {
            add("key", 3.14159265358979323846);
            outputMustBe("{\"key\":3.14}");
        }

        TEST_METHOD(OneNull)
        {
            add("key", (char*) 0);
            outputMustBe("{\"key\":null}");
        }

        TEST_METHOD(OneTrue)
        {
            add("key", true);
            outputMustBe("{\"key\":true}");
        }

        TEST_METHOD(OneFalse)
        {
            add("key", false);
            outputMustBe("{\"key\":false}");
        }

        TEST_METHOD(OneEmptyNestedArray)
        {
            addNested("key", JsonArray<1>());
            outputMustBe("{\"key\":[]}");
        }

        TEST_METHOD(OneEmptyNestedHash)
        {
            addNested("key", JsonHashTable<1>());
            outputMustBe("{\"key\":{}}");
        }

    private:
        
        void addNested(const char* key, Printable& value)
        {
            hash[key] = value;
        }

        template<typename T>
        void add(const char* key, T value)
        {
            hash[key] = value;
        }

        template<int DIGITS>
        void add(const char* key, double value)
        {
            hash[key].set<DIGITS>(value);
        }

        void outputMustBe(const char* expected)
        {            
            size_t actual = hash.printTo(buffer, sizeof(buffer));
            Assert::AreEqual(expected, buffer);
            Assert::AreEqual(strlen(expected), actual);
        }
    };
}