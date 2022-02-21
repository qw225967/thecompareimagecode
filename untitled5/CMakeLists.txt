cmake_minimum_required(VERSION 3.16)
project(untitled5)

set(CMAKE_CXX_STANDARD 14)
set(OpenCV_DIRS /opt/homebrew/Cellar/opencv/4.5.4_3)
set(OpenCV_LIBS ${OpenCV_DIRS}/lib)

message(STATUS ${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

include_directories(
        ${OpenCV_DIRS}/include/opencv4
)
link_directories(
        ${OpenCV_LIBS}
)

add_executable(untitled5 main.cpp Thecompare/Thecompare.cpp Theimageclass/Theimage.cpp)
target_link_libraries(
        untitled5
        opencv_core
        opencv_imgproc
        opencv_imgcodecs
#        opencv_video
#        opencv_highgui
#        opencv_photo
#        opencv_flann
#        opencv_superres
#        opencv_stitching
#        opencv_ml
#        opencv_video
#        opencv_features2d
#        opencv_calib3d
#        opencv_objdetect
)