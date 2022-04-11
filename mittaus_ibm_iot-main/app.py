import json
import os
from dotenv import load_dotenv

import paho.mqtt.client as mqtt
import cloudand

"""
Based on: 
https://pypi.org/project/paho-mqtt/

Interfacing with mqtt:
http://mqtt-explorer.com/

Autentication values are retrieved from .env file
"""
load_dotenv()


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    # iot-2/type/esp32/id/esp3201/evt/+/fmt/json
    client.subscribe("iot-2/type/esp32/id/esp32_ry4/evt/heat/fmt/json")




# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    payload =json.loads( msg.payload.decode("utf-8"))
    print(f'{msg.topic}: {payload}')
    if "d" in payload:
        if "r0" in payload["d"]:
            print(f'{msg.topic}: {payload}')
            token = cloudand.login()
            cloudand.post_value(token, payload["d"]["r0"][0])


client = mqtt.Client(os.environ.get('IOT_CLIENT_ID'))

client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(os.environ.get('IOT_USERNAME'), os.environ.get('IOT_PASSWORD'))
client.connect(os.environ.get('IOT_HOST'), 1883, 60)
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
