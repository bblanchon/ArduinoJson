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
		char json[128];
		JsonParser<32> parser;
		JsonHashTable hashTable;

	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			strcpy(json, "{\"Name\":\"Blanchon\",\"Skills\":[\"C\",\"C++\",\"C#\"],\"Age\":32,\"Online\":true}");
			hashTable = parser.parseHashTable(json);
		}
						
		TEST_METHOD(HashTable_Success_ReturnsTrue)
		{
			Assert::IsTrue(hashTable.success());
		}			

		TEST_METHOD(HashTable_GetString_ReturnsExpectedValue)
		{
			string name = hashTable.getString("Name");
			Assert::AreEqual(name, string("Blanchon"));
		}

		TEST_METHOD(HashTable_GetArray_ReturnsExpectedValue)
		{
			JsonArray skills = hashTable.getArray("Skills");

			string skill0 = skills.getString(0);
			Assert::AreEqual(skill0, string("C"));

			string skill1 = skills.getString(1);
			Assert::AreEqual(skill1, string("C++"));

			string skill2 = skills.getString(2);
			Assert::AreEqual(skill2, string("C#"));
		}

		TEST_METHOD(HashTable_GetLong_ReturnsExpectedValue)
		{
			int age = hashTable.getLong("Age");
			Assert::AreEqual(32, age);
		}

		TEST_METHOD(HashTable_GetBool_ReturnsExpectedValue)
		{
			bool online = hashTable.getBool("Online");
			Assert::AreEqual(true, online);
		}

        TEST_METHOD(OneDimensionArrayInHashTable)
        {
            char json[] = "{a:[0,0],b:[0,0]}";

            JsonHashTable root = parser.parseHashTable(json);
            Assert::IsTrue(root.success());

            JsonArray arrayA = root.getArray("a");
            Assert::IsTrue(arrayA.success());
            Assert::AreEqual(2, arrayA.getLength());

            JsonArray arrayB = root.getArray("b");
            Assert::IsTrue(arrayB.success());
            Assert::AreEqual(2, arrayB.getLength());
        }

        TEST_METHOD(TwoDimensionsArrayInHashTable)
        {
            char json[] = "{a:[[0],[0]],b:[[0],[0]]}";

            JsonHashTable root = parser.parseHashTable(json);
            Assert::IsTrue(root.success());

            JsonArray arrayA = root.getArray("a");
            Assert::IsTrue(arrayA.success());
            Assert::AreEqual(2, arrayA.getLength());

            JsonArray arrayB = root.getArray("b");
            Assert::IsTrue(arrayB.success());
            Assert::AreEqual(2, arrayB.getLength());
        }
	};
}