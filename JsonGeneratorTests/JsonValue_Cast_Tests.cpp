/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "StringBuilder.h"
#include "JsonValue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Generator;
using namespace ArduinoJson::Internals;

namespace JsonGeneratorTests
{
    TEST_CLASS(JsonValue_Cast_Tests)
    {
        JsonValue value;

    public:

        TEST_METHOD(String)
        {            
            value = "hello";
            mustCastTo("hello");
        }

    private:

        void mustCastTo(const char* expected)
        {
            const char* actual = value;
            Assert::AreEqual(expected, actual);
        }
    };
}