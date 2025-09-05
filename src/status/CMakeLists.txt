add_executable(IoTStatus ../shared.cpp ../client.cpp main.cpp)
target_link_libraries(IoTStatus
    PahoMqttCpp::paho-mqtt3a
    PahoMqttCpp::paho-mqttpp3-shared)

include(GNUInstallDirs)
install(TARGETS IoTStatus
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
