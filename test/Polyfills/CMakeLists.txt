# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(PolyfillsTests 
	isFloat.cpp
	isInteger.cpp
	parseFloat.cpp
	parseInteger.cpp
)

target_link_libraries(PolyfillsTests catch)
add_test(Polyfills PolyfillsTests)
