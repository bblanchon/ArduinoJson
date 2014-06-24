#include "CppUnitTest.h"
#include "JsonArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{		
    TEST_CLASS(JsonArrayTests)
    {
        JsonArray<3> arr;
        
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

        void AssertJsonIs(const char* expected)
        {      
            char buffer[256];

            arr.writeTo(buffer, sizeof(buffer));

            Assert::AreEqual(expected, buffer);
        }
    };
}