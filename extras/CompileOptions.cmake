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
		-Wsign-conversion
		-Wsign-promo
		-Wstrict-aliasing
		-Wundef
	)

	if(${COVERAGE})
		set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
	endif()

endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL  "GNU")
	if((CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.8) AND (NOT ${COVERAGE}))
		add_compile_options(-g -Og)
	else()
		add_compile_options(-g -O0)
	endif()

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

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	if((CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.0) AND (NOT ${COVERAGE}))
		add_compile_options(-g -Og)
	else()
		add_compile_options(-g -O0)
	endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	if((CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0) AND (NOT ${COVERAGE}))
		add_compile_options(-g -Og)
	else()
		add_compile_options(-g -O0)
	endif()
endif()

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_compile_options(
		/W4 # Set warning level
		/WX # Treats all compiler warnings as errors.
	)

	if (NOT MSVC_VERSION LESS  1910) #  >= Visual Studio 2017
		add_compile_options(
			/Zc:__cplusplus  # Enable updated __cplusplus macro
		)
	endif()
endif()

if(MINGW)
  # Static link on MinGW to avoid linking with the wrong DLLs when multiple
	# versions are installed.
	add_link_options(-static)
endif()
