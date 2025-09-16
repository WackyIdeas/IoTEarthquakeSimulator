#ifndef CLIENT_H
#define CLIENT_H

#include "shared.h"

/*
 * Class that wraps around Paho's MQTT asynchronous client.
 */
class Client
{
public:
    Client(std::string name, std::string addr);
    ~Client();

    bool connectClient();
    bool disconnectClient();

    // Status messages need to be retained by the broker, so the option is exposed
    // Assume QoS = 1, probably okay for the purposes of this project
    bool publish(std::string topic, std::string payload, int qos = 1, bool retain = false);

    /*
     * Subscribing is asynchronous here, meaning that the main thread continues to run after
     * the client has subscribed to one or more topics. As a result, something needs to keep
     * the main thread blocked while the client awaits incoming messages. This is usually done
     * by prompting the user for keyboard input on the main thread.
     */
    bool subscribe(std::string topic, int qos = 1);

    /*
     * One client can only have one callback method. The callback method provides a message which contains:
     * 1. The topic via get_topic()
     * 2. The message payload itself with get_payload()
     *
     * Other information which could be relevant:
     * - Quality of service via get_qos()
     * - Retained flag via is_retained()
     * - Duplicate message via is_duplicate()
     *
     * The argument of this method is of type mqtt:callback, but it's expected to provide a subclass
     * with a reimplemented virtual method message_arrived instead. In some cases, the subclass
     * should also keep a reference of the parent client so that the client can publish messages to
     * other topics in response to incoming messages. See controller.cpp for details.
     *
     * https://eclipse.dev/paho/files/cppdoc/classmqtt_1_1message.html
     * https://eclipse.dev/paho/files/cppdoc/classmqtt_1_1callback.html
     *
     */
    void setCallback(mqtt::callback &cb);
    std::string name() const;
    std::string address() const;
    bool connected() const;

private:
    mqtt::async_client *m_client;
    std::string m_name;
    std::string m_addr;
    bool m_connected = false;
};

#endif // CLIENT_H
