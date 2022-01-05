# IOT System Implement

## Installation

MQTT

```bash
sudo apt-get install mosquitto mosquitto-clients
```

### Configuring Websockets On Your Own Mosquitto Broker

MQTT over Websockets usually uses port 9001. Make change to the `/etc/mosquitto/mosquitto.conf` file, by adding the following:

```
listener 1833

listener 9001
protocol websockets
```

Python package

```bash
pipenv install
```

Arduino libraries

- MQTTPubSubClient


## Usage


## Reference

- [MQTT.js github](https://github.com/mqttjs/MQTT.js)
- [MQTTTPubSubClient github](https://github.com/hideakitai/MQTTPubSubClient)
- [MQTT basic setting](https://blog.gtwang.org/iot/raspberry-pi/raspberry-pi-mosquitto-mqtt-broker-iot-integration/)
- [MQTT websocket setting](http://www.steves-internet-guide.com/mqtt-websockets/)
- [MQTT multiple listener setting](http://www.steves-internet-guide.com/mossquitto-conf-file/)
