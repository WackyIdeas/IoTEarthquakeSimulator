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

// MQTT Topics
#define ACCEL_TOPIC "building/accel"
#define ULTRA_TOPIC "building/ultra"
#define STATUS_TOPIC "building/status"
#define LED_TOPIC "building/led"

// SSDP service names
#define LED_SERVICE "service/led"
#define ULTRA_SERVICE "service/ultrasonic"
#define ACCEL_SERVICE "service/accelerometer"

// SSDP config
#define SERVICE_MAX_AGE 3600

// Get formatted timestamp for logging purposes
std::string getTimestamp();
void log(std::string entity, std::string msg, bool error = false);

#include "client.h"

#endif // SHARED_H
