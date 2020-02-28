cmake_minimum_required(VERSION 3.15)
project(FYP)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)
set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

set(INC_DIR ~/Documents/sdsl-lite/include)
set(LINK_DIR ~/Documents/sdsl-lite/lib)
include_directories(${INC_DIR})
include_directories(${LINK_DIR})
link_libraries(sdsl-lite)
target_link_libraries(${PROJECT_NAME} "/usr/local/lib/libsdsl.a" "/usr/local/lib/libdivsufsort.a" "/usr/local/lib/libdivsufsort64.a")
