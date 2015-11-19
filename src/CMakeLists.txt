file(GLOB_RECURSE HPP_FILES ../include/*.hpp)
file(GLOB_RECURSE IPP_FILES ../include/*.ipp)
file(GLOB_RECURSE CPP_FILES *.cpp)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_definitions(
		-fno-exceptions
		-fno-rtti
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
		-Wdeprecated-register
	)
endif()

if(MSVC)
	add_definitions(
		-D_CRT_SECURE_NO_WARNINGS
		-W4)
endif()

add_library(ArduinoJson ${CPP_FILES} ${HPP_FILES} ${IPP_FILES})

target_include_directories(ArduinoJson INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
