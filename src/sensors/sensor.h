#ifndef SENSOR_H
#define SENSOR_H

#include "../shared.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// The sensor class pulls data from a JSON file and
class Sensor
{
public:
    Sensor(std::string dataset, int period);
    std::string getSample();

    int period() const;
    bool loaded() const;
    bool reachedEnd() const;
    std::string filePath() const;
private:
    int m_period;
    bool m_loaded = false;
    json data;
    json::iterator it;
    std::string m_dataset;
};

#endif // SENSOR_H
