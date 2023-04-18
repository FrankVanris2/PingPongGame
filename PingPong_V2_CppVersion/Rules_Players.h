#pragma once
//Rules_Player.h
//By:Frank Vanris
//Date:3/17/2023
//Desc:Holds the structs for the Players and the Rules.

#ifndef _RULES_PLAYERS_H
#define _RULES_PLAYERS_H

#define PLAYER1 0
#define PLAYER2 1
#define NONE -1

//Player Struct
struct Player {
	int playerPoints = 0;
	int playerSetPoints = 0;
};

//Rules Struct
struct Rules {
	int isServing = PLAYER1;
	int setWinner = NONE;
	int matchWinner = NONE;
};
#endif // !_RULES_PLAYERS_H

