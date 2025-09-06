#include "../shared.h"
#include "sensor.h"
#include <thread>

int main()
{
    Sensor ultrasonic("../data/ultrasonic.json", 500);

    if(!ultrasonic.loaded())
    {
        return 1;
    }

    Client client("pub_ultrasonic");
    if(!client.connectClient())
    {
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
        std::this_thread::sleep_for(std::chrono::seconds(ultrasonic.period()));
    }

    log(client.name(), "Finished simulation.");
    client.disconnectClient();
    return 0;
}
