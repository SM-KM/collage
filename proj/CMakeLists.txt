cmake_minimum_required(VERSION 3.10)
project(STREAMING)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED True)

# Collect source files
file(GLOB UTIL_SOURCES "util/*.cpp")
file(GLOB REQUESTS_SOURCES "requests/*.cpp")

# Explicit source files
set(SOURCES ${UTIL_SOURCES} ${REQUESTS_SOURCES} main.cpp)

# Executable
add_executable(main ${SOURCES})

# Find libraries
find_package(CURL REQUIRED)
target_include_directories(main PRIVATE ${CURL_INCLUDE_DIR})
target_link_libraries(main PRIVATE ${CURL_LIBRARIES})

# Output directory
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")






