#include <WiFi.h>
#include "env.h"
#include "dht_and_arduino_json.h"
#include "lcd_i2c_and_time.h"

#define DHTPIN 23
#define DHTTYPE DHT11

#define LCD_COL 16
#define LCD_ROW 2

const char* ssid = __SSID;
const char* password = __PASSWORD;

//Optional
IPAddress local_IP(IP1, IP2, IP3, IP4);
IPAddress gateway(GIP1, GIP2, GIP3, GIP4);
IPAddress subnet(MASK1, MASK2, MASK3, MASK4);

void setup(){
  
  char clockTime[]= "TIME:00:00:00";
  char date[] = "DATE:00/00/2000";
  char text[] = "Keep it up, you can do it mate!!!";

  WiFiUDP ntpUDP;
  LiquidCrystal_I2C lcd(0x27, LCD_COL, LCD_ROW);
  NTPClient timeClient(ntpUDP, "time.nist.gov", 25200, 60000);// 25200 = GMT+7 (3600 = GMT+1)

  DHT dht(DHTPIN, DHTTYPE);
  StaticJsonDocument<500> doc;

  Serial.begin(115200);

  lcd.begin();
  lcd.backlight();

  //Initialize WiFi
  if (!WiFi.config(local_IP, gateway, subnet)){
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);

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
      updateTemperature(dht, doc, Serial);
      delay(500);
    }
    printFX(lcd, text, 0, 0, 100);
    printFX(lcd, text, 0, 1, 100);
  }

}

void loop(){
}
