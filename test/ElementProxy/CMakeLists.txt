# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(ElementProxyTests
	add.cpp
	set.cpp
)

target_link_libraries(ElementProxyTests catch)
add_test(ElementProxy ElementProxyTests)
