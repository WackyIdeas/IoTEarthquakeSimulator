#ifndef CLIENT_H
#define CLIENT_H

#include "shared.h"

class Client
{
public:
    Client(std::string name);
    ~Client();

    bool connectClient();
    bool disconnectClient();
    // Assume QoS = 1
    bool publish(std::string topic, std::string payload, int qos = 1, bool retain = false);
    bool subscribe(std::string topic, int qos = 1);

    void setCallback(mqtt::callback &cb);
    std::string name() const;
    bool connected() const;

private:
    mqtt::async_client *m_client;
    std::string m_name;
    bool m_connected = false;
};

#endif // CLIENT_H
