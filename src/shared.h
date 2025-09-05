#ifndef SHARED_H
#define SHARED_H

#include <mqtt/async_client.h>
#include <string>
#include <iostream>
#include <chrono>

#define SERVER_ADDR "tcp://localhost:1883"
#define ACCEL_TOPIC "building/accel"
#define ULTRA_TOPIC "building/ultra"
#define STATUS_TOPIC "building/status"
#define LED_TOPIC "building/led"

std::string getTimestamp();
void log(std::string entity, std::string msg, bool error = false);

#include "client.h"

#endif // SHARED_H
