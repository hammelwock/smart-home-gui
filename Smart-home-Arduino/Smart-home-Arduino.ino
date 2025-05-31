#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

StaticJsonDocument<2000> loadConfig();
void saveConfig(StaticJsonDocument<2000> doc);

OneWire oneWire(8);
DallasTemperature sensors(&oneWire);

int currentPulsePin = -1; // текущий пин для замера импульса (-1 — не назначен)
volatile unsigned long pulseStart = 0;
volatile unsigned long pulseWidth = 0;
volatile bool pulseMeasured = false;
volatile bool measuringPulse = false;

// Обработчик прерывания
void pulseISR() {
  bool level = digitalRead(currentPulsePin);

  if (measuringPulse) {
    if (level == HIGH) {
      pulseStart = micros();
    } else {
      pulseWidth = micros() - pulseStart;
      pulseMeasured = true;
      measuringPulse = false;
    }
  }
}

// Установка прерывания на нужный пин
void attachPulseInterrupt(int pin) {
  // Отсоединяем старое прерывание, если было
  if (currentPulsePin != -1) {
    detachInterrupt(digitalPinToInterrupt(currentPulsePin));
  }

  pinMode(pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin), pulseISR, CHANGE);
  currentPulsePin = pin;
}

void setup() 
{
    Serial.begin(115200);
    sensors.begin(); // Start up the library for ds18b20
    sensors.setResolution(12); // Resolution of ds18b20
    pinMode(13, OUTPUT);
}

unsigned long Timer1 = 0;

void loop() 
{
    if (millis() - Timer1 >= 1000)
    {
        sensors.requestTemperatures(); // request to all devices on the bus
        Timer1 = millis();
    }

    if (Serial.available()) {
        StaticJsonDocument<2000> doc;
        DeserializationError error = deserializeJson(doc, Serial);

        if (!error) {
            String cmd = doc["cmd"];
            int pin = doc["pin"];

            StaticJsonDocument<2000> response;
            response = doc;
            response["recd"] = true;

            if (cmd == "pinMode") {
                String mode = doc["mode"];
                pinMode(pin, (mode == "input") ? INPUT : OUTPUT);
                serializeJson(response, Serial);
                Serial.println();
            } 
            else if (cmd == "read") {
                String type = doc["type"];

                if (type == "analog") {
                    response["value"] = analogRead(pin);
                } 
                else if (type == "digital") {
                    response["value"] = digitalRead(pin);
                } 
                else if (type == "ds18b20") {
                    response["value"] = sensors.getTempCByIndex(doc["pin"]);
                }
                else if (type == "pulse") {
                    attachPulseInterrupt(pin);
                    pulseMeasured = false;
                    measuringPulse = true;

                    unsigned long timeout = millis() + 1000; // 1 сек таймаут

                    while (!pulseMeasured && millis() < timeout) {
                      // ожидание результата
                    }

                    if (pulseMeasured) {
                      response["value"] = pulseWidth;
                    } else {
                      response["value"] = -1; // ошибка - не измерено
                    }
                }

                serializeJson(response, Serial);
                Serial.println();
            } 
            else if (cmd == "digitalWrite") {
                digitalWrite(pin, doc["setValue"]);
                serializeJson(response, Serial);
                Serial.println();
            }
            else if (cmd == "saveConfig") {
                saveConfig(response["config"]);
                serializeJson(response, Serial);
                Serial.println();
            }
            else if (cmd == "loadConfig") {
                //response["config"] = loadConfig();
                serializeJson(loadConfig(), Serial);
                Serial.println();
            }
        }
    }
}

void saveConfig(StaticJsonDocument<2000> doc)
{
  char buffer [2000];
  serializeJson(doc, buffer);

  for (int i = 0; i < strlen(buffer); ++i) {
    EEPROM.update(i, buffer[i]);
  }

 EEPROM.update(strlen(buffer), '\0');
}

StaticJsonDocument<2000> loadConfig()
{
  char buffer[2000];
  for (int i = 0; i < 2000; ++i) {
    buffer[i] = EEPROM.read(i);
    if (buffer[i] == '\0') 
      break;
  }

    
  StaticJsonDocument<2000> doc;
  DeserializationError error = deserializeJson(doc, buffer);

  return doc;
}
