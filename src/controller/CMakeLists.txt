add_executable(IoTController ../shared.cpp ../client.cpp main.cpp)
target_link_libraries(IoTController
    PahoMqttCpp::paho-mqtt3a
    PahoMqttCpp::paho-mqttpp3-shared
    lssdpcpp)

include(GNUInstallDirs)
install(TARGETS IoTStatus
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
