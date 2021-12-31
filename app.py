from flask import Flask, jsonify
from flask_mqtt import Mqtt

from models import OxygenData, db

app = Flask(__name__)

app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['MQTT_BROKER_URL'] = '192.168.0.200'
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_TLS_ENABLED'] = False
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///db.sqlite3'

mqtt = Mqtt(app)
db.init_app(app)


@app.route('/api/oxygen/<int:oxygen>')
def save_oxygen(oxygen):
    data = OxygenData(value=oxygen)
    db.session.add(data)
    db.session.commit()
    return jsonify(data.as_dict())


@app.route('/')
def hello():
    mqtt.publish('test', 'from flask')
    return 'Hello, World!'


@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('test')


@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    data = dict(
        topic=message.topic,
        payload=message.payload.decode()
    )
    print(client, userdata, message)
