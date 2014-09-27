/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "JsonParser.h"
#include <string>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Parser;

namespace ArduinoJsonParserTests
{
    TEST_CLASS(JsonHashTableTests)
    {
        JsonHashTable hashTable;
        JsonArray nestedArray;
        char json[256];
        jsmntok_t tokens[32];
        JsonParserBase parser = JsonParserBase(tokens, 32);

    public:

        TEST_METHOD(EmptyHashTable)
        {
            whenInputIs("{}");
            parseMustSucceed();
        }

        TEST_METHOD(NotEnoughTokens)
        {
            setTokenCountTo(2);

            whenInputIs("{\"key\":0}");

            parseMustFail();
            itemMustNotExist("key");
        }

        TEST_METHOD(TwoIntegers)
        {
            setTokenCountTo(5);

            whenInputIs("{\"key1\":1,\"key2\":2}");

            parseMustSucceed();
            itemMustBe("key1", 1L);
            itemMustBe("key2", 2L);
            itemMustNotExist("key3");
        }

        TEST_METHOD(TwoBooleans)
        {
            setTokenCountTo(5);

            whenInputIs("{\"key1\":true,\"key2\":false}");

            parseMustSucceed();
            itemMustBe("key1", true);
            itemMustBe("key2", false);
            itemMustNotExist("key3");
        }

        TEST_METHOD(TwoStrings)
        {
            setTokenCountTo(5);

            whenInputIs("{\"key1\":\"hello\",\"key2\":\"world\"}");

            parseMustSucceed();
            itemMustBe("key1", "hello");
            itemMustBe("key2", "world");
            itemMustNotExist("key3");
        }

        TEST_METHOD(TwoNestedArrays)
        {
            setTokenCountTo(9);

            whenInputIs("{\"key1\":[1,2],\"key2\":[3,4]}");
            parseMustSucceed();

            itemMustBeAnArray("key1");
            arrayLengthMustBe(2);
            arrayItemMustBe(0, 1L);
            arrayItemMustBe(1, 2L);
            arrayItemMustBe(2, 0L);

            itemMustBeAnArray("key2");
            arrayLengthMustBe(2);
            arrayItemMustBe(0, 3L);
            arrayItemMustBe(1, 4L);
            arrayItemMustBe(2, 0L);

            itemMustNotExist("key3");
        }

    private:

        void setTokenCountTo(int n)
        {
            parser = JsonParserBase(tokens, n);
        }

        void whenInputIs(const char* input)
        {
            strcpy(json, input);
            hashTable = parser.parseHashTable(json);
        }

        void parseMustFail()
        {
            Assert::IsFalse(hashTable.success());
        }

        void parseMustSucceed()
        {
            Assert::IsTrue(hashTable.success());
        }

        void itemMustBe(const char* key, long expected)
        {
            Assert::AreEqual(expected, hashTable.getLong(key));
        }

        void itemMustBe(const char* key, bool expected)
        {
            Assert::AreEqual(expected, hashTable.getBool(key));
        }

        void itemMustBe(const char* key, const char* expected)
        {
            Assert::AreEqual(expected, hashTable.getString(key));
        }

        void itemMustNotExist(const char* key)
        {
            Assert::IsFalse(hashTable.containsKey(key));
            Assert::IsFalse(hashTable.getHashTable(key).success());
            Assert::IsFalse(hashTable.getArray(key).success());
            Assert::IsFalse(hashTable.getBool(key));
            Assert::AreEqual(0.0, hashTable.getDouble(key));
            Assert::AreEqual(0L, hashTable.getLong(key));
            Assert::IsNull(hashTable.getString(key));
        }

        void itemMustBeAnArray(const char* key)
        {
            nestedArray = hashTable.getArray(key);
            Assert::IsTrue(nestedArray.success());
        }

        void arrayLengthMustBe(int expected)
        {
            Assert::AreEqual(expected, nestedArray.getLength());
        }

        void arrayItemMustBe(int index, long expected)
        {
            Assert::AreEqual(expected, nestedArray.getLong(index));
        }
    };
}
