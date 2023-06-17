# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2023, Benoit BLANCHON
# MIT License

add_executable(ResourceManagerTests
	allocVariant.cpp
	clear.cpp
	saveString.cpp
	size.cpp
	StringBuilder.cpp
)

add_test(ResourceManager ResourceManagerTests)

set_tests_properties(ResourceManager
	PROPERTIES
		LABELS "Catch"
)
