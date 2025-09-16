#include "../shared.h"
#include "../service.h"
#include "sensor.h"
#include <thread>
#include <signal.h>
#include "../ssdp/lssdpcpp/lssdpcpp.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Intercept SIGINT (Ctrl-C) signal in order to send ssdp:byebye before termination
Service *ref = nullptr;
void sigintHandler(int dummy)
{
    if(ref)
    {
        ref->byebye();
    }
    exit(0);
}

int main()
{
    std::string serverAddr = "";
    volatile bool finishedSimulation = false;
    Service ultrasonicService(
        "building/ultrasonic",
        ULTRA_SERVICE,
        "ultrasonic",
        "IoTSensor",
        "1.0"
    );
    /*
     * The main service is activated upon SSDP discovery.
     * When the root controller discovers this device, it runs
     * the service in a detached thread.
     */
    auto mqttService = [&]() -> int {
        // Load simulated data, set sampling period to 1000ms
        Sensor ultrasonic("../data/ultrasonic.json", 1000);

        if(!ultrasonic.loaded())
        {
            ultrasonicService.byebye();
            finishedSimulation = true;
            return 1;
        }

        // Initiate MQTT node
        Client client("pub_ultrasonic", serverAddr);
        if(!client.connectClient())
        {
            ultrasonicService.byebye();
            finishedSimulation = true;
            return 1;
        }
        // Perform the main simulation loop
        while(!ultrasonic.reachedEnd())
        {
            std::string sample = ultrasonic.getSample();
            if(sample == "")
            {
                break;
            }
            // Send simulated data to MQTT topic
            client.publish(ULTRA_TOPIC, sample);
            log(client.name(), std::string("Sent sample: ") + sample);
            std::this_thread::sleep_for(std::chrono::milliseconds(ultrasonic.period()));
        }

        log(client.name(), "Finished simulation.");
        ultrasonicService.byebye();
        finishedSimulation = true;
        return 0;

    };

    // Setup SIGINT handler
    ref = &ultrasonicService;
    signal(SIGINT, sigintHandler);

    // Listen for M-Search messages
    bool serviceStarted = false;
    while(!finishedSimulation && ultrasonicService.listenToBroadcast(&serverAddr))
    {
        log("ssdp", "MSearch successful");
        if(!serviceStarted && serverAddr != "")
        {
            log("ssdp", std::string("Connecting to MQTT Broker ") + serverAddr);
            log("mqtt", "Starting MQTT service...");
            serviceStarted = true;
            // Run the MQTT service as a detached thread
            std::thread(mqttService).detach();
        }
    }

    return 0;
}
