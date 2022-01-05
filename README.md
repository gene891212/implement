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
- [MQTT PubSubClient github](https://github.com/hideakitai/MQTTPubSubClient)
- [MQTT basic setting](https://blog.gtwang.org/iot/raspberry-pi/raspberry-pi-mosquitto-mqtt-broker-iot-integration/)
- [MQTT websocket setting](http://www.steves-internet-guide.com/mqtt-websockets/)
- [MQTT multiple listener setting](http://www.steves-internet-guide.com/mossquitto-conf-file/)
- [NodeMCU driver and Arduino setting](https://sites.google.com/site/arduinochutiyan/home/nodemcu/arduino-%E7%92%B0%E5%A2%83%E5%BB%BA%E7%BD%AE-%E5%AE%89%E8%A3%9Desp8266%E9%96%8B%E7%99%BC%E6%9D%BF?authuser=0)
