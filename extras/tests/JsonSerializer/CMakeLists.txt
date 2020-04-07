# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
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

target_link_libraries(JsonSerializerTests
	ArduinoJson
	catch
)

add_test(JsonSerializer JsonSerializerTests)
