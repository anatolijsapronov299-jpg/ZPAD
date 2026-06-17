cmake_minimum_required(VERSION 3.10)

project(Lab7OpenCVDNNThreads)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    include
)

add_executable(lab7_app
    src/main.cpp
    src/CameraProvider.cpp
    src/Display.cpp
    src/FaceDetector.cpp
    src/FrameProcessor.cpp
    src/KeyProcessor.cpp
)

target_link_libraries(lab7_app ${OpenCV_LIBS} pthread)
