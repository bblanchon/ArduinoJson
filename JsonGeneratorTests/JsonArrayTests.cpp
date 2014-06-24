#include "CppUnitTest.h"
#include "JsonArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{		
    TEST_CLASS(JsonArrayTests)
    {
        JsonArray<32> arr;
        
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

        void AssertJsonIs(const char* expected)
        {      
            char buffer[256];

            arr.writeTo(buffer, sizeof(buffer));

            Assert::AreEqual(expected, buffer);
        }
    };
}