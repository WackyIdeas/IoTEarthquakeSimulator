
#include "../shared.h"

class callback : public virtual mqtt::callback
{
    Client *parent;
    void message_arrived(mqtt::const_message_ptr msg) override
    {
        // TODO: publish sensor data to status and actuator nodes
        // TODO: Figure out conditions to trigger alarm
        std::cout << "Message received: " << msg->get_payload_str() << std::endl;
    }
public:
    void setParent(Client* c)
    {
        parent = c;
    }
};

int main()
{
    Client client("controller");
    if(!client.connectClient())
    {
        return 1;
    }

    callback cb;
    client.setCallback(cb);

    client.subscribe(ACCEL_TOPIC);
    client.subscribe(ULTRA_TOPIC);

    log(client.name(), "Monitoring sensors, press Return to exit.");
    std::cin.get();

    client.disconnectClient();
    return 0;
}
