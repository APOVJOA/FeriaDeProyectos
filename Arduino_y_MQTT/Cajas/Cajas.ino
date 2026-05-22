
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "estructura_buffer.h"
#include "conexion_mqtt.h"

#define TRIGPIN 13 
#define ECHOPIN 14
#define PIN_ROJO 2
#define PIN_VERDE 1
#define PIN_BOTON_EMERGENCIA 11

#define MAX_DISTANCE 700 

#define TIEMPO_PAUSA_DE_TAREAS 3000

const float UMBRAL_CAJA = 10.0f; // en cm 

static TaskHandle_t medir_sensor_handle = NULL;
static TaskHandle_t productor_handle = NULL;
static TaskHandle_t consumidor_handle = NULL;


portMUX_TYPE sensor_data_access = portMUX_INITIALIZER_UNLOCKED;

//variables globales
volatile float Distancia_leida_raw = 0.0f;
volatile float Distancia_extraida_buffer = 0.0f;

//isr variables 
volatile bool Seta_ISR = false;
volatile bool Estado_ISR = false;


// variable de estado para MQTT
bool hay_caja_actualmente = false;

// prototipos de funciones y tareas 
void IRAM_ATTR isr();
void conectarWIFI();
void conectarMQTT();
float getSonar();

static void Medir_Sensor_task(void * pvParameters);
static void Productor_task(void * pvParameters);
static void Consumidor_task(void * pvParameters);

void setup()
{
  Serial.begin(115200);
  
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(PIN_ROJO, OUTPUT);
  pinMode(PIN_VERDE, OUTPUT);

  //boton emergencia 
  pinMode(PIN_BOTON_EMERGENCIA, INPUT_PULLUP);
  attachInterrupt(PIN_BOTON_EMERGENCIA, isr, FALLING);

  buf.bufIN = 0; buf.bufOUT = 0; buf.bufCount = 0;

  conectarWIFI();
  client.setServer(mqtt_broker, 1883);

  // Crear Tareas
  xTaskCreate(Medir_Sensor_task, "Medir", 4096, NULL, 1, &medir_sensor_handle);
  xTaskCreate(Productor_task, "Productor", 4096, NULL, 2, &productor_handle);
  xTaskCreate(Consumidor_task, "Consumidor", 4096, NULL, 3, &consumidor_handle);
}

void loop()
{
    if (!client.connected())
    {
        conectarMQTT();
    }
    client.loop();

  //entraria en seccion del boton de emergencia 
    if(Estado_ISR)
    {
        Estado_ISR = false;
        if(Seta_ISR)
        {
            Serial.println("SETA ON");
            client.publish(station_command_topic, MENSAJE_ACTIVAR_SETA);
            for(uint8_t blink = 0; blink < 10; blink++)
            {
                digitalWrite(PIN_ROJO, HIGH); 
                digitalWrite(PIN_VERDE, HIGH); 
                vTaskDelay(pdMS_TO_TICKS(150)); 
                digitalWrite(PIN_ROJO, LOW); 
                digitalWrite(PIN_VERDE, LOW); 
                vTaskDelay(pdMS_TO_TICKS(150)); 
            }
        }
        else
        {
        Serial.println("SETA OFF");
        client.publish(station_command_topic, MENSAJE_DESACTIVAR_SETA);
        }
    }

    while(Seta_ISR)
    {
      delay(TIEMPO_PAUSA_DE_TAREAS);
    }


    // extrae la última distancia procesada por el buffer
    portENTER_CRITICAL(&sensor_data_access);
    float distancia_final = Distancia_extraida_buffer;
    portEXIT_CRITICAL(&sensor_data_access);



    // Lógica de detección por cambio de estado
    if (distancia_final > 0.1 && distancia_final < UMBRAL_CAJA) 
    {
        if (!hay_caja_actualmente) 
        { 
        hay_caja_actualmente = true;
        digitalWrite(PIN_ROJO, HIGH); 
        digitalWrite(PIN_VERDE, LOW); 
        client.publish(station_command_topic, MENSAJE_CAJA);
        Serial.println(">>> MQTT: Caja_detectada");
        }
    } 
    else 
    {
        if (hay_caja_actualmente)
        { 
        hay_caja_actualmente = false;
        digitalWrite(PIN_ROJO, LOW);
        digitalWrite(PIN_VERDE, HIGH);
        client.publish(station_command_topic, MENSAJE_NOCAJA);
        Serial.println(">>> MQTT: Mesa_vacia");
        }
    }
    
    delay(100);
}


void IRAM_ATTR isr()
{   
    static uint32_t tiempo_final;
    uint32_t tiempo_inicio = millis();

    if( (tiempo_final - tiempo_inicio) > 1000)
    {
        Seta_ISR = !Seta_ISR;
        Estado_ISR = true;
    }
    tiempo_final = tiempo_inicio;

}

//Tareas

static void Medir_Sensor_task(void * pvParameters)
{
  for (;;)
    {
        while(Seta_ISR)
        {
            delay(TIEMPO_PAUSA_DE_TAREAS);
        }
        float d = getSonar();
        portENTER_CRITICAL(&sensor_data_access);
        Distancia_leida_raw = d;
        portEXIT_CRITICAL(&sensor_data_access);
        vTaskDelay(pdMS_TO_TICKS(150)); 
    }
}

static void Productor_task(void * pvParameters)
{
    for (;;) 
    {
        while(Seta_ISR)
        {
            delay(TIEMPO_PAUSA_DE_TAREAS);
        }
        float valor;
        portENTER_CRITICAL(&sensor_data_access);
        valor = Distancia_leida_raw;
        portEXIT_CRITICAL(&sensor_data_access);

        put_item(valor);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void Consumidor_task(void * pvParameters)
{
  float item;
  for (;;)
    {
        while(Seta_ISR)
        {
            delay(TIEMPO_PAUSA_DE_TAREAS);
        }
        
        if (get_item(&item) == 0)
        {
        portENTER_CRITICAL(&sensor_data_access);
        Distancia_extraida_buffer = item;
        portEXIT_CRITICAL(&sensor_data_access);
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}


