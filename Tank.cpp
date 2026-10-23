#include "Tank.h"
#include "ConsoleUtility.h"
#include <iostream>

using namespace std;

// true if any tank in the list stands at (px, py)
static bool isOccupied(const Positions& tanks, int px, int py) {
    for (const auto& p : tanks)
        if (p.first == px && p.second == py) return true;
    return false;
}

Tank::Tank(int startX, int startY, int startDir, char tankSymbol, bool colored,
    int tColor, int cColor)
    :x(startX), y(startY), directionIdx(startDir),
    drawnCannonDirIdx(startDir),
    symbol(tankSymbol),
    framesSinceLastShot(FRAMES_BETWEEN_SHOTS),
    leftTrackForward(false), leftTrackBackward(false),
    rightTrackForward(false), rightTrackBackward(false),
    lastCannonX(startX), lastCannonY(startY),
    lastTankX(startX), lastTankY(startY),
    tankColor(tColor), cannonColor(cColor), useColor(colored),
    hasCanon(true) { // tank starts with a working cannon
}

void Tank::draw() {
    // clean old positions
    if (lastTankX >= 0 && lastTankX < WIDTH && lastTankY >= 0 && lastTankY < HEIGHT) {
        ConsoleUtility::gotoxy(lastTankX, lastTankY);
        ConsoleUtility::setColor(COLOR_WHITE);
        cout << ' ';
    }
    if (lastCannonX >= 0 && lastCannonX < WIDTH && lastCannonY >= 0 && lastCannonY < HEIGHT) {
        ConsoleUtility::gotoxy(lastCannonX, lastCannonY);
        ConsoleUtility::setColor(COLOR_WHITE);
        cout << ' ';
    }

    // calculate where cannon should be (use the "frozen" draw direction)
    int cannonX = x, cannonY = y;
    char cannonChar = ' ';
    switch (drawnCannonDirIdx) {     // <— was directionIdx
    case 0: cannonY--; cannonChar = '|'; break;
    case 1: cannonX++; cannonY--; cannonChar = '/'; break;
    case 2: cannonX++; cannonChar = '-'; break;
    case 3: cannonX++; cannonY++; cannonChar = '\\'; break;
    case 4: cannonY++; cannonChar = '|'; break;
    case 5: cannonX--; cannonY++; cannonChar = '/'; break;
    case 6: cannonX--; cannonChar = '-'; break;
    case 7: cannonX--; cannonY--; cannonChar = '\\'; break;
    }

    // draw new cannon only if the tank still has a cannon
    if (hasCanon && cannonX >= 0 && cannonX < WIDTH && cannonY >= 0 && cannonY < HEIGHT) {
        ConsoleUtility::gotoxy(cannonX, cannonY);
        if (useColor) ConsoleUtility::setColor(cannonColor);
        cout << cannonChar;
        lastCannonX = cannonX;
        lastCannonY = cannonY;
    }
    else {
        // no cannon: still remember its last position so it gets erased next time
        lastCannonX = cannonX;
        lastCannonY = cannonY;
    }

    // draw tank body
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        ConsoleUtility::gotoxy(x, y);
        if (useColor) ConsoleUtility::setColor(tankColor);
        cout << symbol;
        lastTankX = x;
        lastTankY = y;
    }

    // reset color back
    if (useColor) ConsoleUtility::setColor(COLOR_WHITE);
}

void Tank::erase() const {
    if (lastTankX >= 0 && lastTankX < WIDTH && lastTankY >= 0 && lastTankY < HEIGHT) {
        ConsoleUtility::gotoxy(lastTankX, lastTankY);
        cout << ' ';
    }
    if (lastCannonX >= 0 && lastCannonX < WIDTH && lastCannonY >= 0 && lastCannonY < HEIGHT) {
        ConsoleUtility::gotoxy(lastCannonX, lastCannonY);
        cout << ' ';
    }
}

void Tank::setLeftTrack(bool forward, bool backward) {
    leftTrackForward = forward;
    leftTrackBackward = backward;
}

void Tank::setRightTrack(bool forward, bool backward) {
    rightTrackForward = forward;
    rightTrackBackward = backward;
}

void Tank::stopMoving() {
    leftTrackForward = false;
    leftTrackBackward = false;
    rightTrackForward = false;
    rightTrackBackward = false;
}

bool Tank::isMoving() const {
    return leftTrackForward || leftTrackBackward || rightTrackForward || rightTrackBackward;
}

