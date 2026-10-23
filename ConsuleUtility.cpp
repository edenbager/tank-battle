#include "ConsuleUtility.h"  // include console utility header
#include <iostream>  // include iostream for cout

using namespace std;  // use std namespace for functions and types

// start with console not initialized
HANDLE ConsoleUtility::hConsole = NULL;  // static handle for console

void ConsoleUtility::initialize() {  // function initialize console handle
    if (!hConsole) {  // if console handle is null
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // get standard output handle
    }
}

void ConsoleUtility::setColor(int color) {  // function set text color by code
    initialize();  // ensure console handle ready
    SetConsoleTextAttribute(hConsole, color);  // set console text attribute
}

void ConsoleUtility::gotoxy(int x, int y) {  // function move cursor to pos
    initialize();  // ensure console handle ready
    COORD pos;  // COORD struct for position
    pos.X = (SHORT)x;  // set X coord
    pos.Y = (SHORT)y;  // set Y coord
    SetConsoleCursorPosition(hConsole, pos);  // move cursor
}

void ConsoleUtility::hideCursor() {  // function hide console cursor
    initialize();  // ensure console handle ready
    CONSOLE_CURSOR_INFO cursorInfo;  // struct for cursor info
    GetConsoleCursorInfo(hConsole, &cursorInfo);  // get current cursor info
    cursorInfo.bVisible = FALSE;  // set cursor not visible
    SetConsoleCursorInfo(hConsole, &cursorInfo);  // apply new cursor info
}

void ConsoleUtility::setConsoleSize(int w, int h) {  // function set console window size
    initialize();  // ensure console handle ready
    COORD coord;  // buffer size struct
    coord.X = (SHORT)w;  // set width
    coord.Y = (SHORT)h;  // set height
    SetConsoleScreenBufferSize(hConsole, coord);  // change buffer size

    SMALL_RECT rect;  // window rect struct
    rect.Top = 0;  // top row
    rect.Left = 0;  // left col
    rect.Bottom = (SHORT)(h - 1);  // bottom row
    rect.Right = (SHORT)(w - 1);  // right colomn
    SetConsoleWindowInfo(hConsole, TRUE, &rect);  // apply window size
}

void ConsoleUtility::clearScreen() {  // function clear whole screen
    system("cls");  // call system cls command
}

void ConsoleUtility::cleanCharAt(int x, int y) {  // function clear single char at pos
    gotoxy(x, y);  // move cursor to pos
    cout << ' ';  // print space to erase char
}
