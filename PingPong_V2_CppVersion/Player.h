// Player.h

#ifndef _PLAYER_h
#define _PLAYER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class Player {
private:
	int playerPoints;
	int playerSetPoints;
public:
	Player();
};

#endif

