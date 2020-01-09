# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

if(MSVC)
	add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(msgpack_fuzzer
	msgpack_fuzzer.cpp
	fuzzer_main.cpp
)

add_executable(json_fuzzer
	json_fuzzer.cpp
	fuzzer_main.cpp
)
