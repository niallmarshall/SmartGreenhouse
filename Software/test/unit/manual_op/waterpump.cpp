/**************************************************************************/
/*!
 *  @file waterpump.cpp
 *  @author A. Saikia
 *  @brief Turns water motor on for 10 seconds.
 *  @version 0.1
 *  @date 2019-04-12
 *  @copyright Copyright (c) 2019
 *
*/

#include <wiringPi.h>
#include <wiringPiI2C.h> // add "-lwiringPi" to compile
#include <stdio.h>
#include <iostream>      // add "-lstdc++" to compile
#include <unistd.h>

/** @brief GPIO pin of motor. */
int water_pump = 23;
/** Main function. */
int main (){
  wiringPiSetup();
  pinMode(water_pump, OUTPUT);
  digitalWrite(water_pump, HIGH);
  delay(10000);         // pump is on for 10 seconds
  digitalWrite(water_pump, LOW);
}
