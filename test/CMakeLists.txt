set(GTEST_DIR ../third-party/gtest-1.7.0)

file(GLOB TESTS_FILES *.hpp *.cpp)

include_directories(
    ${GTEST_DIR}
    ${GTEST_DIR}/include)

add_definitions(-DGTEST_HAS_PTHREAD=0)

# Workaround for Visual Studio 2012
if (MSVC AND MSVC_VERSION EQUAL 1700)
  add_definitions(-D_VARIADIC_MAX=10)
endif()

add_executable(ArduinoJsonTests 
    ${TESTS_FILES}
    ${GTEST_DIR}/src/gtest-all.cc
    ${GTEST_DIR}/src/gtest_main.cc)

target_link_libraries(ArduinoJsonTests ArduinoJson)

add_test(ArduinoJsonTests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ArduinoJsonTests)
