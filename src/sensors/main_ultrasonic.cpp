#include "../shared.h"
#include "../service.h"
#include "sensor.h"
#include <thread>
#include <signal.h>
#include "../ssdp/lssdpcpp/lssdpcpp.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

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

    volatile bool finishedSimulation = false;
    Service ultrasonicService(
        "building/ultrasonic",
        ULTRA_SERVICE,
        "ultrasonic",
        "IoTSensor",
        "1.0"
    );
    auto mqttService = [&]() -> int {
        Sensor ultrasonic("../data/ultrasonic.json", 1000);

        if(!ultrasonic.loaded())
        {
            ultrasonicService.byebye();
            finishedSimulation = true;
            return 1;
        }

        Client client("pub_ultrasonic", SERVER_ADDR);
        if(!client.connectClient())
        {
            ultrasonicService.byebye();
            finishedSimulation = true;
            return 1;
        }

        while(!ultrasonic.reachedEnd())
        {
            std::string sample = ultrasonic.getSample();
            if(sample == "")
            {
                break;
            }
            client.publish(ULTRA_TOPIC, sample);
            log(client.name(), std::string("Sent sample: ") + sample);
            std::this_thread::sleep_for(std::chrono::milliseconds(ultrasonic.period()));
        }

        log(client.name(), "Finished simulation.");
        ultrasonicService.byebye();
        finishedSimulation = true;
        return 0;

    };

    ref = &ultrasonicService;
    signal(SIGINT, sigintHandler);

    bool serviceStarted = false;
    while(!finishedSimulation && ultrasonicService.listenToBroadcast())
    {
        log("ssdp", "MSearch successful");
        if(!serviceStarted)
        {
            log("mqtt", "Starting MQTT service...");
            serviceStarted = true;
            std::thread(mqttService).detach();
        }
    }

    return 0;
}
