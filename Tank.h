// Tank.h - this is the tanks that players control
#pragma once
#include <vector>
#include <utility>
#include "Bullet.h"
#include "GameMap.h"
// (x, y) positions of other tanks
using Positions = std::vector<std::pair<int, int>>;

class Tank {
private:
    int x, y;                    // where tank is
    int directionIdx;            // which way tank pointing (0-7)
    int drawnCannonDirIdx;       // what direction we actually draw the cannon
    char symbol;                 // how tank looks (usually 'O')
    std::vector<Bullet> bullets; // bullets tank shot that still moving
    int framesSinceLastShot;    // how long since last shot
    // track controls
    bool leftTrackForward;
    bool leftTrackBackward;
    bool rightTrackForward;
    bool rightTrackBackward;
    // remember where tank was before to clean screen
    int lastCannonX;
    int lastCannonY;
    int lastTankX;
    int lastTankY;
    // colors for tank
    int tankColor;
    int cannonColor;
    bool useColor;
    // check if the tank has cannon
    bool hasCanon;

    // check if the bullet hits the tank in body
    bool isBodyHit(int bx, int by) const;
    // check if the bullet hits the tank cannon
    bool isCannonHit(int bx, int by) const;

public:
    // makes new tank at starting position
    Tank(int startX, int startY, int startDir, char tankSymbol,
        bool colored = false, int tColor = COLOR_WHITE, int cColor = COLOR_WHITE);
    // shows tank on screen
    void draw();
    // removes tank from old position
    void erase() const;
    // controls for tank tracks
    void setLeftTrack(bool forward, bool backward);
    void setRightTrack(bool forward, bool backward);
    void stopMoving();
    // checks if tank moving
    bool isMoving() const;
    // moves tank and checks if it hit anything
    bool updateMovement(GameMap& gameMap, const Positions& otherTanks);
    // makes tank shoot
    bool fire(GameMap& gameMap, const Positions& enemies);
    // moves all bullets tank shot
    bool updateBullet(GameMap& gameMap, const Positions& enemies);
    // breaks the tank's cannon
    void breakCannon();

    // get tank information
    int getX() const;
    int getY() const;
    int getDirectionIdx() const;
    bool hasCannonLeft() const;
};