# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

add_executable(TextFormatterTests
	writeFloat.cpp
	writeInteger.cpp
	writeString.cpp
)

set_target_properties(TextFormatterTests PROPERTIES UNITY_BUILD OFF)

add_test(TextFormatter TextFormatterTests)

set_tests_properties(TextFormatter
	PROPERTIES
		LABELS "Catch"
)
