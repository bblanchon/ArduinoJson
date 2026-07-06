# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2026, Benoit BLANCHON
# MIT License

add_executable(MiscTests
	arithmeticCompare.cpp
	conflicts.cpp
	issue1967.cpp
	issue2129.cpp
	issue2166.cpp
	JsonString.cpp
	NoArduinoHeader.cpp
	printable.cpp
	Readers.cpp
	StringAdapters.cpp
	StringWriter.cpp
	TypeTraits.cpp
	unsigned_char.cpp
	Utf16.cpp
	Utf8.cpp
	version.cpp
)

set_target_properties(MiscTests PROPERTIES UNITY_BUILD OFF)

add_test(Misc MiscTests)

set_tests_properties(Misc
	PROPERTIES
		LABELS "Catch"
)

add_executable(Issue2181
	issue2181.cpp # Cannot be linked with other tests
)

set_target_properties(Issue2181 PROPERTIES UNITY_BUILD OFF)

add_test(Issue2181 Issue2181)

set_tests_properties(Issue2181
	PROPERTIES
		LABELS "Catch"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(Issue2181
		PRIVATE
			-Wno-keyword-macro # keyword is hidden by macro definition
	)
endif()
