cmake_minimum_required(VERSION 3.16)
project(libmbot)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_NAME mbot)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(ament_cmake REQUIRED)
find_package(serial REQUIRED)
find_package(Threads REQUIRED)

# Declare cpp library
add_executable(${LIBRARY_NAME}
  src/main.cpp
  src/operations.cpp
)

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
ament_export_dependencies(
  serial
)

ament_package()
