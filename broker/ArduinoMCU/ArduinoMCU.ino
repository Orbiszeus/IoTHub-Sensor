#include <NTPClient.h>

/*
 Basic ESP8266 MQTT example

 Principales diferencias con el ejemplo de la librería PubSubClient:
   Uso del tipo String para manejar texto
   Utiliza el ID del chip en setup() para:
     - construir un identificador único para conectar con MQTT (ESP_???????)
     - construir topics únicos para esta placa (infind/ESP_???????/+)
   Conecta a MQTT usando usuario/contraseña (conecta_mqtt())
   Configura el servidor MQTT para admitir mensajes hasta 512 bytes (setup())
   En callback():
     - copia el mensaje (buffer de bytes) a un String
     - comprueba el valor del topic
   Al enviar un mensaje enciende momentaneamente el LED correspondiente al GPIO16
*/
#include <string>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"          // Incluimos la libreria ESP32
#include <MQUnifiedsensor.h> // Incluimos la libreria de MQ135

// Definiciones
#define pin A0                 // Analog input 0 of your arduino
#define RatioMQ135CleanAir 3.6 // RS / R0 = 3.6 ppm
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define type "MQ-135"         // MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define LEDROJO D0            // RED LED for anomalies on gas
#define LEDVERDE D2           // GREEN LED for anomalies on temperature
#define LEDAMARILLO D5        // YELLOW LED for anomalies on humidity

float temp_max_limite = 29;
float temp_min_limite = 10;
float hum_max_limite = 70;
float hum_min_limite = 30;
float co_limite;
float alcohol_limite = 80;
float co2_limite;
float tolueno_limite;
float nh4_limite;
float acetona_limite;

// Declare Sensor
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

DHTesp dht;

WiFiClient wClient;
PubSubClient mqtt_client(wClient);

// Update these with values suitable for your network.
const String ssid = "set your WiFi SSID";
const String password = "set your password";
const String mqtt_server = "35.240.58.211";
const String mqtt_user = "infind";
const String mqtt_pass = "zancudo";

// cadenas para topics e ID
String ID_PLACA;
String topic_PUB;
String topic_SUB_REDLED;
String topic_SUB_YELLOWLED;
String topic_SUB_GREENLED;

String topic_PUB_TEMP_ALARM;
String topic_PUB_HUM_ALARM;
String topic_PUB_ALCOHOL_ALARM;
String NOMBRE = "JUANZAMORANO";
bool all_ok;

// GPIOs
// int LED1 = 2;
// int LED2 = 16;

//-----------------------------------------------------
void conecta_wifi()
{
  Serial.println("Connecting to " + ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LEDAMARILLO, !digitalRead(LEDAMARILLO));
    delay(200);
    Serial.print(".");
  }
  digitalWrite(LEDAMARILLO, HIGH);
  Serial.println();
  Serial.println("WiFi connected, IP address: " + WiFi.localIP().toString());
}

