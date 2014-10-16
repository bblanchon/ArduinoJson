file(GLOB_RECURSE INC_FILES ../include/*.h)
file(GLOB_RECURSE SRC_FILES *.cpp)

include_directories(../include)

add_library(ArduinoJson ${SRC_FILES} ${INC_FILES})