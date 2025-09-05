#include "../shared.h"

class callback : public virtual mqtt::callback
{
    // Keep a reference of the parent client
    // Needs to call setParent() before any new messages arrive
    Client *parent;
    void message_arrived(mqtt::const_message_ptr msg) override
    {
        // TODO: publish sensor data to status and actuator nodes
        // TODO: Figure out conditions to trigger alarm
        std::cout << "Message received: " << msg->get_payload_str() << std::endl;
        // cool
        std::string test = "{ \"timestamp\": \"2025-12-20 05:20\", \"state\": \"Critical\" }";
        if(parent)
        {
            log(parent->name(), "Publishing test to LED alarm");
            parent->publish(LED_TOPIC, test, 1, true);
        }

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
    cb.setParent(&client);
    client.setCallback(cb);

    client.subscribe(ACCEL_TOPIC);
    client.subscribe(ULTRA_TOPIC);

    log(client.name(), "Monitoring sensors, press Return to exit.");
    std::cin.get();

    client.disconnectClient();
    return 0;
}
