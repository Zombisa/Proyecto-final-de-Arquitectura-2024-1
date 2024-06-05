/**
 * @file monitor.h
 * @brief Header file containing functions for environmental monitoring.
 */
#ifndef MONITOR
#define MONITOR

#include "utilidad.h"


/**
 * @brief Function to print environmental parameters to the LCD screen.
 * It includes temperature, humidity, and light intensity.
 */
void printMonitorAmbiental() {
  lcd.clear();
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int photocell = analogRead(photocellPin);

  lcd.setCursor(0, 0);

  lcd.print("TEMP:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("HUM:");
  lcd.print(h);
  
  lcd.setCursor(4,1);
  lcd.print("LUZ:");
  lcd.print(photocell);

  if(t > tempMax){
    Serial.print("La temperatura es mayor a ");
    Serial.println(tempMax);
    taskTemp.Start();
  }
  else if(photocell > luzMax){
    Serial.print("La luz es mayor a ");
    Serial.println(luzMax);
    taskLuz.Start();
  }

}

/**
 * @brief Function to print magnetic field intensity to the LCD screen.
 * It monitors the magnetic field and triggers a task if it exceeds a predefined threshold.
 */
void printMonitorHall(){
  
  int mag = analogRead(HallPin);

  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("MAG:");
  lcd.print(mag);

  if(mag > hallMax)
    taskHall.Start();
}

#endif  // MONITOR