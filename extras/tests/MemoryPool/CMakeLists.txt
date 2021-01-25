# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

add_executable(MemoryPoolTests 
	allocVariant.cpp
	clear.cpp
	saveString.cpp
	size.cpp
	StringCopier.cpp
)

add_test(MemoryPool MemoryPoolTests)
