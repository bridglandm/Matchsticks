#include "gameLib.h"
int main(){
/*typedef struct game{
    bool userTurn, humanOpponent;
    int rowsLeft, movesMade;
    int remainingMatches[3];
} gameSet;  */  gameSet match = {true, NULL, 3, 0, {3, 5, 7}};
    runGame(match);
}

/*
    Bug report

    Title screen:
        - entering other characters before hitting enter
            scans them as opponent declaration.
        - Entering characters after an 'n' or 'y' causes a
            crash when it tries scanning an int.

        = If we return to the previous prompts and enter the
            proper information, the program runs correctly.


    (Human opponent - 'y')
    Player moves:
    - Selecting a row that's out of bounds:
        - returns the appropriate prompts but does not update
            correctly with that information.
        - Also instils a PC opponent for the rest of the match!!

        = Only one board update gets discarded.
        =* If this board update would win the game, this gets
            checked and the game ends as expected.

    Play again:
    - This was a late addition, and doesn't work properly yet.
    
    Enter choice (y/n) and press retrun twice to get it to accept or close the program.
    (Somewhere at the conclusion there was a struct / game state that was still waiting on an input. )
*/
