#pragma once

class Bullet {
private:
    int x, y;           // where bullet is
    int dx, dy;         // which way bullet moving
    bool active;        // if bullet still exists
    int directionIdx;   // direction bullet going (0-7 like in tank)

public:
    // makes new bullet that does nothing yet
    Bullet();

    // starts bullet from position shooting in direction
    void fire(int startX, int startY, int direction);

    // moves bullet forward
    void move();

    // shows bullet on screen
    void draw() const;

    // makes bullet disappear
    void deactivate();

    // functions to check bullet stuff
    int getX() const;
    int getY() const;
    bool isActive() const;
    int getDirectionIdx() const;
};
