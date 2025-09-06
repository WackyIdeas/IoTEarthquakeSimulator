#include "../shared.h"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

const std::string clientname = "sub_led";

class callback : public virtual mqtt::callback
{
    std::string dataset = "../data/actuator_state.json";
    void message_arrived(mqtt::const_message_ptr msg) override
    {
        try
        {
            log(clientname, std::string("Message received: ") + msg->get_payload_str());
            json obj = json::parse(msg->get_payload_str()); // Parse incoming message
            std::ifstream in(dataset);  // Load actuator_state.json
            json target;
            try
            {
                target = json::parse(in);
            }
            catch (json::exception &e)
            {
                target = json::parse("[]");
            }


            target.push_back(obj); // Append to deserialized actuator_state.json
            std::ofstream of(dataset); // Serialize and write target to actuator_state.json
            of << std::setw(4) << target << std::endl; // Pretty print for readability
            log(clientname, "Written to JSON file");

        }
        catch (json::exception &e)
        {
            log(dataset, e.what(), true);
        }

    }
};

int main()
{
    Client client(clientname);

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
