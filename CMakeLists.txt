cmake_minimum_required (VERSION 2.8.12)
project (ConfigIPCAM)
find_package(OpenCV REQUIRED)

include_directories(include)
add_definitions("-std=c++11")


add_executable(twoCam src/twoCam.cpp)
target_link_libraries(config ${OpenCV_LIBS} curl pthread stdc++fs)

add_executable(oneCam src/oneCam.cpp)
target_link_libraries(oneCam ${OpenCV_LIBS} curl pthread stdc++fs)


