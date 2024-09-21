# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

add_executable(MiscTests
	arithmeticCompare.cpp
	conflicts.cpp
	issue1967.cpp
	issue2129.cpp
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
