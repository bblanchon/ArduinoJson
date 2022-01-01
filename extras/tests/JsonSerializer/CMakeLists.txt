# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2022, Benoit BLANCHON
# MIT License

add_executable(JsonSerializerTests
	CustomWriter.cpp
	JsonArray.cpp
	JsonArrayPretty.cpp
	JsonObject.cpp
	JsonObjectPretty.cpp
	JsonVariant.cpp
	misc.cpp
	std_stream.cpp
	std_string.cpp
)

add_test(JsonSerializer JsonSerializerTests)

set_tests_properties(JsonSerializer
	PROPERTIES
		LABELS 		"Catch"
)
