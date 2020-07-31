#include "lcd_i2c_and_time.h"

void updateTime(NTPClient timeClient, LiquidCrystal_I2C lcd, char clockTime[], char date[]){
  
    timeClient.update();
    
    byte second_, minute_, hour_, day_, month_;
    int year_;
    unsigned long unix_epoch = timeClient.getEpochTime();// Get Unix epoch time from NTP server
    
    second_ = second(unix_epoch);
    minute_ = minute(unix_epoch);
    hour_ = hour(unix_epoch);
    clockTime[12] = second_ % 10 + 48;
    clockTime[11] = second_ / 10 + 48;
    clockTime[9] = minute_ % 10 + 48;
    clockTime[8] = minute_ / 10 + 48;
    clockTime[6] = hour_ % 10 + 48;
    clockTime[5] = hour_ / 10 + 48;
    lcd.setCursor(0, 0);
    lcd.print(clockTime);

    day_ = day(unix_epoch);
    month_ = month(unix_epoch);
    year_ = year(unix_epoch);
    date[14] = (year_ % 10) % 10 + 48;
    date[13] = (year_ / 10) % 10+ 48;
    date[9] = month_ % 10 + 48;
    date[8] = month_ / 10 + 48;
    date[6] = day_ % 10 + 48;
    date[5] = day_ / 10 + 48;
    lcd.setCursor(0, 1);
    lcd.print(date);
        
    delay(200);
  
}

void printFX(LiquidCrystal_I2C lcd, char text_[], int col, int row, int delayPeriod){

  if (strlen(text_) < 17){
    lcd.setCursor(col, row);
    lcd.print(text_);
  } else {
    for (int i = col; (strlen(text_) - i) >= 8; i++){
      for (int j = 0; j != 16; j++){
        lcd.setCursor(j, row);
        if ((i+j) >= strlen(text_)){
          lcd.print(" ");
        } else {
          lcd.print(text_[i+j]);
        }
        delay(100);
      }
    }
    lcd.clear();
  }
  delay(delayPeriod);
}