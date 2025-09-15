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
    Sensor ultrasonic("../data/ultrasonic.json", 1000);

    if(!ultrasonic.loaded())
    {
        return 1;
    }

    Client client("pub_ultrasonic");
    if(!client.connectClient())
    {
        return 1;
    }

    Service ultrasonicService(
        "building/ultrasonic",   
        "service/ultrasonic",    
        "ultrasonic",            
        "IoTSensor",             
        "1.0"                    
    );

    ref = &ultrasonicService;
    signal(SIGINT, sigintHandler);

    std::thread ssdpThread([&](){
    if(!ultrasonicService.listenToBroadcast())
    {
        log("pub_ultrasonic", "Something wrong happened with SSDP discovery", true);
    }
    });


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
    client.disconnectClient();
    return 0;
}
