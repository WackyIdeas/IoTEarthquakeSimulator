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
    Sensor accelerometer("../data/accelerometer.json", 1000);

    if(!accelerometer.loaded())
    {
        return 1;
    }

    Client client("pub_accelerometer");
    if(!client.connectClient())
    {
        return 1;
    }

    Service accelerometerService(
        "building/accelerometer",   
        "service/accelerometer",    
        "accelerometer",            
        "IoTSensor",             
        "1.0"                    
    );

    ref = &accelerometerService;
    signal(SIGINT, sigintHandler);

    std::thread ssdpThread([&](){
    if(!accelerometerService.listenToBroadcast())
    {
        log("pub_accelerometer", "Something wrong happened with SSDP discovery", true);
    }
    });

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
    client.disconnectClient();
    return 0;
}

