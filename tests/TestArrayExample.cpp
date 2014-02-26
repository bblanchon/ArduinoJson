#include "CppUnitTest.h"
#include "JsonParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ArduinoJsonParserTests
{
	TEST_CLASS(TestArrayExample)
	{
		char json[128];
		JsonParser<32> parser;
		JsonArray array;

	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			strcpy(json, "[[1.2,3.4],[5.6,7.8]]");
			array = parser.parseArray(json);
		}
		
		TEST_METHOD(Array_Success_ReturnsTrue)
		{
			Assert::IsTrue(array.success());
		}

		TEST_METHOD(Array_GetLength_Returns2)
		{
			Assert::AreEqual(2, array.getLength());
		}

		TEST_METHOD(Array_GetArray0_ReturnsInnerArray0)
		{
			JsonArray innerArray = array.getArray(0);

			Assert::AreEqual(2, innerArray.getLength());
			Assert::AreEqual(1.2, innerArray.getDouble(0));
			Assert::AreEqual(3.4, innerArray.getDouble(1));
		}

		TEST_METHOD(Array_GetArray1_ReturnsInnerArray1)
		{
			JsonArray innerArray = array.getArray(1);

			Assert::AreEqual(2, innerArray.getLength());
			Assert::AreEqual(5.6, innerArray.getDouble(0));
			Assert::AreEqual(7.8, innerArray.getDouble(1));
		}
	};
}