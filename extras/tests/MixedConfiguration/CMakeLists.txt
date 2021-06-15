# ArduinoJson - https://arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

add_executable(MixedConfigurationTests
	decode_unicode_0.cpp
	decode_unicode_1.cpp
	enable_alignment_0.cpp
	enable_alignment_1.cpp
	enable_comments_0.cpp
	enable_comments_1.cpp
	enable_infinity_0.cpp
	enable_infinity_1.cpp
	enable_nan_0.cpp
	enable_nan_1.cpp
	enable_progmem_1.cpp
	enable_string_deduplication_0.cpp
	enable_string_deduplication_1.cpp
	use_double_0.cpp
	use_double_1.cpp
)

set_target_properties(MixedConfigurationTests PROPERTIES UNITY_BUILD OFF)

add_test(MixedConfiguration MixedConfigurationTests)

set_tests_properties(MixedConfiguration
	PROPERTIES
		LABELS 		"Catch"
)
