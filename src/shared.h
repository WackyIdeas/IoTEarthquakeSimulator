#ifndef SHARED_H
#define SHARED_H

/*
 * This includes all the shared libraries, values and functions used by the rest of
 * the codebase
 */
#include <mqtt/async_client.h>
#include <string>
#include <iostream>
#include <chrono>
#include "ssdp/lssdpcpp/lssdpcpp.h"

// Broker address
#define SERVER_PORT 1883
#define SERVER_ADDR "tcp://localhost:1883"
#define ACCEL_TOPIC "building/accel"
#define ULTRA_TOPIC "building/ultra"
#define STATUS_TOPIC "building/status"
#define LED_TOPIC "building/led"

#define BROADCAST_LOCATION "http://239.255.255.250:1900"


// Get formatted timestamp for logging purposes
std::string getTimestamp();
void log(std::string entity, std::string msg, bool error = false);

#include "client.h"

#endif // SHARED_H
