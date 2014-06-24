#include "CppUnitTest.h"
#include "JsonArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{		
    TEST_CLASS(JsonArrayTests)
    {
        JsonArray<2> arr;
        
    public:
        
        TEST_METHOD(EmptyArray)
        {
            AssertJsonIs("[]");
        }

        TEST_METHOD(OneString)
        {
            arr.add("hello");

            AssertJsonIs("['hello']");
        }

        TEST_METHOD(TwoStrings)
        {
            arr.add("hello");
            arr.add("world");

            AssertJsonIs("['hello','world']");
        }

        TEST_METHOD(OverCapacity)
        {
            arr.add("hello");
            arr.add("world");
            arr.add("lost");

            AssertJsonIs("['hello','world']");
        }

        TEST_METHOD(OneNumber)
        {
            arr.add(3.14);

            AssertJsonIs("[3.14]");
        }

        void AssertJsonIs(const char* expected)
        {      
            char buffer[256];

            arr.writeTo(buffer, sizeof(buffer));

            Assert::AreEqual(expected, buffer);
        }
    };
}