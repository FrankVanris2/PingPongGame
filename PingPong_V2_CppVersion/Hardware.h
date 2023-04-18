// Hardware.h
//By:Frank Vanris
//Date:3/17/2023
//Desc:This is the hardware.h file that contains all of the pins and methods that will be based on the
//Hardware that I am using.

#ifndef _HARDWARE_h
#define _HARDWARE_h
#include "Rules_Players.h"
//specific pins for the Arduino and LCD
#define PLAYER1_INCREMENT_BUTTON_CONNECTION 7
#define PLAYER1_DECREMENT_BUTTON_CONNECTION 8

#define PLAYER2_INCREMENT_BUTTON_CONNECTION 9
#define PLAYER2_DECREMENT_BUTTON_CONNECTION 10

#define PLAYER1_INCREMENT_BUTTON 0
#define PLAYER1_DECREMENT_BUTTON 1
#define PLAYER2_INCREMENT_BUTTON 2
#define PLAYER2_DECREMENT_BUTTON 3

#define LED_PLAYER1 A0
#define LED_PLAYER2 A1

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class Hardware {
	public:
		//setting up the pins for the arduino board
		void pinSetUp();


		//begin 
		void begin();

		//lighting up the LED for each player for when they serve.
		void lightLEDForPlayer(Player& player1, Player& player2, Rules& rules);

		//Printing score. 
		void printLCD(Player& player1, Player& player2);

		//Printing the winner of the matches
		void printLCDWinner(Rules& rules);

		//Blinker for who wins the match.
		void matchBlinker(char* text);

		//Switching off led's.
		void ledClear();

		//Set winner of a set.
		void ledForSetWinner(Rules& rules);

		//Reducing repetition.
		void blinkWinner(int ledPlayer);

		//Resetting the game after somebody wins.
		void resetMatch(Player& player1, Player& player2, Rules& rules);
};

#endif

