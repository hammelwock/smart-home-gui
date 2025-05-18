#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(8);
DallasTemperature sensors(&oneWire);

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
                    unsigned long startTime = micros();
                    bool signal = false;
                    while(!signal)
                    {
                      if (micros() - startTime > 10000)
                        break;
                      signal = digitalRead(pin);
                    }

                    startTime = micros();

                    while(signal)
                    {
                      if (micros() - startTime > 10000)
                        break;
                      signal = digitalRead(pin);
                    }
                    while(!signal)
                    {
                      if (micros() - startTime > 10000)
                        break;
                      signal = digitalRead(pin);
                    }

                    response["value"] = micros() - startTime;
                }

                serializeJson(response, Serial);
                Serial.println();
            } 
            else if (cmd == "digitalWrite") {
                digitalWrite(pin, doc["setValue"]);
                serializeJson(response, Serial);
                Serial.println();
            }
        }
        else
          Serial.println("Error");
    }
}
