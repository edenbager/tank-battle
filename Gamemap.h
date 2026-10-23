#pragma once
// GameMap.h - this is the map where tanks fight with walls and stuff
#pragma once
#include "constants.h"

class GameMap {
private:
    char map[HEIGHT][WIDTH];        // what is in each place on map
    int wallHealth[HEIGHT][WIDTH];  // how strong each wall is
    bool isBoundary[HEIGHT][WIDTH]; // if its edge of map
    bool useColor;                  // if we want colors or not

public:
    // makes empty map when game starts
    GameMap();

    // puts walls and mines on map at start of game
    void initialize();

    // says if we want colors or not
    void setColorMode(bool colored);

    // shows the map on screen
    void draw() const;

    // tells what is at position on map
    char getAt(int y, int x) const;

    // checks if something is map edge
    bool isBoundaryWall(int y, int x) const;

    // puts something on map at position
    void setAt(int y, int x, char c);

    // when bullet hits wall, makes wall weaker
    bool damageWall(int y, int x);

    // Places a wall at (x,y) with full health
    void setWall(int x, int y);

    // Places a mine at (x,y)
    void setMine(int x, int y);

    // Clears whatever was at (x,y), making it empty
    void setEmpty(int x, int y);

    // Returns true if (x,y) is in bounds and not a wall or mine
    bool isEmpty(int x, int y) const;

};