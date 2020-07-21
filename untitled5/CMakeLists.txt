cmake_minimum_required(VERSION 3.16)
project(untitled5)

set(CMAKE_CXX_STANDARD 14)

#find_library(OpenCV)
find_package(OpenCV)
set(OpenCV_INCLUDE_DIRS "opencv2")
include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 14)


add_executable(untitled5 main.cpp Thecompare/Thecompare.cpp Theimageclass/Theimage.cpp)

target_link_libraries(untitled5 "${OpenCV_LIBS}")