#include "shared.h"

std::string getTimestamp()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    return std::string(std::ctime(&t_c));
}

void log(std::string entity, std::string msg, bool error)
{
    if(error) std::cerr << "[ERROR]" << " [" << entity << "] " << msg << std::endl;
    else std::cout << "[" << entity << "] " << msg << std::endl;
}
