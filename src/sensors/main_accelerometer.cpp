#include "../shared.h"
#include "sensor.h"
#include <thread>

int main()
{
    Sensor accelerometer("../data/accelerometer.json", 2);

    if(!accelerometer.loaded())
    {
        return 1;
    }

    Client client("pub_accelerometer");
    if(!client.connectClient())
    {
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
        std::this_thread::sleep_for(std::chrono::seconds(accelerometer.period()));
    }

    log(client.name(), "Finished simulation.");
    client.disconnectClient();
    return 0;
}

