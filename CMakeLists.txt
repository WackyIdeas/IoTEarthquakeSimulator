cmake_minimum_required(VERSION 3.16)

project(IoTEarthquakeSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JSON_INCLUDE "json/include/")
include_directories(${JSON_INCLUDE})
find_package(PahoMqttCpp)

add_executable(IoTSubscriber subscriber.cpp)
add_executable(IoTPublisher publisher.cpp)


target_link_libraries(IoTSubscriber
    PahoMqttCpp::paho-mqtt3a
    PahoMqttCpp::paho-mqttpp3-shared)
target_link_libraries(IoTPublisher
    PahoMqttCpp::paho-mqtt3a
    PahoMqttCpp::paho-mqttpp3-shared)

include(GNUInstallDirs)
install(TARGETS IoTSubscriber
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS IoTPublisher
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