bool Tank::updateMovement(GameMap& gameMap, const Positions& otherTanks) {
    if (!isMoving()) return true;

    int newX = x;
    int newY = y;
    int newDirIdx = directionIdx;
    bool rotateOnly = false;

    // handle different track combinations
    if (rightTrackForward && !leftTrackForward && !rightTrackBackward && !leftTrackBackward) {
        rotateOnly = true;
        newDirIdx = (directionIdx + 7) % 8;
    }
    else if (rightTrackForward && !leftTrackForward && !rightTrackBackward && leftTrackBackward) {
        rotateOnly = true;
        newDirIdx = (directionIdx + 6) % 8;
        if (newDirIdx % 2 == 1) {
            newDirIdx = (newDirIdx + 7) % 8;
        }
    }
    else if (leftTrackForward && !rightTrackForward && !leftTrackBackward && !rightTrackBackward) {
        rotateOnly = true;
        newDirIdx = (directionIdx + 1) % 8;
    }
    else if (leftTrackForward && !rightTrackForward && leftTrackBackward && rightTrackBackward) {
        rotateOnly = true;
        newDirIdx = (directionIdx + 2) % 8;
        if (newDirIdx % 2 == 1) {
            newDirIdx = (newDirIdx + 1) % 8;
        }
    }
    else if (leftTrackForward && rightTrackForward && !leftTrackBackward && !rightTrackBackward) {
        const int moveDirX[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        const int moveDirY[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
        newX += moveDirX[directionIdx];
        newY += moveDirY[directionIdx];
    }
    else if (!leftTrackForward && !rightTrackForward && leftTrackBackward && rightTrackBackward) {
        const int moveDirX[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        const int moveDirY[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
        int oppositeDir = (directionIdx + 4) % 8;
        newX += moveDirX[oppositeDir];
        newY += moveDirY[oppositeDir];
    }

    if (rotateOnly) {
        directionIdx = newDirIdx;
        drawnCannonDirIdx = newDirIdx;
        erase();
        draw();
        return true;
    }

    // ——— block movement at map edges ———
    if (newX <= 0 || newX >= WIDTH - 1 ||
        newY <= 0 || newY >= HEIGHT - 1)
    {
        stopMoving();
        return false;
    }

    // calculate where cannon should be
    int cannonX = newX;
    int cannonY = newY;
    switch (directionIdx) {
    case 0: cannonY--;             break;
    case 1: cannonX++; cannonY--; break;
    case 2: cannonX++;             break;
    case 3: cannonX++; cannonY++; break;
    case 4: cannonY++;             break;
    case 5: cannonX--; cannonY++; break;
    case 6: cannonX--;             break;
    case 7: cannonX--; cannonY--; break;
    }

    // ——— block cannon at map edges ———
    if (hasCanon && (cannonX <= 0 || cannonX >= WIDTH - 1 ||
        cannonY <= 0 || cannonY >= HEIGHT - 1))
    {
        stopMoving();
        return false;
    }

    // check if we hit anything
    if ((gameMap.getAt(newY, newX) == '#' ||
        gameMap.getAt(newY, newX) == '!' ||
        isOccupied(otherTanks, newX, newY)) ||
        (hasCanon && (gameMap.getAt(cannonY, cannonX) == '#' ||
            gameMap.getAt(cannonY, cannonX) == '!' ||
            isOccupied(otherTanks, cannonX, cannonY)))) {
        stopMoving();
        return false;
    }

    erase();
    x = newX;
    y = newY;
    draw();

    if (gameMap.getAt(y, x) == '@') {
        stopMoving();
        return false;
    }

    return true;
}

// check if a bullet hit the tank body
bool Tank::isBodyHit(int bx, int by) const {
    return bx == x && by == y;
}

// check if a bullet hit the tank cannon
bool Tank::isCannonHit(int bx, int by) const {
    if (!hasCanon) return false; // no cannon, nothing to hit

    int cannonX = x, cannonY = y;
    switch (directionIdx) {
    case 0: cannonY--; break;
    case 1: cannonX++; cannonY--; break;
    case 2: cannonX++; break;
    case 3: cannonX++; cannonY++; break;
    case 4: cannonY++; break;
    case 5: cannonX--; cannonY++; break;
    case 6: cannonX--; break;
    case 7: cannonX--; cannonY--; break;
    }

    return (bx == cannonX && by == cannonY);
}

// break the tank's cannon
void Tank::breakCannon() {
    hasCanon = false;
    // erase the cannon from the screen
    if (lastCannonX >= 0 && lastCannonX < WIDTH && lastCannonY >= 0 && lastCannonY < HEIGHT) {
        ConsoleUtility::gotoxy(lastCannonX, lastCannonY);
        cout << ' ';
    }
}

bool Tank::fire(GameMap& gameMap, const Positions& enemies) {
    if (framesSinceLastShot < FRAMES_BETWEEN_SHOTS || !hasCanon) {
        return false; // can't shoot without a cannon (or still reloading)
    }

    // calculate where cannon is
    int cannonX = x;
    int cannonY = y;
    char cannonChar = ' ';

    switch (directionIdx) {
    case 0: cannonY--; cannonChar = '|'; break;
    case 1: cannonX++; cannonY--; cannonChar = '/'; break;
    case 2: cannonX++; cannonChar = '-'; break;
    case 3: cannonX++; cannonY++; cannonChar = '\\'; break;
    case 4: cannonY++; cannonChar = '|'; break;
    case 5: cannonX--; cannonY++; cannonChar = '/'; break;
    case 6: cannonX--; cannonChar = '-'; break;
    case 7: cannonX--; cannonY--; cannonChar = '\\'; break;
    }

    // show cannon
    if (cannonX >= 0 && cannonX < WIDTH && cannonY >= 0 && cannonY < HEIGHT) {
        ConsoleUtility::gotoxy(cannonX, cannonY);
        cout << cannonChar;
        drawnCannonDirIdx = directionIdx;
        lastCannonX = cannonX;
        lastCannonY = cannonY;
    }

    // calculate where bullet starts
    int bx = cannonX;
    int by = cannonY;

    switch (directionIdx) {
    case 0: by--; break;
    case 1: bx++; by--; break;
    case 2: bx++; break;
    case 3: bx++; by++; break;
    case 4: by++; break;
    case 5: bx--; by++; break;
    case 6: bx--; break;
    case 7: bx--; by--; break;
    }

    if (bx < 0 || bx >= WIDTH || by < 0 || by >= HEIGHT) {
        return false;
    }

    if (gameMap.getAt(by, bx) == '#' || gameMap.getAt(by, bx) == '!') {
        gameMap.damageWall(by, bx);
        return false;
    }
    else if (gameMap.getAt(by, bx) == '@') {
        gameMap.setAt(by, bx, ' ');
        ConsoleUtility::gotoxy(bx, by);
        cout << ' ';
        return false;
    }
    else if (isOccupied(enemies, bx, by)) {
        return true;
    }

    // make new bullet and add to bullets list
    Bullet newBullet;
    newBullet.fire(bx, by, directionIdx);
    bullets.push_back(newBullet);

    // reset shot timer
    framesSinceLastShot = 0;

    // ── STOP any track movement so we don't auto-rotate after shooting
    stopMoving();

    return false;
}

bool Tank::updateBullet(GameMap& gameMap, const Positions& enemies) {
    // count frames since last shot
    framesSinceLastShot++;

    bool hitTarget = false;
    bool tankDied = false;

    // update all bullets
    for (auto it = bullets.begin(); it != bullets.end();) {
        if (!it->isActive()) {
            it = bullets.erase(it);
            continue;
        }

        it->move();

        int bx = it->getX();
        int by = it->getY();

        // check if bullet hit screen edge
        if (bx <= 0 || bx >= WIDTH - 1 || by <= 0 || by >= HEIGHT - 1) {
            // wrap bullet around screen (both axes, so diagonal bullets wrap correctly at corners)
            if (bx <= 0) bx = WIDTH - 2;
            else if (bx >= WIDTH - 1) bx = 1;
            if (by <= 0) by = HEIGHT - 2;
            else if (by >= HEIGHT - 1) by = 1;

            // keep going same direction from new spot
            it->deactivate();
            it->fire(bx, by, it->getDirectionIdx());
        }

        // walls take 2 hits: '#' -> '!' -> gone (same rules as GameMap::damageWall)
        char cell = gameMap.getAt(by, bx);
        if (cell == '#' || cell == '!') {
            gameMap.damageWall(by, bx);
            it->deactivate();
        }
        else if (gameMap.getAt(by, bx) == '@') {
            gameMap.setAt(by, bx, ' ');
            it->deactivate();
            ConsoleUtility::gotoxy(bx, by);
            cout << ' ';
        }

        // hit opponent?
        if (isOccupied(enemies, bx, by)) {
            it->deactivate();
            hitTarget = true;
        }
        // hit self: check both tank body and cannon position
        else if (isBodyHit(bx, by)) {
            it->deactivate();
            // tank shot its own body - it dies
            tankDied = true;
        }
        else if (isCannonHit(bx, by)) {
            it->deactivate();
            // tank shot its own cannon - the cannon breaks
            breakCannon();
        }
        // otherwise just draw the bullet in its new spot
        else {
            it->draw();
        }

        if (!it->isActive()) {
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }

    // if the tank shot itself, it dies
    if (tankDied) {
        return true; // true marks the tank as dead
    }

    return hitTarget;
}

int Tank::getX() const { return x; }
int Tank::getY() const { return y; }
int Tank::getDirectionIdx() const { return directionIdx; }
bool Tank::hasCannonLeft() const { return hasCanon; }