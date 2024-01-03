# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

add_executable(ResourceManagerTests
	allocVariant.cpp
	clear.cpp
	saveString.cpp
	shrinkToFit.cpp
	size.cpp
	StringBuilder.cpp
	swap.cpp
)

add_compile_definitions(ResourceManagerTests
	ARDUINOJSON_SLOT_ID_SIZE=1 # require less RAM for overflow tests
	ARDUINOJSON_POOL_CAPACITY=16
)

add_test(ResourceManager ResourceManagerTests)

set_tests_properties(ResourceManager
	PROPERTIES
		LABELS "Catch"
)
