#ifndef SERVICE_H
#define SERVICE_H

#include "shared.h"

class Service
{
public:

    Service(std::string location, std::string uid, std::string search_target, std::string productName, std::string version);
    ~Service();

    bool listenToBroadcast();
    void byebye();

private:
    std::string m_uid;
    lssdp::Service *m_service = nullptr;
    bool m_running = true;

};


#endif // SERVICE_H
