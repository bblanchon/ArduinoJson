# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

macro(add_failing_build source_file)
	get_filename_component(target ${source_file} NAME_WE)

	add_executable(${target} ${source_file})

	set_target_properties(${target}
		PROPERTIES
			EXCLUDE_FROM_ALL TRUE
			EXCLUDE_FROM_DEFAULT_BUILD TRUE
	)
	add_test(
		NAME ${target}
			COMMAND ${CMAKE_COMMAND} --build . --target ${target} --config $<CONFIGURATION>
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	set_tests_properties(${target}
		PROPERTIES
			WILL_FAIL TRUE
			LABELS "WillFail"
	)
endmacro()

add_failing_build(Issue978.cpp)
add_failing_build(read_long_long.cpp)
add_failing_build(write_long_long.cpp)
add_failing_build(variant_as_char.cpp)
add_failing_build(assign_char.cpp)
add_failing_build(deserialize_object.cpp)
