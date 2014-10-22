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

	template<typename T>
	void keyMustHaveValue(const char* key, T expected)
	{
		EXPECT_EQ(expected, static_cast<T>(_object[key]));
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

TEST_F(JsonParser_Object_Test, MissingColonAndValue)
{
	whenInputIs("{\"key\"}");
	parseMustFail();
	sizeMustBe(0);
}

TEST_F(JsonParser_Object_Test, MissingQuotesAndColonAndValue)
{
	whenInputIs("{key}");
	parseMustFail();
	sizeMustBe(0);
}

TEST_F(JsonParser_Object_Test, OneString)
{
	whenInputIs("{\"key\":\"value\"}");
	parseMustSucceed();
	sizeMustBe(1);
	keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSingleQuotes)
{
    whenInputIs("{'key':'value'}");
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

TEST_F(JsonParser_Object_Test, TwoStrings)
{
    whenInputIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
    parseMustSucceed();
    sizeMustBe(2);
    keyMustHaveValue("key1", "value1");
    keyMustHaveValue("key2", "value2");
}

TEST_F(JsonParser_Object_Test, TwoStringsSpaceBeforeComma)
{
    whenInputIs("{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
    parseMustSucceed();
    sizeMustBe(2);
    keyMustHaveValue("key1", "value1");
    keyMustHaveValue("key2", "value2");
}

TEST_F(JsonParser_Object_Test, TwoStringsSpaceAfterComma)
{
    whenInputIs("{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
    parseMustSucceed();
    sizeMustBe(2);
    keyMustHaveValue("key1", "value1");
    keyMustHaveValue("key2", "value2");
}

TEST_F(JsonParser_Object_Test, EndingWithAComma)
{
    whenInputIs("{\"key1\":\"value1\",}");
    parseMustFail();
    sizeMustBe(0);
}

TEST_F(JsonParser_Object_Test, TwoIntergers)
{
    whenInputIs("{\"key1\":42,\"key2\":-42}");
    parseMustSucceed();
    sizeMustBe(2);
    keyMustHaveValue("key1", 42);
    keyMustHaveValue("key2", -42);
}

TEST_F(JsonParser_Object_Test, TwoDoubles)
{
    whenInputIs("{\"key1\":12.345,\"key2\":-7.89}");
    parseMustSucceed();
    sizeMustBe(2);
    keyMustHaveValue("key1", 12.345);
    keyMustHaveValue("key2", -7.89);
}