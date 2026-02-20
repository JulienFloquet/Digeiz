cmake_minimum_required(VERSION 3.16)
project(CorruptedVideo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Chemin OpenCV MinGW
set(OpenCV_DIR "C:/opencv_mingw")

find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(CorruptedVideo
    src/CorruptedVideo.cpp
    src/ImageMetrics.cpp
    src/Utilities.cpp
    src/VideoReconstruction.cpp
)

target_include_directories(CorruptedVideo PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(CorruptedVideo
    ${OpenCV_LIBS}
)
