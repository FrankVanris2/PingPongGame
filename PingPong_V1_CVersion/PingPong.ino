/*
 Name:		PingPong.ino
 Created:	2/23/2023 6:00:37 PM
 Author:	Frank Vanris
 Class:     CS351
 Desc: Created a pong game scoreboard that tallies up the points for either player and determines who
 wins a game.

 P.S: I wanted to do more with this project, but I got sick and that didn't help me in anyway. However
 I plan on continuing with this project on the side to see what else I could add. Updates will be posted when
 ready.
*/

// the setup function runs once when you press reset or power the board
#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define NONE -1

#define PLAYER1 0
#define PLAYER2 1

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

#define WINNING_SET 5


//***()***()***()***()***()***()PLAYER STRUCTURE()***()***()***()***()***()***()


bool prevHigh[4] = {false, false, false, false};

struct Player {
    
    //player point and set points
    int playerPoints = 0;
    int playerSetPoints = 0;

};
//***()***()***()***()***()***()PLAYER STRUCTURE()***()***()***()***()***()***()



//***()***()***()***()***()***()RULES STRUCTURE()***()***()***()***()***()***()
struct Rules {
    int isServing = PLAYER1;
    int setWinner = NONE;
    int matchWinner = NONE;
};
//***()***()***()***()***()***()RULES STRUCTURE()***()***()***()***()***()***()

//struct conventions
struct Player player1;
struct Player player2;
struct Rules rules;

//methods for incrementing and decrementing the scores for both players
bool checkButtonStatus(bool buttons[]);

bool checkHighForButton(int isHIGH, int buttonID);

//prints out the points and the set points for the lcd Screen.
void printLCD();

void lightLEDForPlayer();

void setup() {
    //Player One Increment and Decrement
    pinMode(PLAYER1_INCREMENT_BUTTON_CONNECTION, INPUT);
    pinMode(PLAYER1_DECREMENT_BUTTON_CONNECTION, INPUT);

    //Player Two Increment and Decrement
    pinMode(PLAYER2_INCREMENT_BUTTON_CONNECTION, INPUT);
    pinMode(PLAYER2_DECREMENT_BUTTON_CONNECTION, INPUT);

    //Player LED light up
    pinMode(LED_PLAYER1, OUTPUT);
    pinMode(LED_PLAYER2, OUTPUT);


    //size of the LCD screen
    lcd.begin(16, 2);

    //printing the scores on the screen
    printLCD();

    //lighting up the LED for the player that starts the serve
    lightLEDForPlayer();
}



// the loop function runs over and over again until power down or reset
void loop() {

    //setting buttons to false since each one is LOW at first until pressed to HIGH
    bool buttons[4] = { false, false, false, false };

    //setting the buttons to read its values, and stop it from incrementing when held down.
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
            ledClear();
            printLCDWinner();
            resetMatch();
            printLCD();
            lightLEDForPlayer();
        }

        //if there is a set winner
        else if (rules.setWinner != NONE) {
            printLCD();
            ledForSetWinner();
            lightLEDForPlayer();

            //otherwise continue
        } else {
            printLCD();
            lightLEDForPlayer();
            delay(250);
        }
    }
    
}


//checking whether the button is pressed if pressed or held the button does not keep incrementing
bool checkHighForButton(int isHigh, int buttonId) {
    if (isHigh != prevHigh[buttonId]) {
        prevHigh[buttonId] = isHigh;
        if (isHigh)
            return true;
    }

    return false;
}

//states of each button for when it is pressed.
bool checkButtonStatus(Player* player, bool incrButton, bool decrButton) {
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

//handle rule method where all the rules take place in how the scoreboard for the pong game works
void handleRules(Player* player1, Player* player2) {

    //if player1 or player2 wins
    if((player1->playerPoints > 10 || player2->playerPoints > 10) && abs(player1->playerPoints - player2->playerPoints) >= 2) {

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
        if ((player1->playerPoints >= 10 && player2->playerPoints >= 10)) {
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


//printing score
void printLCD() {
    lcd.clear();
    //printing the score on the screen at (0, 0) on the LCD for Player 1
    lcd.setCursor(0, 0);
    lcd.print("P1SP:");
    lcd.print(player1.playerSetPoints);

    //printing the points on the screen at (0, 1) on the LCD for Player 1
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
void printLCDWinner() {
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
void matchBlinker(char* text) {
    lcd.setCursor(1, 0);
    lcd.print(text);
    lcd.setCursor(11, 0);
    lcd.print(text);
    delay(500);
}


//lighting up the LED for the Player that starts the serve
void lightLEDForPlayer() {

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

//switching off led's
void ledClear() {
    digitalWrite(LED_PLAYER1, LOW);
    digitalWrite(LED_PLAYER2, LOW);
}

//set winner of a set
void ledForSetWinner() {
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
void blinkWinner(int ledPlayer) {
    digitalWrite(ledPlayer, LOW);
    for (int idx = 0; idx <= 3; idx++) {
        digitalWrite(ledPlayer, HIGH);
        delay(500);
        digitalWrite(ledPlayer, LOW);
        delay(500);
    }
}

//resetting the game after somebody wins.
void resetMatch() {
    player1.playerPoints = 0;
    player2.playerPoints = 0;
    player1.playerSetPoints = 0;
    player2.playerSetPoints = 0;
    rules.isServing = PLAYER1;
    rules.setWinner = NONE;
    rules.matchWinner = NONE;
}





