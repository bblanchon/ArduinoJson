# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

# I have no idea what this is about, I simply followed the instructions from:
# https://dominikberner.ch/cmake-interface-lib/

add_library(ArduinoJson INTERFACE)

include(GNUInstallDirs)

# Adding the install interface generator expression makes sure that the include
# files are installed to the proper location (provided by GNUInstallDirs)
target_include_directories(ArduinoJson
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# locations are provided by GNUInstallDirs
install(
    TARGETS
        ArduinoJson
    EXPORT
        ArduinoJson_Targets
    ARCHIVE DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_BINDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/ArduinoJsonConfigVersion.cmake"
    VERSION
        6.15.0
    COMPATIBILITY
        SameMajorVersion
)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/extras/ArduinoJsonConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/ArduinoJsonConfig.cmake"
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/ArduinoJson/cmake)

install(
    EXPORT
        ArduinoJson_Targets
    FILE
        ArduinoJsonTargets.cmake
    DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/ArduinoJson/cmake
)

install(
    FILES
        "${PROJECT_BINARY_DIR}/ArduinoJsonConfig.cmake"
        "${PROJECT_BINARY_DIR}/ArduinoJsonConfigVersion.cmake"
    DESTINATION
        "${CMAKE_INSTALL_DATAROOTDIR}/ArduinoJson/cmake"
)

install(
    FILES
        ArduinoJson.h
        ArduinoJson.hpp
    DESTINATION
        include
)

install(
    DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/ArduinoJson"
    DESTINATION
        include
)
