#include "sensor.h"

Sensor::Sensor(std::string dataset, int period)
{
    m_period = period;
    m_dataset = dataset;

    try
    {
        std::fstream f(dataset);
        data = json::parse(f);
        it = data.begin();
    }
    catch(json::exception &e)
    {
        log(m_dataset, e.what(), true);
        m_loaded = false;
        return;
    }
    m_loaded = true;
}

std::string Sensor::getSample()
{
    if(!reachedEnd())
    {
        try
        {
            auto item = *it;
            item["timestamp"] = getTimestamp();
            std::string result = item.dump(); // Serialize JSON data, send it as a string
            it++;
            return result;
        }
        catch (json::exception &e)
        {
            log(m_dataset, e.what(), true);
            return "";
        }
    }
    return "";
}

bool Sensor::loaded() const
{
    return m_loaded;
}
int Sensor::period() const
{
    return m_period;
}

bool Sensor::reachedEnd() const
{
    return it == data.end();
}
std::string Sensor::filePath() const
{
    return m_dataset;
}