//-----------------------------------------------------
void conecta_mqtt()
{
  // Loop until we're reconnected
  while (!mqtt_client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect(ID_PLACA.c_str(), mqtt_user.c_str(), mqtt_pass.c_str()))
    {
      Serial.println(" conectado a broker: " + mqtt_server);
      mqtt_client.subscribe(topic_SUB_REDLED.c_str());
      mqtt_client.subscribe(topic_SUB_YELLOWLED.c_str());
      mqtt_client.subscribe(topic_SUB_GREENLED.c_str());
    }
    else
    {
      Serial.println("ERROR:" + String(mqtt_client.state()) + " reintento en 5s");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//-----------------------------------------------------
void procesa_mensaje(char *topic, byte *payload, unsigned int length)
{
  String message = String(std::string((char *)payload, length).c_str());
  Serial.println("Message received [" + String(topic) + "] " + message);
  // check the topic
  if (String(topic) == topic_SUB_REDLED)
  {
    if (message[0] == '0')
    {
      Serial.println("Turning off RED LED....");
      digitalWrite(LEDROJO, LOW); // Turn the LED on (Note that LOW is the voltage level
      delay(10000);
    }
    else
    {
      digitalWrite(LEDROJO, HIGH); // Turn the LED off by making the voltage HIGH
      delay(10000);
    }
  }
  else if (String(topic) == topic_SUB_YELLOWLED)
  {
    if (message[0] == '0')
    {
      Serial.println("Turning off YELLOW LED....");
      digitalWrite(LEDAMARILLO, LOW); // Turn the LED on (Note that LOW is the voltage level
      delay(10000);
    }
    else
    {
      digitalWrite(LEDAMARILLO, HIGH); // Turn the LED off by making the voltage HIGH
      delay(10000);
    }
  }
  else
  {
    if (message[0] == '0')
    {
      Serial.println("Turning off GREEN LED....");
      digitalWrite(LEDVERDE, LOW); // Turn the LED on (Note that LOW is the voltage level
      delay(10000);
    }
    else
    {
      digitalWrite(LEDVERDE, HIGH); // Turn the LED off by making the voltage HIGH
      delay(10000);
    }
  }
}

//-----------------------------------------------------
//     SETUP
//-----------------------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Empieza setup...");
  // Set math model to calculate the PPM concentration and the value of constants
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b

  /*****************************  MQ Init ********************************************/
  // Remarks: Configure the pin of arduino as input.
  /************************************************************************************/
  MQ135.init();
  /*****************************  MQ CAlibration ********************************************/
  // Explanation:
  // In this routine the sensor will measure the resistance of the sensor supposing before was pre-heated
  // and now is on clean air (Calibration conditions), and it will setup R0 value.
  // We recomend execute this routine only on setup or on the laboratory and save on the eeprom of your arduino
  // This routine not need to execute to every restart, you can load your R0 if you know the value
  // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++)
  {
    MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0 / 10);
  Serial.println("  done!.");

  if (isinf(calcR0))
  {
    Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply");
    while (1)
      ;
  }
  if (calcR0 == 0)
  {
    Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply");
    while (1)
      ;
  }
  /*****************************  MQ CAlibration ********************************************/

  pinMode(LEDROJO, OUTPUT); // inicializa GPIO como salida
  pinMode(LEDVERDE, OUTPUT);
  pinMode(LEDAMARILLO, OUTPUT);
  digitalWrite(LEDROJO, LOW);
  digitalWrite(LEDVERDE, HIGH); // apaga el led
  digitalWrite(LEDAMARILLO, LOW);
  dht.setup(5, DHTesp::DHT11); // Conectamos con el sensor DHT al GPIO 5
  // crea topics usando id de la placa
  // ID_PLACA="ESP_" + String( ESP.getChipId() );
  ID_PLACA = String(ESP.getChipId());
  topic_PUB = ID_PLACA + "/data/pub";
  topic_SUB_REDLED = ID_PLACA + "/data/sub/redled/cmd";
  topic_SUB_YELLOWLED = ID_PLACA + "/data/sub/yellowled/cmd";
  topic_SUB_GREENLED = ID_PLACA + "/data/sub/greenled/cmd";

  topic_PUB_TEMP_ALARM = ID_PLACA + "/data/pub/temp/alarm";
  topic_PUB_HUM_ALARM = ID_PLACA + "/data/pub/hum/alarm";
  topic_PUB_ALCOHOL_ALARM = ID_PLACA + "/data/pub/alcohol/alarm";

  conecta_wifi();
  mqtt_client.setServer(mqtt_server.c_str(), 1883);
  mqtt_client.setBufferSize(512); // para poder enviar mensajes de hasta X bytes
  mqtt_client.setCallback(procesa_mensaje);
  conecta_mqtt();
  Serial.println("Identificador placa: " + ID_PLACA);
  Serial.println("Topic publicacion  : " + topic_PUB);
  Serial.println("Topic subscripcion comando LED rojo : " + topic_SUB_REDLED);
  Serial.println("Topic subscripcion comando LED amarillo : " + topic_SUB_YELLOWLED);
  Serial.println("Topic subscripcion comando LED verde : " + topic_SUB_GREENLED);

  Serial.println("Termina setup en " + String(millis()) + " ms");
}

//-----------------------------------------------------

unsigned long ultimo_mensaje = 0;
//-----------------------------------------------------
//     LOOP
//-----------------------------------------------------
void loop()
{

  if (!mqtt_client.connected())
    conecta_mqtt();

  mqtt_client.loop(); // esta llamada para que la librería recupere el control

  unsigned long ahora = millis();

  if (ahora - ultimo_mensaje >= 5000)
  {
    float humedad = dht.getHumidity();
    float temperatura = dht.getTemperature();
    MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
    // monoxido de carbono
    MQ135.setA(605.18);
    MQ135.setB(-3.937);            // Configurate the ecuation values to get CO concentration
    float CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
    MQ135.setA(77.255);
    MQ135.setB(-3.18);                  // Configurate the ecuation values to get Alcohol concentration
    float Alcohol = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    // dioxido de carbono
    MQ135.setA(110.47);
    MQ135.setB(-2.862);             // Configurate the ecuation values to get CO2 concentration
    float CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    MQ135.setA(44.947);
    MQ135.setB(-3.445);                 // Configurate the ecuation values to get Tolueno concentration
    float Tolueno = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    MQ135.setA(102.2);
    MQ135.setB(-2.473);             // Configurate the ecuation values to get NH4 concentration
    float NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    MQ135.setA(34.668);
    MQ135.setB(-3.369);                 // Configurate the ecuation values to get Acetona concentration
    float Acetona = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    ultimo_mensaje = ahora;
    String mensaje = "{\"temperature\": " + String(temperatura) + ", \"humidity\": " + String(humedad) + ", \"co\": " + String(CO) + ", \"Alcohol\": " + String(Alcohol) + ", \"CO2\": " + String(CO2) + ", \"Tolueno\": " + String(Tolueno) + ", \"NH4\": " + String(NH4) + ", \"Acetona\": " + String(Acetona) + ", \"ID_PLACA\": " + String(ID_PLACA) + "}";
    Serial.println();
    Serial.println("Topic   : " + topic_PUB);
    Serial.println("Payload : " + mensaje);

    // Send data to the MQTT broker
    mqtt_client.publish(topic_PUB.c_str(), mensaje.c_str());

    if (temperatura > temp_max_limite)
    {
      digitalWrite(LEDVERDE, LOW);
      digitalWrite(LEDROJO, HIGH);
      all_ok = false;
      String temp_alarm = "1";
      mqtt_client.publish(topic_PUB_TEMP_ALARM.c_str(), temp_alarm.c_str());

      Serial.println("La temperatura esta por encima del limite");
    }
    else
    {
      if (temperatura < temp_min_limite)
      {
        digitalWrite(LEDVERDE, LOW);
        digitalWrite(LEDROJO, HIGH);
        all_ok = false;
        String temp_alarm = "1";
        mqtt_client.publish(topic_PUB_TEMP_ALARM.c_str(), temp_alarm.c_str());
        Serial.println("La temperatura esta por debajo del limite");
      }
      else
      {
        all_ok = true;
        String temp_alarm = "0";
        mqtt_client.publish(topic_PUB_TEMP_ALARM.c_str(), temp_alarm.c_str());
        Serial.println("La temperatura esta bien");
      }
    }
    if (humedad > hum_max_limite)
    {
      digitalWrite(LEDVERDE, LOW);
      digitalWrite(LEDROJO, HIGH);
      all_ok = false;
      String hum_alarm = "1";
      mqtt_client.publish(topic_PUB_HUM_ALARM.c_str(), hum_alarm.c_str());
      Serial.println("humedad por encima del maximo");
    }
    else
    {
      if (humedad < hum_min_limite)
      {
        digitalWrite(LEDVERDE, LOW);
        digitalWrite(LEDROJO, HIGH);
        all_ok = false;
        String hum_alarm = "1";
        mqtt_client.publish(topic_PUB_HUM_ALARM.c_str(), hum_alarm.c_str());
        Serial.println("La humedad esta por debajo del limite");
      }
      else
      {
        all_ok = true;
        String hum_alarm = "0";
        mqtt_client.publish(topic_PUB_HUM_ALARM.c_str(), hum_alarm.c_str());
        Serial.println("La humedad esta bien");
      }
    }
    // cualquier valor de gas fuera de los limites enciende el led rojo
    if (Alcohol > alcohol_limite)
    {
      digitalWrite(LEDVERDE, LOW);
      digitalWrite(LEDROJO, HIGH);
      all_ok = false;
      String alcohol_alarm = "1";
      mqtt_client.publish(topic_PUB_ALCOHOL_ALARM.c_str(), alcohol_alarm.c_str());
      Serial.println("El alcohol esta por encima del limite");
    }
    else
    {
      all_ok = true;
      String alcohol_alarm = "0";
      mqtt_client.publish(topic_PUB_ALCOHOL_ALARM.c_str(), alcohol_alarm.c_str());
      Serial.println("El alcohol esta bien");
    }

    digitalWrite(LEDAMARILLO, HIGH); // enciende el led al enviar mensaje
    delay(200);
    digitalWrite(LEDAMARILLO, LOW);
  }
  // if (digitalRead(LED2)==LOW && ahora-ultimo_mensaje>=100)
  //   digitalWrite(LED2, LOW);
}
