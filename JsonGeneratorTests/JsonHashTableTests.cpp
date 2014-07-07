#include "CppUnitTest.h"
#include "JsonArray.h"
#include "JsonHashTable.h"

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
            jsonIs("{}");
        }

        TEST_METHOD(OneString)
        {
            addValue("key", "value");

            jsonIs("{\"key\":\"value\"}");
        }

        TEST_METHOD(TwoStrings)
        {
            addValue("key1", "value1");
            addValue("key2", "value2");

            jsonIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(OneStringOverCapacity)
        {
            addValue("key1", "value1");
            addValue("key2", "value2");
            addValue("key3", "value3");

            jsonIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(OneInteger)
        {
            addValue("key", 1);
            jsonIs("{\"key\":1}");
        }

        TEST_METHOD(OneDoubleFourDigits)
        {
            addValue<4>("key", 3.14159265358979323846);
            jsonIs("{\"key\":3.1416}");
        }

        TEST_METHOD(OneDoubleDefaultDigits)
        {
            addValue("key", 3.14159265358979323846);
            jsonIs("{\"key\":3.14}");
        }

        TEST_METHOD(OneNull)
        {
            addValue("key", (char*) 0);
            jsonIs("{\"key\":null}");
        }

        TEST_METHOD(OneTrue)
        {
            addValue("key", true);
            jsonIs("{\"key\":true}");
        }

        TEST_METHOD(OneFalse)
        {
            addValue("key", false);
            jsonIs("{\"key\":false}");
        }

        TEST_METHOD(OneEmptyNestedArray)
        {
            JsonArray<1> nestedArray;
            addNested("key", nestedArray);

            jsonIs("{\"key\":[]}");
        }

        TEST_METHOD(OneEmptyNestedHash)
        {
            JsonHashTable<1> nestedHash;
            addNested("key", nestedHash);

            jsonIs("{\"key\":{}}");
        }

    private:
        
        void addNested(const char* key, JsonObjectBase& value)
        {
            hash.add<JsonObjectBase&>(key, value);
        }

        template<typename T>
        void addValue(const char* key, T value)
        {
            hash.add(key, value);
        }

        template<int DIGITS>
        void addValue(const char* key, double value)
        {
            hash.add<DIGITS>(key, value);
        }

        void jsonIs(const char* expected)
        {            
            size_t actual = hash.printTo(buffer, sizeof(buffer));
            Assert::AreEqual(expected, buffer);
            Assert::AreEqual(strlen(expected), actual);
        }
    };
}