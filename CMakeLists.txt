cmake_minimum_required(VERSION 3.16)

project(IoTEarthquakeSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JSON_INCLUDE "json/include/")
include_directories(${JSON_INCLUDE})
find_package(PahoMqttCpp)

add_subdirectory(src)
