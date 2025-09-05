#include "../shared.h"

class callback : public virtual mqtt::callback
{
    void message_arrived(mqtt::const_message_ptr msg) override
    {
        //TODO: Display system status
        std::cout << "Message received: " << msg->get_payload_str() << std::endl;
    }
};

int main()
{
    Client client("sub_status");
    if(!client.connectClient())
    {
        return 1;
    }

    callback cb;
    client.setCallback(cb);

    client.subscribe(STATUS_TOPIC);

    log(client.name(), "Monitoring status, press Return to exit.");
    std::cin.get();

    client.disconnectClient();
    return 0;
}
