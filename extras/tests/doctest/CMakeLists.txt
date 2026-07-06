include(FetchContent)
FetchContent_Declare(
	doctest
	URL https://github.com/doctest/doctest/releases/download/v2.4.12/doctest.h
	DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(doctest)
FetchContent_GetProperties(doctest SOURCE_DIR DOCTEST_SOURCE_DIR)

add_library(doctest main.cpp)
target_include_directories(doctest PUBLIC ${DOCTEST_SOURCE_DIR})
