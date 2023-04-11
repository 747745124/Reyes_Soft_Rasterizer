cmake_minimum_required(VERSION 3.13) # CMake version check
project(TESTS)
set(CMAKE_CXX_STANDARD 17) # Enable c++17 standard

include_directories(../base)

# used for image reading and writing
find_package(OpenCV REQUIRED)
set(BASE framebuffer_test.cpp)

add_executable(TESTS ${BASE})
target_link_libraries(TESTS ${OpenCV_LIBRARIES})