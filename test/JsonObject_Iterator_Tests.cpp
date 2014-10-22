#include <gtest/gtest.h>
#include <ArduinoJson/JsonObject.hpp>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

TEST(JsonObject_Iterator_Test, SimpleTest)
{
	StaticJsonBuffer<42> jsonBuffer;

	JsonObject object = jsonBuffer.createObject();
	object["ab"] = 12;
	object["cd"] = 34;

	JsonObjectIterator it = object.begin();
	JsonObjectIterator end = object.end();

	EXPECT_NE(end, it);
	EXPECT_STREQ("ab", it.key());
	EXPECT_EQ(12, static_cast<int>(it.value()));
	++it;
	EXPECT_NE(end, it);
	EXPECT_STREQ("cd", it.key());
	EXPECT_EQ(34, static_cast<int>(it.value()));
	++it;
	EXPECT_EQ(object.end(), it);
}