#include "CppUnitTest.h"
#include "JsonArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{		
    TEST_CLASS(JsonArrayTests)
    {
    public:
        
        TEST_METHOD(EmptyArray)
        {
            JsonArray arr;

            char buffer[256];
            arr.writeTo(buffer, sizeof(buffer));

            Assert::AreEqual("[]", buffer);
        }
    };
}