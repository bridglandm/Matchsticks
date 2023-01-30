#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <ctype.h>

typedef struct game{
    bool userTurn, humanOpponent;
    int rowsLeft, movesMade;
    int remainingMatches[3];
} gameSet;

/// TODO: Fix bugs in the userMove() and playAgain() functions.
///
/// Invalid entries cause corrupt updates from the re-prompts.
/// Player opponent declaration has hiccups.

/// Scanning chars on int inputs cause program failure. (isalpha()?)
///     It looks as though ints can be stored as chars (then checked),
///      but chars don't fit in the assigned space for an int.
///      Could we force a fix with malloc()?

///Supporting Functions
int getLine(gameSet board, int row){
    return board.remainingMatches[row];
}

int getMatches(gameSet board){
    int leftToTake;
    leftToTake = getLine(board, 2) + getLine(board, 1) + getLine(board, 0);
    return leftToTake;
}

void printBoard(gameSet board){
    //Determines matches remaining
    int top = getLine(board, 0);
    int mid = getLine(board, 1);
    int bot = getLine(board, 2);

    //Prints each line
    printf("      ");
    while(top > 0){
        printf(" | ");
        top--;
    }
    printf("\n");

    printf("   ");
    while(mid > 0){
        printf(" | ");
        mid--;
    }
    printf("\n");

    while(bot > 0){
        printf(" | ");
        bot--;
    }
    printf("\n");

    return;
}

///Validation on this method is non-existant. This is the programs largest (identified) liability.
gameSet update(gameSet board, int row, int remove){
    int remaining = getLine(board, row);
    int mod = (board.rowsLeft % 3) % 2 ;
    //By counting down modulo operations we can be sure rowsLeft is equal to 1... or returning 0.

    remaining -= mod;   //line contents, less one if it's the last row.

    if(remove==remaining){
        board.rowsLeft--;
    }

    int left = remaining - remove + mod + mod;
    board.remainingMatches[row] = left;
    board.movesMade++;

    if(board.userTurn==false){
        printf("%d matches taken from row %d\n", remove, row+1);
    }


    int turn = board.movesMade;

    //printf("There's %d pieces left on row %d\t", left, row+1);
    //printf("And this is the last row : %d\n", mod);

    int matchEnd = getMatches(board);
    printBoard(board);
    if(matchEnd == 1){
        if(board.humanOpponent==true){
            int turn = board.movesMade;
            int player = (turn+1)%2 + 1;
            printf("Player %d wins! Good job!\n", player);
        }

        if(board.humanOpponent==false){
            if(board.userTurn){
                printf("You win! Congratulations!\n");
            } else {
                printf("Sorry! Better luck next time!\n");
            }
        }

        printf("Game ran in %d turns.\n", turn);
        playAgain(board);
        //exit(0);
    }
    return board;
}

gameSet userMove(gameSet board){
    int row;
    int exists;
    int turn = board.movesMade;
    int player = turn%2 + 1;
    bool lastRow = false;
    bool valid = false;

    if(board.humanOpponent==true){
        printf("Player %d's turn:\n", player);
    }

    printf("From which row will you take away pieces?\t");
    scanf("%d", &row);
    row--;      //transposes range from (1 to 3) to (0 to 2)

    exists = getLine(board, row);

    if(row >= 3 || row <= -1){
        printf("Sorry, the row %d doesn't exist.\n", row+1);
        userMove(board);            /// for player opponent
        //return board;    //else -> player [unchanged]'s turn:
                        //        From which row ...?
                        //        How many pieces ...?
                        //          correct printBoard()
                        //        ! How many pieces...?
    }

    if(exists < 1){
        printf("Sorry, that row has already been emptied.\n");
        userMove(board);            /// for player opponent
        return board;   //else -> player [unchanged]'s turn:
                        //        From which row ...?
                        //        How many pieces ...?
                        //          correct printBoard()
                        //        ! How many pieces...?

        /// However, *with* the return statement... (and PC opponent)
            // ''       ''  -> correct printBoard()
            // PCs turn: \n  Player 1's turn: \n
            // From which row ... ?
            // How many pieces ... ?
            // printBoard() minus the last move.


    }

    while(valid==false){
        int num;
        printf("How many pieces will you take away?\t   ");
        scanf("%d", &num);

        if(num < 1){
            printf("Must be a positive number\n");
            userMove(board);
            //return board;
            /// It looks at though when the function gets called again, the previous
            /// iteration continues to run. We can either return here -> no board updates,
            /// or let it run, giving interwoven inconsistent outputs.
        }

        if(board.rowsLeft == 1){
            lastRow = true;
            num++;
        }

        if(exists - num < 0)
            printf("Not enough pieces\n");
        else{
            board = update(board, row, num);
            if(board.humanOpponent == false){
                board.userTurn = false;
                printf("PCs turn:\n");
            }
            valid = true;
            return board;
        }
    }
}

