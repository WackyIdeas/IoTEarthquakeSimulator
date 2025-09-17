# IoT Earthquake Simulator

## Requirements: 

- Linux environment
- Docker 

## EMQX Broker 

Running the EMQX broker instance for the first time:
```bash
$ sudo docker run -d --name emqx -p 1883:1883 -p 8083:8083 -p 8883:8883 -p 18083:18083 emqx/emqx:latest
```

If an EMQX instance was previously created this way, running the instance again simply becomes:

```bash
$ sudo docker container start emqx
```

The web interface can be accessed then via `https://localhost:18083`

To shut down the Docker container:

```bash
$ sudo docker ps # List all running docker containers
CONTAINER ID   IMAGE            COMMAND   CREATED   STATUS    PORTS     NAMES
9f548271ad47   emqx/emqx:latest ...       ...       ...       ...       emqx
$ sudo docker container stop emqx
```



## Dependencies

- cmake
- curl
- ninja (Optional)

Ubuntu: 
```bash
sudo apt install curl cmake ninja-build
```

Run the following script to install and build the dependencies:

```bash
$ sh install_deps.sh
```

If the Paho C and C++ libraries are already installed on the system, rebuilding can be skipped by

```bash
$ sh install_deps.sh --skip-paho
```

## Build the project 

```bash
$ sh install.sh
```

If Ninja is installed on the system, the build process can be sped up by: 

```bash
$ sh install.sh --ninja
```

# Notes

- It's assumed that the MQTT controller node also acts as the root controller (SSDP) and runs the MQTT broker. 
- SSDP and MQTT related ports must be enabled in the firewall in order for SSDP discovery and the MQTT broker to work correctly across the local network

# Features

- Manages node discovery through SSDP via broadcast IP
- SSDP: support for discovery (M-SEARCH) and advertisement (NOTIFY; ssdp:alive, ssdp:byebye)
- Most other nodes connect to the controller node by getting the sender IP via SSDP
- Reading and writing from/to JSON files as part of a loose simulation/proof of concept
- Sending and receiving serialized (un)retained JSON messages via MQTT topics

# Credits 

- [EMQX](https://github.com/emqx/emqx)
- [paho.mqtt.cpp](https://github.com/eclipse-paho/paho.mqtt.cpp)
- [lssdp-cpp](https://github.com/jeanreP/lssdp-cpp)
