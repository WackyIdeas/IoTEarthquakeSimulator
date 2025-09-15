#include "../shared.h"
#include <thread>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class callback : public virtual mqtt::callback
{
    // Keep a reference of the parent client
    // Needs to call setParent() before any new messages arrive
    Client *parent;
    json accelerometer_data;
    json ultrasonic_data;
    void message_arrived(mqtt::const_message_ptr msg) override
    {
        try
        {
            if(msg->get_topic() == ACCEL_TOPIC)
            {
                accelerometer_data = json::parse(msg->get_payload_str());
            }

            if(msg->get_topic() == ULTRA_TOPIC)
            {
                ultrasonic_data = json::parse(msg->get_payload_str());
            }

            auto ts = getTimestamp();
            json status;
            status["timestamp"] = ts;
            status["ultrasonic"] = ultrasonic_data.is_null() ? "Off (!!)" : "On";
            status["accelerometer"] = accelerometer_data.is_null() ? "Off (!!)" : "On";

            // Actuator
            json target;
            target["timestamp"] = ts;

            // Logic
            std::string actuatorstate;
            int threshold_accel = 0;
            int threshold_ultra = 0;

            /*
             * The logic that decides when to activate the LED alarm
             * depends on two threshold values for the two sensors.
             *
             * The total threshold is defined as the sum of the two
             * thresholds, and the LED alarm gets the following messages
             * depending on the value of the threshold:
             *
             * 0 -> Safe
             * 1-2 -> Warning
             * 3+ -> Critical
             *
             * The logic that calculates the sensor thresholds is:
             *
             * Accelerometer threshold:
             * g < 0.05 -> 0
             * 0.05 <= g < 0.1 -> 1
             * g > 0.1 -> 3
             *
             * Ultrasonic threshold:
             * d < 2.5 -> 0
             * 2.5 <= d < 5 -> 1
             * d >= 5 -> 2
             *
             */

            if(!accelerometer_data.is_null())
            {
                double g = accelerometer_data["acceleration_g"];
                if(g < 0.05)
                    threshold_accel = 0;
                else if(g >= 0.05 && g < 0.1)
                    threshold_accel = 1;
                else if(g >= 0.1)
                    threshold_accel = 3;
            }

            if(!ultrasonic_data.is_null())
            {
                double d = ultrasonic_data["distance_cm"];
                if(d < 2.5)
                    threshold_ultra = 0;
                else if(d >= 2.5 && d < 5.0)
                    threshold_ultra = 1;
                else if(d >= 5.0)
                    threshold_ultra = 2;
            }

            // Calculate total threshold values
            int total_threshold = threshold_accel + threshold_ultra;
            if(total_threshold == 0)
            {
                actuatorstate = "safe";
                status["actuator"] = "Off";
            }
            else if(total_threshold < 3)
            {
                actuatorstate = "warning";
                status["actuator"] = "On";
            }
            else if(total_threshold >= 3)
            {
                actuatorstate = "critical";
                status["actuator"] = "On";
            }
            else
            {
                status["actuator"] = "N/A";

            }
            target["state"] = actuatorstate;
            status["severity"] = total_threshold;

            // Publish messages, with sanity checks
            if(parent)
            {
                log(parent->name(), std::string("Message received: ") + msg->get_payload_str());
                parent->publish(LED_TOPIC, target.dump(), 1, true);

                if(!status.is_null())
                {
                    parent->publish(STATUS_TOPIC, status.dump(), 1, true);
                }

            }

        }
        catch(json::exception &e)
        {
            log(parent->name(), e.what(), true);
        }

    }
public:
    void setParent(Client* c)
    {
        parent = c;
    }
};

int main()
{
    Client client("controller");
    if(!client.connectClient())
    {
        return 1;
    }

    callback cb;
    cb.setParent(&client);
    client.setCallback(cb);

    client.subscribe(ACCEL_TOPIC);
    client.subscribe(ULTRA_TOPIC);

    // Init SSDP controller
    lssdp::ServiceFinder ssdpController(
        BROADCAST_LOCATION,
        "IoTController",
        "1.0",
        ""
    );


    bool runssdp = true;
    try
    {
        std::chrono::seconds msearch_period = std::chrono::seconds(5);
        auto time = std::chrono::system_clock::now();

        // Send MSearch for the first time as a sanity check
        if (!ssdpController.sendMSearch()) log(client.name(), ssdpController.getLastSendErrors(), true);

        do
        {
            auto time_now = std::chrono::system_clock::now();
            if((time_now - time) >= msearch_period)
            {
                time = time_now;
                log(client.name(), "Querying services via SSDP...");
                ssdpController.sendMSearch();
            }

            ssdpController.checkForServices(
                [&](const lssdp::ServiceFinder::ServiceUpdateEvent& update_event)
                {
                    log(client.name(), "Received message from service:");
                    std::cout << update_event << std::endl;
                },
                std::chrono::seconds(1)); // Timeout after 1 second
        } while(runssdp);

    }
    catch(std::exception &e)
    {
        log(client.name(), e.what(), true); // Log
        runssdp = false;
    }

    client.disconnectClient();
    return 0;
}
