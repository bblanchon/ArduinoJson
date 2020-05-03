# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(ElementProxyTests
	add.cpp
	clear.cpp
	compare.cpp
	remove.cpp
	set.cpp
	size.cpp
)

add_test(ElementProxy ElementProxyTests)
