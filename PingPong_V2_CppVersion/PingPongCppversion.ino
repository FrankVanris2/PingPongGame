/*
 Name:		PingPongCppversion.ino
 Created:	3/9/2023 7:49:20 PM
 Author:	Frank Vanris
 Desc: Creating the PingPong game but in CPP
*/

// the setup function runs once when you press reset or power the board
//#include "Hardware.h"
//#include <LiquidCrystal.h>
#include "PingPongManager.h"

PingPongManager pingPongManager;

//contains the setup for the program to run
void setup() {
    pingPongManager.setup();
}

// the loop function runs over and over again until power down or reset
void loop() {
    pingPongManager.loop();
}
