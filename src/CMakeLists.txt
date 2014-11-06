file(GLOB_RECURSE INC_FILES ../include/*.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp)

message(">    ${CMAKE_CXX_COMPILER}")
message(">    ${CMAKE_CXX_COMPILER_ID}")

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_definitions(
		-Wall
		-Wcast-align
		-Wcast-qual
		-Wctor-dtor-privacy
		-Wdisabled-optimization
		-Werrors
		-Wextra
		-Wfatal-errors
		-Wformat=2
		-Winit-self
		-Wlogical-op
		-Wmissing-include-dirs
		-Wno-parentheses
		-Wno-unused
		-Wno-variadic-macros
		-Wnoexcept
		-Wold-style-cast
		-Woverloaded-virtual
		-Wpedantic
		-Wredundant-decls
		-Wshadow
		-Wsign-promo
		-Wstrict-null-sentinel
		-Wstrict-overflow=5
		-Wundef
	)
endif()

add_library(ArduinoJson ${SRC_FILES} ${INC_FILES})