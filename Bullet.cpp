#include "Bullet.h"  // include bullet header file
#include "ConsoleUtility.h"  // include console utility for cursor and color
#include "constants.h"  // include game constants like WIDTH and HEIGHT
#include <iostream>  // include iostream for cout

using namespace std;  // use std namespace

Bullet::Bullet() : x(0), y(0), dx(0), dy(0), active(false), directionIdx(0) {}  // constructor init all values to zero or false

void Bullet::fire(int startX, int startY, int direction) {  // function to fire bullet from start pos
    x = startX;  // set bullet x pos
    y = startY;  // set bullet y pos
    directionIdx = direction;  // store direction index

    // array that tells which way bullet goes
    const int dirX[] = { 0, 1, 1, 1, 0, -1, -1, -1 };  // x deltas per direction
    const int dirY[] = { -1, -1, 0, 1, 1, 1, 0, -1 };  // y deltas per direction

    dx = dirX[directionIdx];  // set dx from array
    dy = dirY[directionIdx];  // set dy from array

    active = true;  // mark bullet active

    ConsoleUtility::gotoxy(x, y);  // move cursor to bullet pos
    cout << '*';  // draw bullet symbol
}

void Bullet::move() {  // function move bullet one step
    if (!active) return;  // if bullet not active, skip

    // clean old position
    ConsoleUtility::gotoxy(x, y);  // go to old pos
    cout << ' ';  // print space to erase

    // move to new position
    x += dx;  // update x
    y += dy;  // update y
}

void Bullet::draw() const {  // function draw bullet at current pos
    if (!active) return;  // skip if not active

    ConsoleUtility::gotoxy(x, y);  // move cursor to pos
    cout << '*';  // print bullet symbol
}

void Bullet::deactivate() {  // function to deactivate bullet
    active = false;  // set active false
}

int Bullet::getX() const { return x; }  // get bullet x pos
int Bullet::getY() const { return y; }  // get bullet y pos
bool Bullet::isActive() const { return active; }  // return active flag
int Bullet::getDirectionIdx() const { return directionIdx; }  // get direction index
