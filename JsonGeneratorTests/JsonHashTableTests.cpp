#include "CppUnitTest.h"
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
            add("key", "value");
            jsonIs("{\"key\":\"value\"}");
        }

        TEST_METHOD(TwoStrings)
        {
            add("key1", "value1");
            add("key2", "value2"); 
            jsonIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(OneStringOverCapacity)
        {
            add("key1", "value1");
            add("key2", "value2");
            add("key3", "value3");
            jsonIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

    private:

        template<typename T>
        void add(const char* key, T value)
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