// PingPongManager.h
// By:Frank Vanris
// Date:3/9/2023
// Description: Ping Pong Manager that holds in all the main pins for output and input.
#include <LiquidCrystal.h>
#include "Hardware.h"
#include "Rules_Players.h"
#ifndef _PINGPONGMANAGER_h
#define _PINGPONGMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"

#else
	#include "WProgram.h"
#endif

class PingPongManager {
private:

	//Hardware object
	Hardware mHardware;

	//struct conventions.
	struct Player player1;
	struct Player player2;
	struct Rules rules;
public:
	
	//Setup method like ino.
	void setup();

	//Loop method like ino.
	void loop();

	//Method for incrementing and decrementing the scores for both players.
	bool checkButtonStatus(Player* player, bool incrButton, bool decrButton);

	//Checking for button status if it's being pressed or not.
	bool checkHighForButton(int isHigh, int buttonID);

	//Handle rule method where all the rules take place in how the scoreboard for the pong game works.
	void handleRules(Player* player1, Player* player2);

};

#endif

