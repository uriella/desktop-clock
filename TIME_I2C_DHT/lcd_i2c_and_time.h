#ifndef LCD_I2C_AND_TIME
#define LCD_I2C_AND_TIME

#include <WiFiUDP.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>

void updateTime(NTPClient timeClient, LiquidCrystal_I2C lcd, char clockTime[], char date[]);
void printFX(LiquidCrystal_I2C lcd, char text_[], int col, int row, int delayPeriod);

#endif