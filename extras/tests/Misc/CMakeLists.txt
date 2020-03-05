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

macro(build_should_fail target)
	set_target_properties(${target}
		PROPERTIES
	    	EXCLUDE_FROM_ALL TRUE
	        EXCLUDE_FROM_DEFAULT_BUILD TRUE
	)
	add_test(
		NAME
			${target}
	    COMMAND
	    	${CMAKE_COMMAND} --build . --target ${target} --config $<CONFIGURATION>
	    WORKING_DIRECTORY
	    	${CMAKE_BINARY_DIR}
	)
	set_tests_properties(${target} 



		PROPERTIES
			WILL_FAIL TRUE)
endmacro()


add_executable(Issue978
	Issue978.cpp
)
build_should_fail(Issue978)

add_executable(Issue1189
	Issue1189.cpp
)
build_should_fail(Issue1189)
