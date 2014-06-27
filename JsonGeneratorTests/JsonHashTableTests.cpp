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
            assertJsonIs("{}");
        }

    private:

        void assertJsonIs(const char* expected)
        {
            char buffer[256];

            hash.writeTo(buffer, sizeof(buffer));

            Assert::AreEqual(expected, buffer);
        }
    };
}