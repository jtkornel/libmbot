cmake_minimum_required(VERSION 3.23)
project(libmbot)

set(PACKAGE_VERSION 0.1.0)

option(USE_SANITIZERS "Enable various runtime sanitizers")

if(USE_SANITIZERS)
    set(SANITIZER_FLAGS "-fsanitize=undefined,leak,address -fstack-protector-all -static-libasan")
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)

if(CMAKE_CXX_CPPCHECK)
    list(APPEND CMAKE_CXX_CPPCHECK "--enable=style" "--suppressions-list=${CMAKE_CURRENT_SOURCE_DIR}/suppressions.txt")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Asio REQUIRED)
find_package(Threads REQUIRED)

set(LIBRARY_NAME mbot)

add_library(${LIBRARY_NAME} STATIC
    src/comm.cpp
)

set_property(TARGET ${LIBRARY_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(
    ${LIBRARY_NAME}
    PUBLIC
    FILE_SET public_headers
    TYPE HEADERS
    BASE_DIRS include
    FILES
    include/boards.hpp
    include/comm.hpp
    include/devices.hpp
    include/operations.hpp
)

# Example executables
set(EXAMPLES
    battery_power
    auriga_board
    read_motor_sensors
)

foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} examples/${EXAMPLE}.cpp)

    target_link_libraries(${EXAMPLE}
        ${LIBRARY_NAME}
    )
endforeach()

# Without PUBLIC, we get serious buffer overrun errors
# which needs investigation
target_link_libraries(${LIBRARY_NAME} PUBLIC "$<BUILD_INTERFACE:asio>" ${SANITIZER_FLAGS})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    VERSION "${PACKAGE_VERSION}"
    COMPATIBILITY AnyNewerVersion
)

# generate the config file that includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# Install targets
install(
    TARGETS ${LIBRARY_NAME}
    EXPORT ${PROJECT_NAME}-targets
    FILE_SET
    public_headers
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Install config and config-version
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)
