import base64
import json
import datetime
from google.cloud import bigquery
import paho.mqtt.client as mqtt


DEVICE_ID='device1'
PROJECT_ID='local-incline-419216 '
CLOUD_REGION='europe-west1'
REGISTRY_ID='mi-registro'


MQTT_BRIDGE_HOSTNAME='iot.ac.uma.es'
MQTT_BRIDGE_PORT=1883


def on_connect(client, userdata, glags, reason_code, properties):
     print("Connected with result code " + str(reason_code))

     client.subscribe("+/data/pub")
     print ("Subscribed")

def on_message(client, userdata, msg):

    print(msg.topic +" "+str(msg.payload))
   
    response_dict = json.loads(msg.payload)
    response_dict['when'] = datetime.datetime.now()

    rows_to_insert = [response_dict]

    print('Start insertion')  
    client = bigquery.Client()
    dataset_id = 'finalproject'  # replace with your dataset ID
    table_id = 'finalproject'  # replace with your table ID
    table_ref = client.dataset(dataset_id).table(table_id)
    table = client.get_table(table_ref)
    
    #response_dict=json.loads(pubsub_message)
    rows_to_insert = [ response_dict]
    
    errors = client.insert_rows(table, rows_to_insert)  # API request
    print(errors)
    print('End of insertion')

def on_event (client, userdata, msg):
     print("Event number: "+msg.topic+" "+str(msg.payload))

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BRIDGE_HOSTNAME, MQTT_BRIDGE_PORT, 60)

client.loop_forever()