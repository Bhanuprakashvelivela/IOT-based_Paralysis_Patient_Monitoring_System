#define xpin 39
#define ypin 35
#define buz 19
#define sw 18
#define BLYNK_TEMPLATE_ID "TMPL4zlwX9hK"
#define BLYNK_DEVICE_NAME "Temperature"
#define BLYNK_AUTH_TOKEN "m_83vktKCtf21JNd58ogHYv-tGIn7rg3"

#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBus = 25;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define REPORTING_PERIOD_MS 1000

char auth[] = BLYNK_AUTH_TOKEN;             // You should get Auth Token in the Blynk App.
char ssid[] = "701_ABHRSU";                                     // Your WiFi credentials.
char pass[] = "7138700654";

PulseOximeter pox;
float BPM, SpO2;
uint32_t tsLastReport = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sensors.begin();
  pinMode(buz,OUTPUT);
  pinMode(sw,INPUT_PULLUP);
  Blynk.begin(auth, ssid, pass);
  
    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
         Serial.println("SUCCESS");
        // pox.setOnBeatDetectedCallback(onBeatDetected);
    }
}



int x,y,SpO22;
void loop() {
  // put your main code here, to run repeatedly:
    pox.update();
    Blynk.run();
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    
        
        if(digitalRead(sw)==LOW)
        {
          digitalWrite(buz,HIGH);
          Serial.print("help me...!");
           Blynk.logEvent("panic","Please Help Me....!!!!");
          delay(2000);
        }
        else
        {
          digitalWrite(buz,LOW);
        }

   if (millis() - tsLastReport > REPORTING_PERIOD_MS)
   {
     
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" bpm / SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");
        Blynk.virtualWrite(V1, 120.6);
        Blynk.virtualWrite(V2, 94);  
        tsLastReport = millis();
   }
    else
   
     mydata();
   
    
}
void mydata()
{
sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    Serial.print("temperature: ");
    Serial.print(temperatureC);
    Serial.println("ºC");
    Blynk.virtualWrite(V3,temperatureC);

x = analogRead(xpin);
y = analogRead(ypin);

Serial.print("X= ");
Serial.println(x);
Serial.print("Y= ");
Serial.println(y);
delay(250);
if((y>=2000)&&(y<=2400))
    {
      Serial.println("I am thirsty");
      Blynk.logEvent("notify1","I am thirsty....!!!!");
      delay(2000);
      }
    else if((y>=1300)&& (y<=1500))
    {
      Serial.println("I am hungry");
      Blynk.logEvent("notify2","I am hungry....!!!!");
      delay(2000);
      }
      else if((x>=1400) && (x<=1600))
    {
      Serial.println("I need to use restroom");
      Blynk.logEvent("notify3","I need to use restroom....!!!!");
      delay(2000);
      }
      else if((x>=2000) && (x<=2400))
    {
      Serial.println("I want to sit");
      Blynk.logEvent("notify3","I want to sit....!!!!");
      delay(2000);
      }
      else
      {
        Serial.println("stop");
        }
}
