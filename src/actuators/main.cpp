#include "../shared.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class callback : public virtual mqtt::callback
{
    std::string dataset = "../data/actuator_state.json";
    void message_arrived(mqtt::const_message_ptr msg) override
    {
        //TODO write actuator data to JSON file data/actuator_state.json
        //std::cout << "[ALARM] LED ON" << std::endl;
        //std::cout << "[ALARM] LED OFF" << std::endl;
        std::cout << "Message received: " << msg->get_payload_str() << std::endl;
    }
};

int main()
{
    Client client("sub_led");

    if(!client.connectClient())
    {
        return 1;
    }

    callback cb;
    client.setCallback(cb);

    client.subscribe(LED_TOPIC);

    log(client.name(), "Monitoring status, press Return to exit.");
    std::cin.get();

    client.disconnectClient();
    return 0;
}
