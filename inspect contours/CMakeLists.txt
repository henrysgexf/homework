cmake_minimum_required(VERSION 3.20)

project(contours)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(findcontours findcontours.cpp)

target_link_libraries(findcontours ${OpenCV_LIBS})