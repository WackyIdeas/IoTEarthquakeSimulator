#!/bin/bash

echo "Downloading JSON C++ library..."
curl -L https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz > json.tar.xz
tar -xf json.tar.xz

git clone https://github.com/eclipse/paho.mqtt.c.git
git clone https://github.com/eclipse/paho.mqtt.cpp

# Build C library, install it globally
echo "Building Paho C library..."
cd paho.mqtt.c
git checkout v1.3.15
cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON -DCMAKE_INSTALL_PREFIX=/usr
sudo cmake --build build/ --target install
echo "Done."

# Build C++ library, install it globally
echo "Building Paho C++ library..."
cd ../paho.mqtt.cpp
git checkout v1.5.3
git submodule init
git submodule update

cmake -Bbuild -H. -DPAHO_WITH_MQTT_C=ON -DPAHO_BUILD_EXAMPLES=ON -DCMAKE_INSTALL_PREFIX=/usr
sudo cmake --build build/ --target install
echo "Done."
