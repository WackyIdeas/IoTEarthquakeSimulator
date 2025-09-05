#include "client.h"
Client::Client(std::string name)
{
    m_name = name;
    m_client = new mqtt::async_client(SERVER_ADDR, name);
}
Client::~Client()
{
    if(connected()) disconnectClient();
    delete m_client;
}

bool Client::connectClient()
{
    // Boilerplate for connection establishment
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);
    try
    {
        m_client->connect(connOpts)->wait();
        log(m_name, "Connected to broker successfully");
        m_connected = true;
    }
    catch(mqtt::exception &e) // Gracefully handle exceptions
    {
        log(m_name, e.what(), true);
        return false;
    }
    return true;

}
bool Client::disconnectClient()
{
    try
    {
        m_client->disconnect()->wait();
        m_connected = false;
        log(m_name, "Disconnected from broker");
    }
    catch(mqtt::exception &e)
    {
        log(m_name, e.what(), true);
        return false;
    }

    return true;
}
bool Client::publish(std::string topic, std::string payload, int qos, bool retain)
{
    try
    {
        mqtt::message_ptr pubmsg = mqtt::make_message(topic, payload, qos, retain);
        m_client->publish(pubmsg)->wait();
        log(m_name, std::string("Message published: ") + payload);
    }
    catch(mqtt::exception &e)
    {
        log(m_name, e.what(), true);
        return false;
    }
    return true;
}
bool Client::subscribe(std::string topic, int qos)
{
    try
    {
        m_client->subscribe(topic, qos)->wait();
        log(m_name, std::string("Connected to topic: " + topic));
    }
    catch(mqtt::exception &e)
    {
        log(m_name, e.what(), true);
        return false;
    }
    return true;

}
void Client::setCallback(mqtt::callback &cb)
{
    m_client->set_callback(cb);
}

bool Client::connected() const
{
    return m_connected;
}

std::string Client::name() const
{
    return m_name;
}
