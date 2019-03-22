# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

add_executable(MemberProxyTests
	add.cpp
	clear.cpp
	containsKey.cpp
	remove.cpp
	set.cpp
	size.cpp
	subscript.cpp
)

target_link_libraries(MemberProxyTests catch)
add_test(MemberProxy MemberProxyTests)
