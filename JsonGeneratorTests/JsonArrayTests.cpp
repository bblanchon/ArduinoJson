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
            returnValueIs(2);
            jsonIs("[]");
        }

        TEST_METHOD(Null)
        {
            addValue((char*)0);

            returnValueIs(6);
            jsonIs("[null]");
        }

        TEST_METHOD(OneString)
        {
            addValue("hello");

            returnValueIs(9);
            jsonIs("[\"hello\"]");
        }

        TEST_METHOD(TwoStrings)
        {
            addValue("hello");
            addValue("world");

            returnValueIs(17);
            jsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(OneStringOverCapacity)
        {
            addValue("hello");
            addValue("world");
            addValue("lost");

            returnValueIs(17);
            jsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(OneNumber)
        {
            addValue(3.14);

            returnValueIs(6);
            jsonIs("[3.14]");
        }

        TEST_METHOD(TwoNumbers)
        {
            addValue(3.14);
            addValue(2.72);

            returnValueIs(11);
            jsonIs("[3.14,2.72]");
        }

        TEST_METHOD(OneNumberOverCapacity)
        {
            addValue(3.14);
            addValue(2.72);
            addValue(1.41);

            returnValueIs(11);
            jsonIs("[3.14,2.72]");
        }

        TEST_METHOD(OneTrue)
        {
            addValue(true);

            returnValueIs(6);
            jsonIs("[true]");
        }

        TEST_METHOD(OneFalse)
        {
            addValue(false);

            returnValueIs(7);
            jsonIs("[false]");
        }

        TEST_METHOD(TwoBooleans)
        {
            addValue(false);
            addValue(true);

            returnValueIs(12);
            jsonIs("[false,true]");
        }

        TEST_METHOD(OneBooleanOverCapacity)
        {
            addValue(false);
            addValue(true);
            addValue(false);

            returnValueIs(12);
            jsonIs("[false,true]");
        }

        TEST_METHOD(OneEmptyNestedArray)
        {
            JsonArray<1> nestedArray;
            
            addNested(nestedArray);

            returnValueIs(4);
            jsonIs("[[]]");
        }

        TEST_METHOD(OneEmptyNestedHash)
        {
            JsonHashTable<1> nestedHash;

            addNested(nestedHash);

            returnValueIs(4);
            jsonIs("[{}]");
        }

        TEST_METHOD(OneNestedArrayWithOneItem)
        {
            JsonArray<1> nestedArray;
            nestedArray.add(3.14);

            addNested(nestedArray);

            returnValueIs(8);
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

        void returnValueIs(size_t expected)
        {
            size_t actual = arr.printTo(buffer, sizeof(buffer));
            Assert::AreEqual(expected, actual);
        }
    };
}