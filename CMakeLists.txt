cmake_minimum_required (VERSION 3.1)
project (BottomFlow VERSION 1.0 LANGUAGES CXX)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3")
include_directories(".")

add_subdirectory(Vizualization)
add_subdirectory(InfoAcquisition)
add_subdirectory(indicators)
include_directories("InfoAcquisition" "Vizualization" "indicators")


add_executable(bflow main.cpp)
target_link_libraries(bflow Vizualization InfoAcquisition)


