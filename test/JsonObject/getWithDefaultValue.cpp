// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::getWithDefault()") {
	const char *json = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.75608,2.302038]}";
	DynamicJsonBuffer jb;
	JsonObject &jsonObject = jb.parseObject(json);

	REQUIRE(jsonObject.get("sensorNon", "GPS") == "GPS");

	std::string timeNon = "timeNon";
	REQUIRE(jsonObject.get(timeNon, 10000000) == 10000000);

	JsonArray &data = jsonObject["data"];
	REQUIRE(data.get(10, -1) == -1);
}
