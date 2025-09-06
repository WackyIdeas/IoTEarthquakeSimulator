# IoT Earthquake Simulator

## Requirements: 

- Linux environment, will likely not work on Windows
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
