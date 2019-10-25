# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

add_subdirectory(catch)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(
		-pedantic
		-Wall
		-Wcast-align
		-Wcast-qual
		-Wconversion
		-Wctor-dtor-privacy
		-Wdisabled-optimization
		-Werror
		-Wextra
		-Wformat=2
		-Winit-self
		-Wmissing-include-dirs
		-Wnon-virtual-dtor
		-Wold-style-cast
		-Woverloaded-virtual
		-Wparentheses
		-Wredundant-decls
		-Wshadow
		-Wsign-promo
		-Wstrict-aliasing
		-Wundef
	)

	if(NOT MINGW)
		add_compile_options(
			-std=c++98
		)
	endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options(
		-Wstrict-null-sentinel
		-Wno-vla # Allow VLA in tests
	)
	add_definitions(-DHAS_VARIABLE_LENGTH_ARRAY)

	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.5)
		add_compile_options(-Wlogical-op) # the flag exists in 4.4 but is buggy
	endif()

	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.6)
		add_compile_options(-Wnoexcept)
	endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(
		-Wc++11-compat
		-Wdeprecated-register
		-Wno-vla-extension # Allow VLA in tests
	)
	add_definitions(
		-DHAS_VARIABLE_LENGTH_ARRAY
		-DSUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR
	)
endif()

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_compile_options(
		/W4 # Set warning level
		/WX # Treats all compiler warnings as errors.
	)
endif()

include_directories(Helpers)
add_subdirectory(ElementProxy)
add_subdirectory(IntegrationTests)
add_subdirectory(JsonArray)
add_subdirectory(JsonDeserializer)
add_subdirectory(JsonDocument)
add_subdirectory(JsonObject)
add_subdirectory(JsonSerializer)
add_subdirectory(JsonVariant)
add_subdirectory(MemberProxy)
add_subdirectory(MemoryPool)
add_subdirectory(Misc)
add_subdirectory(MixedConfiguration)
add_subdirectory(MsgPackDeserializer)
add_subdirectory(MsgPackSerializer)
add_subdirectory(Numbers)
add_subdirectory(TextFormatter)
