#include "CppUnitTest.h"
#include "JsonArray.h"
#include "JsonHashTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
    TEST_CLASS(JsonHashTableTests)
    {
        JsonHashTable<2> hash;

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

        TEST_METHOD(OneNumber)
        {
            addValue("pi", 3.14);

            jsonIs("{\"pi\":3.14}");
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
            hash.add(key, value);
        }

        template<typename T>
        void addValue(const char* key, T value)
        {
            hash.add(key, value);
        }

        void jsonIs(const char* expected)
        {
            char buffer[256];

            hash.writeTo(buffer, sizeof(buffer));

            Assert::AreEqual(expected, buffer);
        }
    };
}