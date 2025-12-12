#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "1234"
#define STAPSK "wwsh0731"
#endif

ESP8266WiFiMulti WiFiMulti;



const int RED = D0;
const int YELLOW = D1;
const int GREEN = D5;
const int GREEN2 = D2;

const int RED_CIVIL = D7;
const int GREEN_CIVIL = D8;

unsigned long loopLength = 0;
const unsigned long yellowLength = 1000;
const int maxBlinks = 8;
const unsigned long blinkGap = 500;
unsigned long gap = 0;
const unsigned long gapBetweenCycles = 5000;

unsigned long setTimes = yellowLength * 2 + maxBlinks * blinkGap;
unsigned long lastMillis = 0;
long long cycleTime = 0;

//admin stuff
int currentHour = 3;
int currentDay = 3;
int timeBased = 0;
int timeStart = 0;
int timeEnd = 0;
int dayBased = 0;
int dayStart = 0;
int dayEnd = 0;
int turnlight = 0;
bool turnlightturn = true;
int yellowCount = 0;

void allOff() {
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(GREEN2, LOW);
  digitalWrite(RED_CIVIL, LOW);
  digitalWrite(GREEN_CIVIL, LOW);
}
void civilOff() {
  digitalWrite(RED_CIVIL, LOW);
  digitalWrite(GREEN_CIVIL, LOW);
}

void redLight() {
  allOff();
  digitalWrite(RED, HIGH);
  digitalWrite(RED_CIVIL, HIGH);
}

void yellowLight() {
  digitalWrite(GREEN, LOW);
  digitalWrite(GREEN2, LOW);
  digitalWrite(YELLOW, HIGH);
}

void greenLight() {
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, HIGH);
}

void civilGreen() {
  digitalWrite(RED_CIVIL, LOW);
  digitalWrite(GREEN_CIVIL, HIGH);
}

void civilRed() {
  digitalWrite(GREEN_CIVIL, LOW);
  digitalWrite(RED_CIVIL, HIGH);
}

void blinkLight(int pin, int blinks) {
  for (int i = 0; i < blinks; i += 2) {
    digitalWrite(pin, LOW);
    delay(blinkGap);
    digitalWrite(pin, HIGH);
    delay(blinkGap);
  }
}
void blink2Light(int pin, int pin2 ,int blinks) {
  for (int i = 0; i < blinks; i += 2) {
    digitalWrite(pin, LOW);
    digitalWrite(pin2, LOW);
    delay(blinkGap);
    digitalWrite(pin, HIGH);
    digitalWrite(pin2, HIGH);
    delay(blinkGap);
  }
}
void yellowBlinking(){
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(GREEN2, LOW);
  civilOff();
  blinkLight(YELLOW, maxBlinks);
  if(yellowCount >= 15){
    connect("https://script.google.com/macros/s/AKfycbyyVV-ThzEI5TGvOtGYeaMGslrUpKdnkVkn-b92wearQmHvNTdv-ZGB3_ZSEjDiuefF7A/exec?foorinr=1");
    yellowCount = 0;
  } else {
    yellowCount++;
  }
}

void runCycle() {
  unsigned long current = millis() - lastMillis;
  unsigned long cycleSeconds = (loopLength + setTimes) / 1000;
  unsigned long ts = ((cycleTime + current + gap))/1000 % cycleSeconds;
  unsigned long civilBlinkStart = ((loopLength + setTimes) / 2) / 1000;
  
  Serial.println(ts);

  if (timeBased == 1){
    if(timeStart>timeEnd){
      if(currentHour >= timeStart || currentHour <= timeEnd){
        ts = -1;
        yellowBlinking();
      } 
    } else {
      if(timeStart <= currentHour <= timeEnd){
        ts = -1;
        yellowBlinking();
      }
    }
  }

  if (dayBased == 1){
    if(dayStart > dayEnd){
      if(currentDay >= dayStart || currentDay <= dayEnd){
        ts = -1;
        yellowBlinking();
      } 
    } else {
      if(dayStart <= currentDay <= dayEnd){
        ts = -1;
        yellowBlinking();
      }
    }
  }

  if (ts == 0) {
    digitalWrite(RED, LOW);
    if(turnlight == 1){
      blink2Light(GREEN, GREEN2, maxBlinks);
    } else if (turnlight == 2){
      if(turnlightturn){
        blink2Light(GREEN, GREEN2, maxBlinks);
      } 
    } else {
      blinkLight(GREEN_CIVIL, maxBlinks);
    }
    
    yellowLight();
    delay(yellowLength);
    redLight();
    delay(gapBetweenCycles);
    civilGreen();
    connect("https://script.google.com/macros/s/AKfycbyyVV-ThzEI5TGvOtGYeaMGslrUpKdnkVkn-b92wearQmHvNTdv-ZGB3_ZSEjDiuefF7A/exec?foorinr=1");
  }

  if (ts == civilBlinkStart) {
    digitalWrite(RED_CIVIL, LOW);
    blinkLight(GREEN_CIVIL, maxBlinks);
    civilRed();
    delay(gapBetweenCycles);
    digitalWrite(YELLOW, HIGH);
    delay(yellowLength);
    greenLight();
    if(turnlight == 1){
      digitalWrite(GREEN2, HIGH);
    } 
    else if (turnlight == 2){
      if(turnlightturn){
        digitalWrite(GREEN2, HIGH);
        turnlightturn = false;
      } 
      else {
        turnlightturn = true;
      }
    } 
  }

}



void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(RED_CIVIL, OUTPUT);
  pinMode(GREEN_CIVIL, OUTPUT);

  redLight();
  delay(1000);
  
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(STASSID, STAPSK);

  connect("https://script.google.com/macros/s/AKfycbyyVV-ThzEI5TGvOtGYeaMGslrUpKdnkVkn-b92wearQmHvNTdv-ZGB3_ZSEjDiuefF7A/exec?foorinr=1");
  lastMillis = millis();
}

void loop() {
  if (loopLength == 0) {
    redLight();
    return;
  }
  
  runCycle();
}

void connect(String website){
  Serial.printf("%s", website.c_str());
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    //client->setFingerprint(fingerprint_sni_cloudflaressl_com);
    // Or, if you happy to ignore the SSL certificate, then use the following line instead:
    client->setInsecure();

    HTTPClient https;




    Serial.print("[HTTPS] begin...\n");
    https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    if (https.begin(*client, website)) { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);

          StaticJsonDocument<200> doc;

          DeserializationError error = deserializeJson(doc, payload);
          if (error) {
              Serial.print("JSON parse failed: ");
              Serial.println(error.f_str());
              return;
          }

          loopLength = doc[0].as<int>() * 1000 - setTimes;
          gap = doc[1].as<int>() * 1000;
          
          if(cycleTime == 0){
            cycleTime = doc[2].as<long long>(); 
          }

          currentHour = doc[3].as<int>();
          currentDay = doc[4].as<int>();
          timeBased = doc[5].as<int>();
          timeStart = doc[6].as<int>();
          timeEnd = doc[7].as<int>();
          dayBased = doc[8].as<int>();
          dayStart = doc[9].as<int>();
          dayEnd = doc[10].as<int>();
          turnlight = doc[11].as<int>();

          Serial.println(loopLength);
          Serial.println(cycleTime);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
}

