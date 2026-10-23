#pragma once
#include <windows.h>

class ConsoleUtility {
private:
    static HANDLE hConsole;

public:
    // starts the console stuff
    static void initialize();

    // changes color of text we print
    static void setColor(int color);

    // moves cursor to place we want to print at
    static void gotoxy(int x, int y);

    // makes cursor invisible so game looks better
    static void hideCursor();

    // makes console window the size we want
    static void setConsoleSize(int w, int h);

    // cleans everything from screen
    static void clearScreen();

    // cleans just one character from screen at position x,y
    static void cleanCharAt(int x, int y);
}; 
