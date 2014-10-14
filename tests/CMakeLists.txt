set(GTEST_DIR ../third-party/gtest-1.7.0)

file(GLOB TESTS_FILES *.cpp)

include_directories(
	../srcs
    ${GTEST_DIR}
    ${GTEST_DIR}/include)

add_executable(ArduinoJsonTests 
    ${TESTS_FILES}
    ${GTEST_DIR}/src/gtest-all.cc
    ${GTEST_DIR}/src/gtest_main.cc)

target_link_libraries(ArduinoJsonTests ArduinoJson)

add_test(ArduinoJsonTests ArduinoJsonTests)