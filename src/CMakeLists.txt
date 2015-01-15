file(GLOB_RECURSE INC_FILES ../include/*.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_definitions(
		-fno-exceptions
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
		-Wno-parentheses
		-Wno-sign-conversion
		-Wno-unused
		-Wno-variadic-macros
		-Wnon-virtual-dtor
		-Wold-style-cast
		-Woverloaded-virtual
		-Wredundant-decls
		-Wshadow
		-Wsign-promo
		-Wstrict-overflow=5
		-Wundef
	)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU)")
	add_definitions(
		-Wlogical-op
		-Wnoexcept
		-Wstrict-null-sentinel
	)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_definitions(
		-Wc++11-compat
	)
endif()

add_library(ArduinoJson ${SRC_FILES} ${INC_FILES})