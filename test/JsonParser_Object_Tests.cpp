#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>
#include <ArduinoJson/JsonValue.hpp>

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

	void keyMustHaveValue(const char* key, const char* expected)
	{
		EXPECT_STREQ(expected, static_cast<const char*>(_object[key]));
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

TEST_F(JsonParser_Object_Test, OneStringNoSpace)
{
	whenInputIs("{\"key\":\"value\"}");
	parseMustSucceed();
	sizeMustBe(1);
	keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSpaceBeforeKey)
{
	whenInputIs("{ \"key\":\"value\"}");
	parseMustSucceed();
	sizeMustBe(1);
	keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSpaceAfterKey)
{
	whenInputIs("{\"key\" :\"value\"}");
	parseMustSucceed();
	sizeMustBe(1);
	keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSpaceBeforeValue)
{
	whenInputIs("{\"key\": \"value\"}");
	parseMustSucceed();
	sizeMustBe(1);
	keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSpaceAfterValue)
{
	whenInputIs("{\"key\":\"value\" }");
	parseMustSucceed();
	sizeMustBe(1);
	keyMustHaveValue("key", "value");
}