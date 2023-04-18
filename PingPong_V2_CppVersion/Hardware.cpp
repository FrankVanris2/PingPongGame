//Hardware.cpp
//By:Frank Vanris
//Date:3/17/2023
//Desc:This contains all my methods for the Hardware to work on my breadboard, and the arduino

#include "Hardware.h"
#include <LiquidCrystal.h>
//initialize the library by associating any needed LCD interface pin
//with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void Hardware::pinSetUp() {
    //Player One Increment and Decrement
    pinMode(PLAYER1_INCREMENT_BUTTON_CONNECTION, INPUT);
    pinMode(PLAYER1_DECREMENT_BUTTON_CONNECTION, INPUT);

    //Player Two Increment and Decrement
    pinMode(PLAYER2_INCREMENT_BUTTON_CONNECTION, INPUT);
    pinMode(PLAYER2_DECREMENT_BUTTON_CONNECTION, INPUT);

    //Player LED light up
    pinMode(LED_PLAYER1, OUTPUT);
    pinMode(LED_PLAYER2, OUTPUT);
}

//lighting up the LED for the Player that starts the serve
void Hardware::lightLEDForPlayer(Player& player1, Player& player2, Rules& rules) {
    //serving rules for who is serving and who is not
    if (rules.isServing == PLAYER1) {
        digitalWrite(LED_PLAYER1, HIGH);
        digitalWrite(LED_PLAYER2, LOW);
    }
    else {
        digitalWrite(LED_PLAYER1, LOW);
        digitalWrite(LED_PLAYER2, HIGH);
    }
}

//printing the text on the LCD screen for player1 and player2
void Hardware::printLCD(Player& player1, Player& player2) {
    lcd.clear();

    //printing the score on the screen at (0, 0) on the LCD for Player 1
    lcd.setCursor(0, 0);
    lcd.print("P1SP:");
    lcd.print(player1.playerSetPoints);

    //printing the points on the screen at (0, 1) on the LCD for Player1
    lcd.setCursor(0, 1);
    lcd.print("P1P:");
    lcd.print(player1.playerPoints);

    //printing the score on the screen at (16, 0) on the LCD for Player 2
    lcd.setCursor(10, 0);
    lcd.print("P2SP:");
    lcd.print(player2.playerSetPoints);

    //printing the points on the screen at (16, 1) on the LCD for Player 2
    lcd.setCursor(10, 1);
    lcd.print("P2P:");
    lcd.print(player2.playerPoints);
}


//printing winner of the matches
void Hardware::printLCDWinner(Rules& rules) {
    lcd.clear();
    lcd.setCursor(3, 0);
    if (rules.matchWinner == PLAYER1) {
        lcd.print("Player 1");
    }
    else {
        lcd.print("Player 2");
    }
    lcd.setCursor(5, 1);
    lcd.print("Wins");

    for (int idx = 0; idx <= 3; idx++) {
        matchBlinker("**");
        matchBlinker("  ");
    }
}

//blinker for who wins the match
void Hardware::matchBlinker(char* text) {
    lcd.setCursor(1, 0);
    lcd.print(text);
    lcd.setCursor(11, 0);
    lcd.print(text);
    delay(500);
}

//switching off led's
void Hardware::ledClear() {
    digitalWrite(LED_PLAYER1, LOW);
    digitalWrite(LED_PLAYER2, LOW);
}

void Hardware::begin() {
    lcd.begin(16, 2);
}

void Hardware::ledForSetWinner(Rules& rules) {
    //winning rules for who won and who didn't
    if (rules.setWinner == PLAYER1) {
        digitalWrite(LED_PLAYER2, LOW);
        blinkWinner(LED_PLAYER1);
        rules.setWinner = NONE;
    }
    else if (rules.setWinner == PLAYER2) {
        digitalWrite(LED_PLAYER1, LOW);
        blinkWinner(LED_PLAYER2);
        rules.setWinner = NONE;
    }
}

//reducing repetition
void Hardware::blinkWinner(int ledPlayer) {
    digitalWrite(ledPlayer, LOW);
    for (int idx = 0; idx <= 3; idx++) {
        digitalWrite(ledPlayer, HIGH);
        delay(500);
        digitalWrite(ledPlayer, LOW);
        delay(500);
    }
}

//resetting the game after somebody wins.
void Hardware::resetMatch(Player& player1, Player& player2, Rules& rules) {
    player1.playerPoints = 0;
    player2.playerPoints = 0;
    player1.playerSetPoints = 0;
    player2.playerSetPoints = 0;
    rules.isServing = PLAYER1;
    rules.setWinner = NONE;
    rules.matchWinner = NONE;
}



