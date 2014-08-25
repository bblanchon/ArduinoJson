/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "IndentedPrintDecorator.h"
#include "StringBuilder.h"

using namespace ArduinoJson::Internals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
	TEST_CLASS(IntentedPrintTests)
	{
        char buffer[256];

	public:
		
		TEST_METHOD(EmptyString)
		{
            whenInputIs("");
            outputMustBe("");
		}

    private:

        void whenInputIs(const char input[])
        {
            StringBuilder sb(buffer, sizeof(buffer));
            IndentedPrintDecorator decorator(sb);

            decorator.print(input);
        }

        void outputMustBe(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
        }
	};
}