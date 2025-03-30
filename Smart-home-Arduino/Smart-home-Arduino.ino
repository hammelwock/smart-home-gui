#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(8);
DallasTemperature sensors(&oneWire);

void setup() 
{
    Serial.begin(115200);
    sensors.begin(); // Start up the library for ds18b20
    sensors.setResolution(12); //Resolution of ds18b20
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
        //String input = Serial.readStringUntil('\n');  // Читаем входную строку
        //Serial.print("Received: ");  // Выводим полученную строку обратно
        //Serial.println(input);

        StaticJsonDocument<200> doc;
        //DeserializationError error = deserializeJson(doc, input);
        DeserializationError error = deserializeJson(doc, Serial);

        if (!error) {
            String cmd = doc["cmd"];
            int pin = doc["pin"];

            if (cmd == "pinMode") {
                String mode = doc["mode"];
                pinMode(pin, (mode == "input") ? INPUT : OUTPUT);
            } 
            else if (cmd == "read") {
                String type = doc["type"];
                StaticJsonDocument<200> response;
                response["pin"] = pin;

                if (type == "analog") {
                    response["value"] = analogRead(pin);
                } 
                else if (type == "digital") {
                    response["value"] = digitalRead(pin);
                } 
                else if (type == "ds18b20") {
                    response["value"] = sensors.getTempCByIndex(doc["index"]);
                    response["ds18b20Index"] = doc["index"];
                }

                serializeJson(response, Serial);
                Serial.println();
            } 
            else if (cmd == "write") {
                digitalWrite(pin, doc["value"]);
            }
        }
    }
}
