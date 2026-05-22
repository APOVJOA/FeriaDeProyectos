#ifndef CONEXION_MQTT_H
#define CONEXION_MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>

#define MENSAJE_CAJA "Detecto_Caja"
#define MENSAJE_NOCAJA "No_Detecto_Caja"

#define MENSAJE_ACTIVAR_SETA "EMERGENCIA"
#define MENSAJE_DESACTIVAR_SETA "LIBERANDO"

//configuracion mqtt
const char *ssid = "UPV-PSK"; //recordar cambiar 
const char *password = "giirob-pr2-2023";

const char *mqtt_broker = "broker.emqx.io";
const char *station_command_topic = "giirob/pr2/station/mesa/commands"; // solo enviar 

const char *mqtt_username = "giirob222";
const char *mqtt_password = "UPV2024";
const int mqtt_port = 1883;

//variable global del wifi
WiFiClient espClient;
PubSubClient client(espClient);

#endif