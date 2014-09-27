/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "StringBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Internals;

namespace JsonGeneratorTests
{
    TEST_CLASS(StringBuilderTests)
    {
        char buffer[20];
        Print* sb;
        size_t returnValue;

    public:
        
        TEST_METHOD_INITIALIZE(Initialize)
        {
            sb = new StringBuilder(buffer, sizeof(buffer));
        }

        TEST_METHOD(InitialState)
        {
            outputMustBe("");
        }

        TEST_METHOD(OverCapacity)
        {
            print("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            resultMustBe(19);

            print("ABC");
            resultMustBe(0);

            outputMustBe("ABCDEFGHIJKLMNOPQRS");
        }

        TEST_METHOD(EmptyString)
        {
            print("");
            resultMustBe(0);
            outputMustBe("");
        }

        TEST_METHOD(OneString)
        {
            print("ABCD");
            resultMustBe(4);
            outputMustBe("ABCD");
        }

        TEST_METHOD(TwoStrings)
        {
            print("ABCD");
            resultMustBe(4);

            print("EFGH");
            resultMustBe(4);

            outputMustBe("ABCDEFGH");
        }

    private:

        void print(const char* value)
        {
            returnValue = sb->print(value);
        }

        void outputMustBe(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
        }

        void resultMustBe(size_t expected)
        {
            Assert::AreEqual(expected, returnValue);
        }
    };
}
