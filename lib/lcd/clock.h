#ifndef CLOCK
#define CLOCK

#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiUDP.h>

void updateTime(NTPClient timeClient, LiquidCrystal_I2C lcd, char clockTime[],
                char date[]);
void printFX(LiquidCrystal_I2C lcd, char text_[], int col, int row,
             int delayPeriod);

#endif