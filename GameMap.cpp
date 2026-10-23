#include "GameMap.h"  // include header for GameMap class
#include "ConsoleUtility.h"  // include console utility for cursor control
#include <iostream>  // include iostream for cout

using namespace std;  // use std namespace for cout

GameMap::GameMap() : useColor(false) {  // constructor init useColor false
    // make empty map at start
    for (int i = 0; i < HEIGHT; i++) {  // for each row
        for (int j = 0; j < WIDTH; j++) {  // for each col
            map[i][j] = ' ';  // set cell to empty space
            wallHealth[i][j] = 0;  // no wall health
            isBoundary[i][j] = false;  // not boundary
        }
    }
}

void GameMap::initialize() {  // init map fresh
    // clean everything first
    for (int i = 0; i < HEIGHT; i++) {  // for each row
        for (int j = 0; j < WIDTH; j++) {  // for each col
            map[i][j] = ' ';  // clear cell
            wallHealth[i][j] = 0;  // reset health
            isBoundary[i][j] = false;  // reset boundary flag
        }
    }

    // make walls on edges
    for (int x = 0; x < WIDTH; x++) {  // top and bottom edges
        map[0][x] = '#';  // edge is drawn as a wall
        map[HEIGHT - 1][x] = '#';
        wallHealth[0][x] = -1;  // boundary health -1 means indestructible
        wallHealth[HEIGHT - 1][x] = -1;
        isBoundary[0][x] = true;  // mark edge boundary
        isBoundary[HEIGHT - 1][x] = true;
    }
    for (int y = 0; y < HEIGHT; y++) {  // left and right edges
        map[y][0] = '#';
        map[y][WIDTH - 1] = '#';
        wallHealth[y][0] = -1;
        wallHealth[y][WIDTH - 1] = -1;
        isBoundary[y][0] = true;
        isBoundary[y][WIDTH - 1] = true;
    }

    // make walls inside map (plus shape)
    for (int y = 5; y < 20; y++) {  // vertical bar of plus
        map[y][40] = '#';  // set wall char
        wallHealth[y][40] = 2;  // two hits to break
    }
    for (int x = 15; x < 66; x++) {  // horizontal bar of plus
        map[12][x] = '#';
        wallHealth[12][x] = 2;
    }

    // put mines on map
    map[10][20] = '@';  // mine char at pos
    map[10][60] = '@';
    map[15][20] = '@';
    map[15][60] = '@';
}

void GameMap::setColorMode(bool colored) {  // set color mode flag
    useColor = colored;  // store colored choice
}

void GameMap::draw() const {  // draw map on screen
    for (int y = 0; y < HEIGHT; y++) {  // each row
        ConsoleUtility::gotoxy(0, y);  // move cursor start of row
        for (int x = 0; x < WIDTH; x++) {  // each col
            if (useColor && map[y][x] == '@') {  // if color mode and mine
                ConsoleUtility::setColor(COLOR_RED);  // set red
                cout << map[y][x];  // print mine
                ConsoleUtility::setColor(COLOR_WHITE);  // reset white
            }
            else {  // normal char
                cout << map[y][x];  // print map cell
            }
        }
    }
}

char GameMap::getAt(int y, int x) const {  // get map char at pos
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {  // in bounds
        if (isBoundary[y][x]) {  // if boundary cell
            return '#';  // return wall char
        }
        return map[y][x];  // return stored char
    }
    return '#';  // out of bounds treat as wall
}

bool GameMap::isBoundaryWall(int y, int x) const {  // check if boundary
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {  // in bounds
        return isBoundary[y][x];  // return flag
    }
    return false;  // out of bounds not boundary
}

void GameMap::setAt(int y, int x, char c) {  // set map char at pos
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {  // in bounds
        map[y][x] = c;  // store char
        if (c == ' ') {  // if empty char
            isBoundary[y][x] = false;  // clear boundary flag
        }
    }
}

bool GameMap::damageWall(int y, int x) {  // hit wall or mine at pos
    if (getAt(y, x) != '#' && getAt(y, x) != '!') return false;  // not destructible wall
    if (wallHealth[y][x] == -1) return true;  // boundary always true but unchanged

    wallHealth[y][x]--;  // reduce health

    if (wallHealth[y][x] == 1) {  // if one hit left
        map[y][x] = '!';  // mark damaged wall
        ConsoleUtility::gotoxy(x, y);  // go to cell
        cout << '!';  // draw damaged
    }
    else if (wallHealth[y][x] <= 0) {  // if broken
        map[y][x] = ' ';  // clear map cell
        ConsoleUtility::gotoxy(x, y);  // go to cell
        cout << ' ';  // erase char
    }

    return true;  // wall damage processed
}

// --- place a two-hit wall at (x,y) ---
void GameMap::setWall(int x, int y) {  // place wall
    if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) return;  // out of bounds
    map[y][x] = '#';  // wall char
    wallHealth[y][x] = 2;  // two hits
    isBoundary[y][x] = false;  // not boundary
}

// --- place a mine at (x,y) ---
void GameMap::setMine(int x, int y) {  // place mine
    if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) return;  // out of bounds
    map[y][x] = '@';  // mine char
    wallHealth[y][x] = 0;  // no health
    isBoundary[y][x] = false;  // not boundary
}

// --- clear (x,y) to empty ---
void GameMap::setEmpty(int x, int y) {  // clear cell
    if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) return;  // out of bounds
    map[y][x] = ' ';  // empty char
    wallHealth[y][x] = 0;  // reset health
    isBoundary[y][x] = false;  // clear boundary
}

// --- return true if (x,y) is in-bounds, not boundary, and contains no wall/mine ---
bool GameMap::isEmpty(int x, int y) const {  // check empty spot
    if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) return false;  // out of bounds
    if (isBoundary[y][x])                return false;  // boundary not empty
    return (map[y][x] == ' ');  // true if space char
}
