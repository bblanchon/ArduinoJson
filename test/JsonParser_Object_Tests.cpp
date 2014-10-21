#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

class JsonParser_Object_Test : public testing::Test
{
protected:

	void whenInputIs(const char* jsonString)
	{
		strcpy(_jsonString, jsonString);
		_object = _jsonBuffer.parseObject(_jsonString);
	}

	void parseMustSucceed()
	{
		EXPECT_TRUE(_object.success());
	}

	void parseMustFail()
	{
		EXPECT_FALSE(_object.success());
	}

	void sizeMustBe(int expected)
	{
		EXPECT_EQ(expected, _object.size());
	}

private:
	StaticJsonBuffer<10> _jsonBuffer;
	JsonObject _object;
	char _jsonString[256];
};

TEST_F(JsonParser_Object_Test, EmptyObject)
{
    whenInputIs("{}");
    parseMustSucceed();
    sizeMustBe(0);
}

TEST_F(JsonParser_Object_Test, MissingClosingBrace)
{
	whenInputIs("{");
	parseMustFail();
	sizeMustBe(0);
}