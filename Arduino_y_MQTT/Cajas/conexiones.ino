#include <WiFi.h>
#include <PubSubClient.h>

#include "conexion_mqtt.h"

void conectarWIFI()
{
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
  Serial.println("\nWiFi OK");
}

void conectarMQTT()
{
  while (!client.connected()) 
  {
    Serial.print("Conectando a MQTT...");

    if (client.connect("ESP32_S3", mqtt_username, mqtt_password))
    {
      Serial.println("conectado");
      client.subscribe(station_command_topic);
    }
    else 
    {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" reintentando en 2s");
      delay(2000);
    }
  }
  
}