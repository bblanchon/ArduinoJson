file(GLOB_RECURSE INC_FILES ../include/*.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp)

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(
		-Wall
		-Wcast-align
		-Wcast-qual
		-Wctor-dtor-privacy
		-Wdisabled-optimization
		-Wextra
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