#include "../shared.h"
#include "../service.h"
#include <thread>
#include <signal.h>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

const std::string clientname = "sub_led";

// Intercept SIGINT (Ctrl-C) signal in order to send ssdp:byebye before the application closes
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

    // Setup Ctrl-C intercept
    ref = &ledService;
    signal(SIGINT, sigintHandler);

    bool serviceStarted = false;

    // Periodically listen to the SSDP brd IP for M-Search messages
    while(ledService.listenToBroadcast(&serverAddr))
    {
        log("ssdp", "MSearch successful");
        // In case the server address is found, connect to its device's MQTT broker
        if(!serviceStarted && serverAddr != "")
        {
            log("ssdp", std::string("Connecting to MQTT Broker ") + serverAddr);
            serviceStarted = true;
            client = new Client(clientname, serverAddr);
            log(client->name(), "Starting MQTT service");
            if(!client->connectClient()) // Failed to connect to MQTT broker
            {
                ledService.byebye();
                return 1;
            }
            client->setCallback(cb); // Set subscription callback
            client->subscribe(LED_TOPIC); // Subscribe to actuator topic
        }
    }

    client->disconnectClient();
    delete client;
    return 0;
}
