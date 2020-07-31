#include <WiFi.h>
#include "env.h"
#include "dht_and_arduino_json.h"
#include "lcd_i2c_and_time.h"

#define DHTPIN 23
#define DHTTYPE DHT11

#define LCD_COL 16
#define LCD_ROW 2

void setup(){
  
  // Container for LCD output 
  char clockTime[]= "TIME:00:00:00";
  char date[] = "DATE:00/00/2000";
  char text[] = "Keep it up, you can do it mate!!!";

  // Container for DHT output
  StaticJsonDocument<500> doc;

  // LCD variables setup
  WiFiUDP ntpUDP;
  LiquidCrystal_I2C lcd(0x27, LCD_COL, LCD_ROW);
  NTPClient timeClient( ntpUDP, 
                        "time.nist.gov", 
                        25200, // 25200 = GMT+7 (3600 = GMT+1)
                        60000 );

  // Temperature variables setup
  DHT dht(DHTPIN, DHTTYPE);

  //Optional WiFi Config
  IPAddress local_IP(IP1, IP2, IP3, IP4);
  IPAddress gateway(GIP1, GIP2, GIP3, GIP4);
  IPAddress subnet(MASK1, MASK2, MASK3, MASK4);
  IPAddress primaryDNS(1, 1, 1, 1);  // Cloudflare's DNS 
  IPAddress secondaryDNS(1, 0, 0, 1);// Cloudflare's DNS
  
  // Initialize Section
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  // Optional WiFi Config 
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)){
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(__SSID, __PASSWORD);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }
  Serial.println("\n WiFi Connected");

  timeClient.begin();

  for (;;){
    for (int i = 0; i < 5; i++){
      updateTime(timeClient, lcd, clockTime, date);
      updateTemperature(dht, doc);
      delay(500);
    }
    printFX(lcd, text, 0, 0, 100);
    printFX(lcd, text, 0, 1, 100);
  }

}

void loop(){
}
