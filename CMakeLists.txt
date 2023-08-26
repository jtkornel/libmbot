cmake_minimum_required(VERSION 3.17)
project(libmbot)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)

if(CMAKE_CXX_CPPCHECK)
    list(APPEND CMAKE_CXX_CPPCHECK "--enable=style")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(ament_cmake REQUIRED)
find_package(serial REQUIRED)

# Library
set(LIBRARY_NAME mbot)

include_directories(
    include
)

add_library(${LIBRARY_NAME} STATIC
    src/comm.cpp
    include/comm.hpp
    include/devices.hpp
    include/operations.hpp
)

# Example executables
set(EXAMPLES
    battery_power
)

foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} examples/${EXAMPLE}.cpp)

    target_link_libraries(${EXAMPLE}
        ${LIBRARY_NAME}
    )
endforeach()

ament_target_dependencies(${LIBRARY_NAME}
    serial
)

install(
    TARGETS
    ${LIBRARY_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

ament_export_libraries(
    libmbot
)
ament_export_include_directories(
    include
)

ament_export_dependencies(
    serial
)

ament_package()
