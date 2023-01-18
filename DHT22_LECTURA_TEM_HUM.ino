#include <DHT.h> 
#include "UbidotsEsp32Mqtt.h"          //Cargamos la librería DHT
#define DHTTYPE   DHT22   //Definimos el modelo del sensor (hay //otros DHT)
#define DHTPIN    2     // Se define el pin 2 del Arduino para conectar el sensor DHT11

DHT dht(DHTPIN, DHTTYPE, 22); 

const char *UBIDOTS_TOKEN = "BBFF-mJErTxhd6IfCEAuHTPLYMxTFTI82zt";
const char *WIFI_SSID = "NotQuery";      
const char *WIFI_PASS = "L4bLRD$@";
const char *DEVICE_LABEL = "esp32";
const char *VARIABLE_LABEL_1 = "temperatura"; 
const char *VARIABLE_LABEL_2 = "humedad";
const int PUBLISH_FREQUENCY = 5000; 
int timer;

Ubidots ubidots(UBIDOTS_TOKEN);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
void setup()
{
  Serial.begin(9600);   //Se inicia la comunicación serial 
  dht.begin();
  delay(1000); 
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  timer = millis();
}
void loop()
{

if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  if (millis() - timer > PUBLISH_FREQUENCY) 
  {
    /*Hacemos la medición de Temperatura y Humedad del sensor, y lo definimos en variables Float */
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    /*Definimos que el valor de Temperatura será enviado por la variable 1, a la plataforma Ubidots*/
    ubidots.add(VARIABLE_LABEL_1, t);
    /*Definimos que el valor de Humedad será enviado por la variable 2, a la plataforma Ubidots*/
    ubidots.add(VARIABLE_LABEL_2, h);
    /*Hacemos la publicación de los datos en el dispositivo definido*/
    ubidots.publish(DEVICE_LABEL);
    /*Para mostrar los datos, los imprimimos en el terminal Serial*/
    Serial.println("Enviando los datos a Ubidots: ");
    Serial.println("Temperatura: " + String(t));
    Serial.println("Humedad: " + String(h));
    Serial.println("-----------------------------------------");
    timer = millis();
  }
  /*Agregamos un delay de 10 segundos para el envío, y que luego de este tiempo, se proceda a reiniciar el bucle*/
  delay(10000); 
  ubidots.loop();

   //Se espera 2 segundos para seguir leyendo datos
}
