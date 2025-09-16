#include "../shared.h"
#include "../service.h"
#include <thread>
#include <signal.h>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

const std::string clientname = "sub_led";

Service *ref = nullptr;
void sigintHandler(int dummy)
{
    if(ref)
    {
        ref->byebye();
    }
    exit(0);
}
class callback : public virtual mqtt::callback
{
    std::string serverAddr = "";
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
public:
    void setServerAddr(std::string s)
    {
        serverAddr = s;
    }
};

int main()
{
    std::string serverAddr = "";

    Client *client;
    callback cb;

    Service ledService(
        "building/led",
        LED_SERVICE,
        "led",
        "IoTActuator",
        "1.0"
        );

    ref = &ledService;

    signal(SIGINT, sigintHandler);

    bool serviceStarted = false;
    while(ledService.listenToBroadcast(&serverAddr))
    {
        log("ssdp", "MSearch successful");
        log("ssdp", serverAddr);
        if(!serviceStarted && serverAddr != "")
        {
            client = new Client(clientname, serverAddr);
            serviceStarted = true;
            log(client->name(), "Starting MQTT service");
            if(!client->connectClient())
            {
                ledService.byebye();
                return 1;
            }
            cb.setServerAddr(serverAddr);
            client->setCallback(cb);
            client->subscribe(LED_TOPIC);
        }
    }

    client->disconnectClient();
    delete client;
    return 0;
}
