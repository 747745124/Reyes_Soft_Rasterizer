cmake_minimum_required(VERSION 3.13) # CMake version check
project(TESTS)
set(CMAKE_CXX_STANDARD 17) # Enable c++17 standard
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE STRING "C++ compiler" FORCE)
#set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE STRING "C compiler" FORCE)
include_directories(./base)

# used for image reading and writing
find_package(OpenCV REQUIRED)
set(BASE ./base/object3D.cpp ./base/camera.cpp ./base/framebuffer.cpp ./base/primitive.cpp ./base/texture.cpp ./base/light.cpp)

# Add main.cpp file of project root directory as source files
set(SOURCE_FILES ./all_tests.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(TESTS ${SOURCE_FILES} ${BASE})
target_link_libraries(TESTS ${OpenCV_LIBRARIES})