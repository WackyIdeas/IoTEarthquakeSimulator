#include "service.h"
#include <functional>

void Service::byebye()
{
    if(m_service)
    {
        log(m_uid, "Closing client");
        m_service->sendNotifyByeBye();
    }
}
Service::Service(std::string location, std::string uid, std::string search_target, std::string productName, std::string version)
{
    m_service = new lssdp::Service(BROADCAST_LOCATION,
        std::chrono::seconds(SERVICE_MAX_AGE),
        location,
        uid,
        search_target,
        productName,
        version
    );

    m_uid = uid;

    try
    {
        m_service->sendNotifyAlive();
    }
    catch(std::exception &e)
    {
        log(m_uid, e.what(), true);
        m_running = false;
    }
}
Service::~Service()
{
    if(m_service)
    {
        delete m_service;
    }
}

bool Service::listenToBroadcast(std::string *sender_addr)
{
    if(!m_running) return false;
    try
    {
        log(m_uid, "Checking for MSearch queries...");
        if(!m_service->checkForMSearchAndSendResponse(std::chrono::seconds(1), sender_addr))
        {
            log(m_uid, m_service->getLastSendErrors(), true);
            return false;
        }
    }
    catch(std::exception &e)
    {
        log(m_uid, e.what(), true);
        return false;
    }
    return true;
}
