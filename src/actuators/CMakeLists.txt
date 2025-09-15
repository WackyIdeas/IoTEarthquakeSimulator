add_executable(IoTActuator ../shared.cpp ../client.cpp ../service.cpp main.cpp)
target_link_libraries(IoTActuator
    PahoMqttCpp::paho-mqtt3a
    PahoMqttCpp::paho-mqttpp3-shared
    lssdpcpp)

include(GNUInstallDirs)
install(TARGETS IoTActuator
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
