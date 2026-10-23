// TankBattle.h - main game class that runs everything
#pragma once
#include "Tank.h"
#include "GameMap.h"
#include <vector>
#include <string>


class TankBattle {
private:
    bool exitProgram;  // if player want to exit game
    bool gameOver;     // if round finished
    bool paused;       // if game paused
    char winner;       // who won (1 or 2 or nothing)
    GameMap gameMap;   // the game map
    bool colorMode = false;    // if we using colors
    // Loads one .screen file into gameMap and spawns tanks:
    void loadScreen(const std::string& filename);

    // For multi‐tank support:
    std::vector<Tank*> player1Tanks;
    std::vector<Tank*> player2Tanks;
    size_t current1 = 0, current2 = 0;    // which tank is currently controlled
    int legendX = 0, legendY = 0;         // where to draw score/legend
    int scoreP1 = 0, scoreP2 = 0;         // running score

    // functions that control game
    char showMainMenu() const; 
    void showInstructions();
    void initializeGame();
    void handleInput();
    void update();
    void runGame();

public:
    // starts game
    TankBattle();

    // cleans up when game ends
    ~TankBattle();

    // main function that runs game
    void run();

    std::vector<std::string> discoverScreens() const;

};