gameSet autoMove(gameSet board){
    int top = getLine(board, 0);
    int mid = getLine(board, 1);
    int bot = getLine(board, 2);
    int rows = board.rowsLeft;

    if(rows == 1){
        //final and winning move to clear all but one of the last row,
        //  leaving one piece to the opponent.
        if(top!=0){
            board = update(board, 0, top-1);
        }
        if(mid!=0){
            board = update(board, 1, mid-1);
        }
        if(bot!=0){
            board = update(board, 2, bot-1);
        }
    }//end case: single line

    if(rows == 2){
        //by matching user moves, we force an eventual winning scenario where
        // a) user clears a line -> we go to the former case.
        // b) user leaves one line with one -> we clear the other.
        if(top==1 || mid==1 || bot==1){
            if(top>1){  board = update(board, 0, top);  }
            if(mid>1){  board = update(board, 1, mid);  }
            if(bot>1){  board = update(board, 2, bot);  }
        } else {
            if(top==0){
                if(mid<bot){
                    board = update(board, 2, bot-mid);
                }
                else if(bot<mid){
                    board = update(board, 1, mid-bot);
                } else {
                    board = update(board, 2, 1); //user can force a win.
                }
            }

            if(mid==0){
                if(top<bot){
                    board = update(board, 2, bot-top);
                }
                else if(bot<top){
                    board = update(board, 0, top-bot);
                } else {
                    board = update(board, 2, 1);
                }
            }

            if(bot==0){
                if(top<mid){
                    board = update(board, 1, mid-top);
                }
                else if(mid<top){
                    board = update(board, 0, top-mid);
                } else {
                    board = update(board, 1, 1);
                }
            }
        }
    }//end case: double line

    if(rows==3){    ///IMPROVE ON THIS!!
        int turn = board.movesMade;
        int left = getMatches(board);
        board = update(board, (turn%3), 1);
    }

    board.userTurn = true;
    return board;
}

bool getOpponent(){
    printf("Welcome to MatchSticks! Hit enter to begin!\n");
    char in;
    scanf("%c", &in);
    printf("Do you have a partner?\n");
    printf("(enter 'y' for yes, 'n' for no)\t");
    scanf("%c", &in);
    //printf("You've selected %c\n", in);

    if(in != 'y' && in != 'n' && in != 'Y' && in != 'N'){
        printf("Sorry, %c that wasn't one of the accepted responses.\n", in);
        scanf("%c");    //picks up the empty character from the first response
        getOpponent();
    }

    if(in == 'y' || in == 'Y'){
        return true;
    }

    if(in == 'n' || in == 'N'){
        return false;
    }
}

gameSet runGame(gameSet board){
    if(board.humanOpponent==NULL){
        board.humanOpponent = getOpponent();
    }
    printBoard(board);

    while(true){
        if(board.userTurn == true){
            board = userMove(board);
        }
        if(board.userTurn == false){
            board = autoMove(board);
        }
    }
}

void playAgain(gameSet board){
    printf("Would you like to play again?\t");
    char in;
    scanf("%c");
    scanf("%c", &in);

    if(in != 'y' && in != 'n' && in != 'Y' && in != 'N'){
        printf("Sorry, %c that wasn't one of the accepted responses, hit 'y' or 'n'.\n", in);
        scanf("%c");    //picks up the empty character from the first response
        scanf("%c");
        playAgain(board);
    }

    if(in == 'y' || in == 'Y'){
        bool hOp = board.humanOpponent;
        gameSet match = {true, hOp, 3, 0, {3, 5, 7}};
        runGame(match);
    }

    if(in == 'n' || in == 'N'){
        printf("Alright, thanks for playing!");
        exit(0);
    }
}
