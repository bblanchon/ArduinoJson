# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(MiscTests
	conflicts.cpp
	FloatParts.cpp
	Readers.cpp
	StringAdapters.cpp
	StringWriter.cpp
	TypeTraits.cpp
	unsigned_char.cpp
	Utf8.cpp
	Utf16.cpp
	version.cpp
)

target_link_libraries(MiscTests catch)
set_target_properties(MiscTests PROPERTIES UNITY_BUILD OFF)

add_test(Misc MiscTests)


add_executable(Issue978
	Issue978.cpp
)
set_target_properties(Issue978
	PROPERTIES
    	EXCLUDE_FROM_ALL TRUE
        EXCLUDE_FROM_DEFAULT_BUILD TRUE
)
add_test(
	NAME
		Issue978
    COMMAND
    	${CMAKE_COMMAND} --build . --target Issue978 --config $<CONFIGURATION>
    WORKING_DIRECTORY
    	${CMAKE_BINARY_DIR}
)
set_tests_properties(Issue978 
	PROPERTIES
		WILL_FAIL TRUE)
