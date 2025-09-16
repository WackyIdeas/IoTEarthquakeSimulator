#ifndef SENSOR_H
#define SENSOR_H

#include "../shared.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/*
 * The sensor class pulls data from a JSON file and
 * iteratively provides samples on demand. This should
 * be done with a while loop that depends on the
 * reachedEnd() method, and a sleep method that blocks
 * execution for period() milliseconds, to simulate new
 * messages coming in at regular intervals.
 */
class Sensor
{
public:
    Sensor(std::string dataset, int period);
    std::string getSample();       // Returns a serialized string of the JSON object pointed to by the current iterator
                                   // This should be deserialized once the message arrives via callbacks from other MQTT nodes
                                   // This gets around the limitation of MQTT messages being limited to byte streams/strings of text.
    int period() const;
    bool loaded() const;
    bool reachedEnd() const;       // The sensor has reached the end of its simulation if the iterator is at the end of the JSON STL container
    std::string filePath() const;
private:
    int m_period;
    bool m_loaded = false;
    json data;
    json::iterator it;      // STL iterator of the JSON object, which is supposed to be an array of objects
    std::string m_dataset;  // Filepath of the JSON file that is being loaded
};

#endif // SENSOR_H
