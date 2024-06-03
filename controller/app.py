from fastapi import FastAPI, HTTPException
import repository
from fastapi.middleware.cors import CORSMiddleware
from contextlib import asynccontextmanager
from fastapi_mqtt.fastmqtt import FastMQTT
from fastapi_mqtt.config import MQTTConfig
from gmqtt import Client as MQTTClient
from typing import Any

fast_mqtt = FastMQTT(config=MQTTConfig())

mqtt_config = MQTTConfig(
    host="35.240.58.211",
    port=1883,
    keepalive=60
)

fast_mqtt = FastMQTT(config=mqtt_config)
@asynccontextmanager
async def _lifespan(_app: FastAPI):
    await fast_mqtt.mqtt_startup()
    yield
    await fast_mqtt.mqtt_shutdown()
    
app = FastAPI(lifespan=_lifespan)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

repo = repository.Repository()


@fast_mqtt.on_connect()
def connect(client, flags, rc, properties):
    fast_mqtt.client.subscribe("/2500319") #subscribing mqtt topic
    print("Connected: ", client, flags, rc, properties)

'''
This mqtt subscribe method has subscribed to the broker's method
that sends alerts when some values are over what it should be,
like if the temperature is higher then what normally is an alert
will be sent to here. 
'''
@fast_mqtt.subscribe("2500319/data/pub/temp/alarm", qos=1)
async def alert_temp(client: MQTTClient, topic: str, payload: bytes, qos: int, properties: Any):
    print("ALERT TEMPERATURE: ", topic, payload.decode(), qos, properties)

@fast_mqtt.subscribe("2500319/data/pub/hum/alarm", qos=1)
async def alert_hum(client: MQTTClient, topic: str, payload: bytes, qos: int, properties: Any):
    print("ALERT HUMIDITY: ", topic, payload.decode(), qos, properties)
    
@fast_mqtt.subscribe("2500319/data/pub/alcohol/alarm", qos=1)
async def alert_alco(client: MQTTClient, topic: str, payload: bytes, qos: int, properties: Any):
    print("ALERT ALCOHOL: ", topic, payload.decode(), qos, properties)
     
# @fast_mqtt.on_message()
# async def message(client: MQTTClient, topic: str, payload: bytes, qos: int, properties: Any):
#     print("Received message: ", topic, payload.decode(), qos, properties)

@fast_mqtt.on_disconnect()
def disconnect(client: MQTTClient, packet, exc=None):
    print("Disconnected")
    
@fast_mqtt.on_subscribe()
def subscribe(client: MQTTClient, mid: int, qos: int, properties: Any):
    print("subscribed", client, mid, qos, properties)

@app.get("/all_items")
def get_all(limit: int):
    return repo.get_all_items(limit)

@app.get("/latest_item")
def get_latest():
    return repo.get_latest_item()

'''
This endpoint is to send a command to the mqtt broker to close
or open the led light, we send either 0 or 1 to do that. 
'''
@app.get("/change_led")
async def func():
    fast_mqtt.publish("2500319/data/sub/led/cmd", '1')  
    return {"result": True, "message": "Published"}

@app.get("/alerts")
def get_alerts():
    return 0
