# IOT System Implement

## Installation

MQTT

```bash
sudo apt-get install mosquitto mosquitto-clients
```

### Configuring Websockets On Your Own Mosquitto Broker

MQTT over Websockets usually uses port 9001 but it isn’t fixed.

Make change to the `mosquitto.conf` file, by adding the following:

```
listener 9001
protocol websockets
```

Python package

```bash
pipenv install
```

## Usage


## Reference

- [MQTT basic setting](https://blog.gtwang.org/iot/raspberry-pi/raspberry-pi-mosquitto-mqtt-broker-iot-integration/)
- [MQTT websocket setting](http://www.steves-internet-guide.com/mqtt-websockets/)
