#include "../shared.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

const std::string clientname = "sub_status";
class callback : public virtual mqtt::callback
{
    void message_arrived(mqtt::const_message_ptr msg) override
    {
        try
        {
            log(clientname, std::string("Message received: ") + msg->get_payload_str());
            json results = json::parse(msg->get_payload_str());

            // timestamp
            // ultrasonic status
            // accel status
            // actuator status

            std::string output = "\n";

            std::string ts = results["timestamp"].template get<std::string>();
            std::string ul = results["ultrasonic"].template get<std::string>();
            std::string ac = results["accelerometer"].template get<std::string>();
            std::string al = results["actuator"].template get<std::string>();
            std::string st = results["state"].template get<std::string>();
            ts.pop_back();

            std::string sv = std::to_string(results["severity"].template get<int>());

            output += std::string("\t[") + ts + std::string("]\n");
            output += std::string("\tUltrasonic sensor: ") + ul + std::string("\n");
            output += std::string("\tAccelerometer sensor: ") + ac + std::string("\n");
            output += std::string("\tActuator: ") + al + std::string("\n");
            output += std::string("\tSeverity: ") + sv + std::string("\n");

            log("Hardware Status", output);
        }
        catch(json::exception &e)
        {
            log(clientname, e.what(), true);
        }
    }
};

int main()
{
    Client client(clientname, SERVER_ADDR);
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
