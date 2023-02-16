# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2023, Benoit BLANCHON
# MIT License

add_executable(MsgPackSerializerTests
	destination_types.cpp
	measure.cpp
	misc.cpp
	serializeArray.cpp
	serializeObject.cpp
	serializeVariant.cpp
)

add_test(MsgPackSerializer MsgPackSerializerTests)

set_tests_properties(MsgPackSerializer
	PROPERTIES
		LABELS "Catch"
)
