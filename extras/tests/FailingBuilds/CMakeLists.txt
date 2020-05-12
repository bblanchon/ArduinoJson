# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

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
			WILL_FAIL TRUE
	)
endmacro()


add_executable(Issue978 Issue978.cpp)
build_should_fail(Issue978)

add_executable(Issue1189 Issue1189.cpp)
build_should_fail(Issue1189)

add_executable(read_long_long read_long_long.cpp)
set_property(TARGET read_long_long PROPERTY CXX_STANDARD 11)
build_should_fail(read_long_long)

add_executable(write_long_long write_long_long.cpp)
set_property(TARGET write_long_long PROPERTY CXX_STANDARD 11)
build_should_fail(write_long_long)
