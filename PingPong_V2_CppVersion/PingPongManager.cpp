//
//By:Frank Vanris
//Date:3/9/2023
//Desc:This holds my main methods for the ping pong game such as the rules that will take place for my code
//to run. This is a ping pong game with 2 players you can easily set this up via a simple led screen and 4 seperate buttons indicating the players increment/decrement outputs.

#include "PingPongManager.h"
#include <LiquidCrystal.h>


//checking if buttons are pressed
bool prevHigh[4] = { false, false, false, false };

//setup for ping pong game.
void PingPongManager::setup() {
    
    //Pin set up
    mHardware.pinSetUp();

    //size of the LCD screen
    mHardware.begin();

    mHardware.printLCD(player1, player2);

    mHardware.lightLEDForPlayer(player1, player2, rules);
}

//The loop function over and over again until power down or reset.
void PingPongManager::loop() {

    //Setting buttons to false since each one is LOW at first until pressed to HIGH.
    bool buttons[4] = { false, false, false, false };

    //Setting the buttons to read its values, and stop it from incrementing when held down.
    buttons[PLAYER1_INCREMENT_BUTTON] = checkHighForButton(digitalRead(PLAYER1_INCREMENT_BUTTON_CONNECTION), PLAYER1_INCREMENT_BUTTON);
    buttons[PLAYER1_DECREMENT_BUTTON] = checkHighForButton(digitalRead(PLAYER1_DECREMENT_BUTTON_CONNECTION), PLAYER1_DECREMENT_BUTTON);
    buttons[PLAYER2_INCREMENT_BUTTON] = checkHighForButton(digitalRead(PLAYER2_INCREMENT_BUTTON_CONNECTION), PLAYER2_INCREMENT_BUTTON);
    buttons[PLAYER2_DECREMENT_BUTTON] = checkHighForButton(digitalRead(PLAYER2_DECREMENT_BUTTON_CONNECTION), PLAYER2_DECREMENT_BUTTON);

    //incrementing and decrementing the scores of player 1 and 2 also printing out the scores.
    bool isChanged = checkButtonStatus(&player1, buttons[PLAYER1_INCREMENT_BUTTON], buttons[PLAYER1_DECREMENT_BUTTON]);
    isChanged = checkButtonStatus(&player2, buttons[PLAYER2_INCREMENT_BUTTON], buttons[PLAYER2_DECREMENT_BUTTON]) || isChanged;

    if (isChanged) {
        //checking up on the rules who wins and who loses.
        handleRules(&player1, &player2);

        //if there is a match winner
        if (rules.matchWinner != NONE) {
            mHardware.ledClear();
            mHardware.printLCDWinner(rules);
            mHardware.resetMatch(player1, player2, rules);
            mHardware.printLCD(player1, player2);
            mHardware.lightLEDForPlayer(player1, player2, rules);
        }

        //if there is a set winner
        else if (rules.setWinner != NONE) {
            mHardware.printLCD(player1, player2);
            mHardware.ledForSetWinner(rules);
            mHardware.lightLEDForPlayer(player1, player2, rules);
            
            //otherwise continue
        }else {
            mHardware.printLCD(player1, player2);
            mHardware.lightLEDForPlayer(player1, player2, rules);
            delay(250);
        }
    }
}

//checking whether the button is pressed if pressed or held the button does not keep incrementing
bool PingPongManager::checkHighForButton(int isHigh, int buttonId) {
    if (isHigh != prevHigh[buttonId]) {
        prevHigh[buttonId] = isHigh;
        if (isHigh)
            return true;
    }
    return false;
}

//states of each button for when it is pressed.
bool PingPongManager::checkButtonStatus(Player* player, bool incrButton, bool decrButton) {
    bool isChanged = false;

    //incrementing points and set points of player 1
    if (incrButton) {
        isChanged = true;
        player->playerPoints++;
    }

    //decrementing points and possibly set points of player 1
    if (decrButton) {
        if (player->playerPoints > 0 || player->playerSetPoints > 0) {
            isChanged = true;
            player->playerPoints--;
            if (player->playerPoints < 0 && player->playerSetPoints > 0) {
                player->playerPoints = 10;
                player->playerSetPoints--;
            }
        }
    }
    return isChanged;
}


void PingPongManager::handleRules(Player* player1, Player* player2) {
    //if player1 or player2 wins
    if ((player1->playerPoints > 10 || player2->playerPoints > 10) && abs(player1->playerPoints - player2->playerPoints) >= 2) {

        //if player1 wins
        if (player1->playerPoints > player2->playerPoints) {
            player1->playerSetPoints++;
            rules.setWinner = PLAYER1;
        }
        else {
            player2->playerSetPoints++;
            rules.setWinner = PLAYER2;
        }
        player1->playerPoints = 0, player2->playerPoints = 0;

        //who won the match
        if (player1->playerSetPoints == 3) {
            rules.matchWinner = PLAYER1;
        }
        else if (player2->playerSetPoints == 3) {
            rules.matchWinner = PLAYER2;
        }
    }
    else {
        //total points and set points
        int numPoints = player1->playerPoints + player2->playerPoints;
        int numSets = player1->playerSetPoints + player2->playerSetPoints;

        //if player1 and player2 have reached 10 it is switching servers every point
        //otherwise the player that serves will need a 2 point difference from the other player.
        if (player1->playerPoints >= 10 && player2->playerPoints >= 10) {
            if (numSets % 2 == 0)
                rules.isServing = numPoints % 2 == 0 ? PLAYER1 : PLAYER2;
            else {
                rules.isServing = numPoints % 2 == 0 ? PLAYER2 : PLAYER1;
            }
        }

        //switch to the other player if either player wins a game
        else {
            if (numSets % 2 == 0)
                rules.isServing = (numPoints / 2) % 2 == 0 ? PLAYER1 : PLAYER2;
            else {
                rules.isServing = (numPoints / 2) % 2 == 0 ? PLAYER2 : PLAYER1;
            }
        }
    }
}


