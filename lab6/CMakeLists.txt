cmake_minimum_required(VERSION 3.10)

project(Lab6OpenCV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    include
)

add_executable(lab6_app
    src/main.cpp
    src/CameraProvider.cpp
    src/FrameProcessor.cpp
    src/KeyProcessor.cpp
    src/Display.cpp
)

target_link_libraries(lab6_app ${OpenCV_LIBS})
