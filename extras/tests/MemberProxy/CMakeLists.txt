# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(MemberProxyTests
	add.cpp
	clear.cpp
	compare.cpp
	containsKey.cpp
	remove.cpp
	set.cpp
	size.cpp
	subscript.cpp
)

target_link_libraries(MemberProxyTests
	ArduinoJson
	catch
)

add_test(MemberProxy MemberProxyTests)
