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
    std::string serverAddr = "";
    volatile bool finishedSimulation = false;
    Service accelerometerService(
        "building/accelerometer",
        ACCEL_SERVICE,
        "accelerometer",
        "IoTSensor",
        "1.0"
    );
    auto mqttService = [&]() -> int {
        Sensor accelerometer("../data/accelerometer.json", 1000);

        if(!accelerometer.loaded())
        {
            accelerometerService.byebye();
            finishedSimulation = true;
            return 1;
        }

        Client client("pub_accelerometer", serverAddr);
        if(!client.connectClient())
        {
            accelerometerService.byebye();
            finishedSimulation = true;
            return 1;
        }

        while(!accelerometer.reachedEnd())
        {
            std::string sample = accelerometer.getSample();
            if(sample == "")
            {
                break;
            }
            client.publish(ACCEL_TOPIC, sample);
            log(client.name(), std::string("Sent sample: ") + sample);
            std::this_thread::sleep_for(std::chrono::milliseconds(accelerometer.period()));
        }

        log(client.name(), "Finished simulation.");
        accelerometerService.byebye();
        finishedSimulation = true;
        return 0;
    };

    ref = &accelerometerService;
    signal(SIGINT, sigintHandler);

    bool serviceStarted = false;
    while(!finishedSimulation && accelerometerService.listenToBroadcast(&serverAddr))
    {
        log("ssdp", "MSearch successful");
        log("ssdp", serverAddr);
        if(!serviceStarted && serverAddr != "")
        {
            log("mqtt", "Starting MQTT service...");
            serviceStarted = true;
            std::thread(mqttService).detach();
        }
    }

    return 0;
}

