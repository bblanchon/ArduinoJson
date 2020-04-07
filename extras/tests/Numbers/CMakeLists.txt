# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(NumbersTests 
	parseFloat.cpp
	parseInteger.cpp
	parseNumber.cpp
)

target_link_libraries(NumbersTests
	ArduinoJson
	catch
)


add_test(Numbers NumbersTests)
