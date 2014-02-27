#include "CppUnitTest.h"
#include "JsonParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ArduinoJsonParserTests
{
	TEST_CLASS(TestArrays)
	{
		JsonParser<32> parser;

	public:
		
		TEST_METHOD(EmptyString)
		{
			char json[] = "";

			JsonArray array = parser.parseArray(json);
			Assert::IsFalse(array.success());
		}

		TEST_METHOD(EmptyArray)
		{
			char json[] = "[]";

			JsonArray array = parser.parseArray(json);
			Assert::IsTrue(array.success());
		}

		TEST_METHOD(TooFewClosingBrackets)
		{
			char json[] = "[[]";

			JsonArray array = parser.parseArray(json);
			Assert::IsFalse(array.success());
		}

		TEST_METHOD(TooManyClosingBrackets)
		{
			char json[] = "[]]";

			JsonArray array = parser.parseArray(json);
			Assert::IsFalse(array.success());
		}
		
		TEST_METHOD(OneDimensionsArray)
		{
			char json [] = "[0,0]";

			JsonArray array = parser.parseArray(json);
			Assert::IsTrue(array.success());
			Assert::AreEqual(2, array.getLength());

			for (int i = 0; i < 2; i++)
			{
				Assert::AreEqual(0L, array.getLong(i));
			}
		}

		TEST_METHOD(TwoDimensionsArray)
		{
			char json[] = "[[0,0],[0,0]]";

			JsonArray array1 = parser.parseArray(json);
			Assert::IsTrue(array1.success());
			Assert::AreEqual(2, array1.getLength());

			for (int i = 0; i < 2; i++)
			{
				JsonArray array2 = array1.getArray(i);

				Assert::AreEqual(2, array2.getLength());

				for (int j = 0; j < 2; j++)
				{
					Assert::AreEqual(0L, array2.getLong(j));
				}
			}
		}

		TEST_METHOD(TreeDimensionsArray)
		{
			char json[] = "[[[[0,0],[0,0]],[[0,0],[0,0]]]]";

			JsonArray array1 = parser.parseArray(json);
			Assert::IsTrue(array1.success());
			Assert::AreEqual(2, array1.getLength());

			for (int i = 0; i < 2; i++)
			{
				JsonArray array2 = array1.getArray(i);

				Assert::AreEqual(2, array2.getLength());

				for (int j = 0; j < 2; j++)
				{
					JsonArray array3 = array2.getArray(j);

					Assert::AreEqual(2, array3.getLength());

					for (int k = 0; k < 2; k++)
					{
						Assert::AreEqual(0L, array3.getLong(k));
					}
				}
			}
		}
	};
}