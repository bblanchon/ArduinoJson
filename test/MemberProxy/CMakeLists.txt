# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(MemberProxyTests
	add.cpp
	subscript.cpp
	set.cpp
)

target_link_libraries(MemberProxyTests catch)
add_test(MemberProxy MemberProxyTests)
