# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

# we need C++11 for 'long long'
set(CMAKE_CXX_STANDARD 11)

add_executable(MixedConfigurationTests
	decode_unicode_0.cpp
	decode_unicode_1.cpp
	use_double_0.cpp
	use_double_1.cpp
	use_long_long_0.cpp
	use_long_long_1.cpp
)

target_link_libraries(MixedConfigurationTests catch)
add_test(MixedConfiguration MixedConfigurationTests)
