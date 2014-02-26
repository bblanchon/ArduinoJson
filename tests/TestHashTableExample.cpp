#include "CppUnitTest.h"
#include "JsonParser.h"
#include <string>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ArduinoJsonParserTests
{		
	TEST_CLASS(TestHashTableExample)
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
	};
}