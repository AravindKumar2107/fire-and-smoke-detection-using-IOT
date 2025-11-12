/********* BLYNK SETTINGS *********/

// REMOVE THIS → makes IRAM overflow
//#define BLYNK_PRINT Serial

#define BLYNK_AUTH_TOKEN "Thkd7luCslbN7Csdf8kuo04bWGfyErlQ"
#define BLYNK_TEMPLATE_ID "TMPL3C3U8bjiP"
#define BLYNK_TEMPLATE_NAME "Project"

#include <Wire.h>
// REMOVE THIS → included automatically by SSD1306
// #include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp8266.h>

/********* OLED DISPLAY *********/

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/********* PIN DEFINITIONS *********/
#define GAS_PIN A0
#define FLAME_PIN D6
#define BUZZER_PIN D5
#define LED_PIN D7

char ssid[] = "Aravind";
char pass[] = "0987654321";
int gasValue;
int flameValue;
int gasThreshold = 350;  // More practical threshold

/********* RECEIVE LED/BUZZER FROM BLYNK *********/
BLYNK_WRITE(V3) {
  digitalWrite(LED_PIN, param.asInt());
}

BLYNK_WRITE(V4) {
  digitalWrite(BUZZER_PIN, param.asInt());
}

void setup() {
  Serial.begin(115200);

  pinMode(GAS_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Wire.begin(D2, D1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Fire & Gas System");
  display.setCursor(0, 8);
  display.println("Aravind Kumar");
  display.println("Riya Kumari");
  display.display();
  delay(2000);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  gasValue = analogRead(GAS_PIN);
  flameValue = digitalRead(FLAME_PIN);

  Blynk.virtualWrite(V1, gasValue);
  Blynk.virtualWrite(V2, flameValue);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("GAS:");
  
  display.setTextSize(1);        
  display.setCursor(40, 0);
  display.println(gasValue);
  

  display.setTextSize(1);
  display.setCursor(0, 10);
  display.print("FLAME:");

  display.setTextSize(1);
  display.setCursor(40, 10);
  display.println(flameValue == 0 ? "FIRE" : "SAFE");
   display.display();

  if (gasValue > gasThreshold || flameValue == 0) {
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("ALERT! Fire/Gas!");

    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    Blynk.logEvent("flame_alert","Fire/Gas Detected! Take immediate action.");

  
  } else {
  
    display.setTextSize(1);
    display.setCursor(0, 20);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    display.println("Status: Normal");
   
  }

  display.display();
  delay(300);
}
