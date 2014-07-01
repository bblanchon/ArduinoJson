#include "CppUnitTest.h"
#include "JsonArray.h"
#include "JsonHashTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{		
    TEST_CLASS(JsonArrayTests)
    {
        JsonArray<2> arr;
        char buffer[256];

    public:
        
        TEST_METHOD(Empty)
        {
            returns(2);
            jsonIs("[]");
        }

        TEST_METHOD(Null)
        {
            addValue((char*)0);

            returns(6);
            jsonIs("[null]");
        }

        TEST_METHOD(OneString)
        {
            addValue("hello");

            returns(9);
            jsonIs("[\"hello\"]");
        }

        TEST_METHOD(TwoStrings)
        {
            addValue("hello");
            addValue("world");

            returns(17);
            jsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(OneStringOverCapacity)
        {
            addValue("hello");
            addValue("world");
            addValue("lost");

            returns(17);
            jsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(OneNumber)
        {
            addValue(3.14);

            returns(6);
            jsonIs("[3.14]");
        }

        TEST_METHOD(TwoNumbers)
        {
            addValue(3.14);
            addValue(2.72);

            returns(11);
            jsonIs("[3.14,2.72]");
        }

        TEST_METHOD(OneNumberOverCapacity)
        {
            addValue(3.14);
            addValue(2.72);
            addValue(1.41);

            returns(11);
            jsonIs("[3.14,2.72]");
        }

        TEST_METHOD(OneTrue)
        {
            addValue(true);

            returns(6);
            jsonIs("[true]");
        }

        TEST_METHOD(OneFalse)
        {
            addValue(false);

            returns(7);
            jsonIs("[false]");
        }

        TEST_METHOD(TwoBooleans)
        {
            addValue(false);
            addValue(true);

            returns(12);
            jsonIs("[false,true]");
        }

        TEST_METHOD(OneBooleanOverCapacity)
        {
            addValue(false);
            addValue(true);
            addValue(false);

            returns(12);
            jsonIs("[false,true]");
        }

        TEST_METHOD(OneEmptyNestedArray)
        {
            JsonArray<1> nestedArray;
            
            addNested(nestedArray);

            returns(4);
            jsonIs("[[]]");
        }

        TEST_METHOD(OneEmptyNestedHash)
        {
            JsonHashTable<1> nestedHash;

            addNested(nestedHash);

            returns(4);
            jsonIs("[{}]");
        }

        TEST_METHOD(OneNestedArrayWithOneItem)
        {
            JsonArray<1> nestedArray;
            nestedArray.add(3.14);

            addNested(nestedArray);

            returns(8);
            jsonIs("[[3.14]]");
        }

    private:

        void addNested(JsonObjectBase& value)
        {
            arr.add(value);
        }

        template<typename T>
        void addValue(T value)
        {
            arr.add(value);
        }

        void jsonIs(const char* expected)
        {      
            arr.printTo(buffer, sizeof(buffer));
            Assert::AreEqual(expected, buffer);
        }

        void returns(size_t expected)
        {
            size_t actual = arr.printTo(buffer, sizeof(buffer));
            Assert::AreEqual(expected, actual);
        }
    };
}