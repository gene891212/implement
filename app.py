import json

from flask import Flask
from flask_mqtt import Mqtt

from models import Biomedical, db

app = Flask(__name__)

app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['MQTT_BROKER_URL'] = '192.168.0.200'
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_TLS_ENABLED'] = False
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///db.sqlite3'

db.init_app(app)
db.create_all(app=app)
db.app = app
mqtt = Mqtt(app)

@app.route('/api/oxygen/<int:oxygen>', methods=["POST"])
@app.route('/api/oxygen/<float:oxygen>', methods=["POST"])
def save_oxygen(oxygen):
    # FIXME: add oxygen and heart rate to parameter
    data = Biomedical(value=oxygen)
    db.session.add(data)
    db.session.commit()
    return data.as_dict()


@app.route('/api/oxygen', methods=["GET"])
@app.route('/api/oxygen/', methods=["GET"])
def get_oxygen():
    data = Biomedical.query.order_by(-Biomedical.id).first()
    return data.as_dict()


@app.route('/api/oxygens', methods=["GET"])
def get_oxygens():
    # FIXME: modify return value
    data = Biomedical.query.all()
    print(data)
    return {"oxygens": [i.as_dict() for i in data]}


@app.route('/')
def hello():
    mqtt.publish('control', 'from flask')
    return 'Hello, World!'


@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('biomedical')


@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    msg = message.payload.decode()
    biomedical = json.loads(msg)
    print(biomedical)
    data = Biomedical(
        biomedical['oxygen'],
        biomedical['heartRate'],
    )
    db.session.add(data)
    db.session.commit()
