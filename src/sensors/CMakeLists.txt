add_executable(IoTUltrasonic ../shared.cpp ../client.cpp sensor.cpp main_ultrasonic.cpp)
add_executable(IoTAccelerometer ../shared.cpp ../client.cpp sensor.cpp main_accelerometer.cpp)

target_link_libraries(IoTUltrasonic
    PahoMqttCpp::paho-mqtt3a
    PahoMqttCpp::paho-mqttpp3-shared
    lssdpcpp)
target_link_libraries(IoTAccelerometer
    PahoMqttCpp::paho-mqtt3a
    PahoMqttCpp::paho-mqttpp3-shared)

include(GNUInstallDirs)
install(TARGETS IoTUltrasonic
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS IoTAccelerometer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
