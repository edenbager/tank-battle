#include "PauseUtils.h"  // include pause utils header file
#include "ConsoleUtility.h"  // include console functions for cursor movement
#include "constants.h"  // include constants like WIDTH and HEIGHT
#include <iostream>  // include iostream for cout
#include <string>  // include string class for building messages

using namespace std;  // use std namespace for cout and string

void showPauseMessage() {  // function to show pause message on screen
    ConsoleUtility::gotoxy(10, 0);  // move cursor to column 10, row 0
    cout << "Game paused, press ESC again to continue or X to go back to the main menu";  // print pause instructions
}

void clearPauseMessage() {  // function to clear pause message from screen
    const string msg =  // define the same message as string
        "Game paused, press ESC again to continue or X to go back to the main menu";
    const int startX = 10;  // starting x position of message
    const int totalLength = startX + static_cast<int>(msg.length());  // total chars until end of message

    // how many rows message takes
    int rows = totalLength / WIDTH + 1;  // divide by WIDTH to get number of rows used

    // clean all rows where message could be
    for (int r = 0; r < rows && r < HEIGHT; ++r) {  // loop over each row up to HEIGHT
        ConsoleUtility::gotoxy(0, r);  // move cursor to start of row r
        cout << string(WIDTH, ' ');  // print spaces across entire row to clear
    }

    // move cursor back to start
    ConsoleUtility::gotoxy(0, 0);  // reset cursor to origin
}
