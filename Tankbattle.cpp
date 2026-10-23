#include "TankBattle.h"  // include tank battle header
#include "ConsuleUtility.h"  // include console utitlity
#include "PauseUtils.h"  // include pause utils
#include <iostream>  // include iostream for cout
#include <conio.h>  // include conio for getch and kbhit
#include <filesystem>  // include filesystem for dir iter
#include <vector>  // include vector for tank lists
#include <string>  // include string
#include <algorithm>  // include algorithm for sort
#include <fstream>  // include fstream for file read
#include <cstdlib>      // for rand()



namespace fs = std::filesystem;  // alias fs

std::vector<std::string> TankBattle::discoverScreens() const {  // find .screen files
    std::vector<std::string> screens;  // make vector of string
    const std::string prefix = "tanks-game";  // prefix filter
    const std::string suffix = ".screen";  // suffix filter

    for (auto& entry : fs::directory_iterator(fs::current_path())) {  // for each file in cwd
        if (!entry.is_directory()) {  // if not folder
            auto name = entry.path().filename().string();  // get filename
            if (name.rfind(prefix, 0) == 0  // if start with prefix
                && name.size() > prefix.size()  // and length enough
                && name.substr(name.size() - suffix.size()) == suffix)  // and endwith suffix
            {
                screens.push_back(name);  // add to screens
            }
        }
    }
    std::sort(screens.begin(), screens.end());  // sort names
    return screens;  // return list
}

void TankBattle::loadScreen(const std::string& filename) {  // load one screeen file
    // 1) Clear old map + tanks
    gameMap.initialize();  // reset map
    for (auto t : player1Tanks) delete t;  // delete old tanks p1
    for (auto t : player2Tanks) delete t;  // delete old tanks p2
    player1Tanks.clear();  // clear vector p1
    player2Tanks.clear();  // clear vector p2
    current1 = current2 = 0;  // reset current indexes
    legendX = legendY = 0;      // reset legend pos but score stay

    // 2) Open and parse the .screen file
    std::ifstream in(filename);  // open file
    if (!in) {  // if fail
        ConsoleUtility::clearScreen();  // clear screen
        std::cout << "Failed to open " << filename << "\nPress any key...";  // err msg
        (void)_getch();  // wait key
        return;  // exit
    }

    std::string line;  // for each line
    int y = 0;  // y row idx
    const int dirX[8] = { 0,1,1,1,0,-1,-1,-1 };  // bullet dir x
    const int dirY[8] = { -1,-1,0,1,1,1,0,-1 };  // bullet dir y

    while (std::getline(in, line)) {  // read line by line
        for (int x = 0; x < (int)line.size(); ++x) {  // for each char in line
            char c = line[x];  // get char
            switch (c) {  // switch on char
            case '#':  gameMap.setWall(x, y);  break;  // # is wall
            case '@':  gameMap.setMine(x, y);  break;  // @ is mine
            case 'L':
                legendX = x; legendY = y;  // set legend pos
                gameMap.setEmpty(x, y);  // clear cell
                break;
            case '1': {
                int cannonDir = -1;  // choose random direction
                for (int i = 0; i < 8; ++i) {  // try up to 8 times
                    int d = rand() % 8;  // random dir
                    int nx = x + dirX[d], ny = y + dirY[d];  // new pos
                    if (gameMap.isEmpty(nx, ny)) { cannonDir = d; break; }  // if free
                }
                player1Tanks.push_back(
                    new Tank(x, y,
                        cannonDir < 0 ? 0 : cannonDir,
                        'O', colorMode)
                );  // make tank and push
                break;
            }
            case '2': {
                int cannonDir = -1;
                for (int i = 0; i < 8; ++i) {
                    int d = rand() % 8;
                    int nx = x + dirX[d], ny = y + dirY[d];
                    if (gameMap.isEmpty(nx, ny)) { cannonDir = d; break; }
                }
                player2Tanks.push_back(
                    new Tank(x, y,
                        cannonDir < 0 ? 0 : cannonDir,
                        'O', colorMode)
                );
                break;
            }
            default:
                gameMap.setEmpty(x, y);  // anything else empty
            }
        }
        ++y;  // next row
    }

    // 3) Draw the new map, legend, and all tanks
    ConsoleUtility::clearScreen();  // clear screen
    gameMap.draw();  // draw map
    ConsoleUtility::gotoxy(legendX, legendY);  // move to legend
    std::cout << "P1:" << scoreP1 << "  P2:" << scoreP2;  // print score
    for (auto t : player1Tanks) t->draw();  // draw p1 tanks
    for (auto t : player2Tanks) t->draw();  // draw p2 tanks
    std::cout.flush();  // flush output
}




using namespace std;  // use std for later

TankBattle::TankBattle() : exitProgram(false), gameOver(false), paused(false), winner(' ') {  // ctor init flags
    ConsoleUtility::setConsoleSize(WIDTH, HEIGHT);  // set console size
    ConsoleUtility::hideCursor();  // hide cursor

}

TankBattle::~TankBattle() {  // destructor
    // delete every spawned tank
    for (auto t : player1Tanks) delete t;
    for (auto t : player2Tanks) delete t;
}

char TankBattle::showMainMenu() const {  // show main menu options
    ConsoleUtility::clearScreen();  // clear screen
    cout << "********** TANK BATTLE **********" << endl;  // title
    cout << "1. Start Classic Game" << endl;  // option 1
    cout << "2. Start Colored Game" << endl;  // option 2
    cout << "3. Instructions" << endl;  // option 3
    cout << "4. Exit" << endl;  // option 4
    cout.flush();  // flush

    char choice = 0;  // store user choice
    while (true) {  // loop until valid
        if (_kbhit()) {  // if key pressed
            choice = _getch();  // get char
            if (choice >= '1' && choice <= '4') {  // if valid
                return choice;  // return it
            }
        }
        Sleep(50);  // small delay
    }
}

// this fucntion show all game instructin on screen for player
void TankBattle::showInstructions() {
    // clear old screen so no old text stay
    ConsoleUtility::clearScreen();
    // print title of instructin
    cout << "*** Instructions ***" << endl;
    // show player1 control explain
    cout << "Player 1 controls:" << endl;
    // Q and A track forward/back
    cout << "  Q = LEFT track forward, A = LEFT track backwards" << endl;
    // E and D track forward/back
    cout << "  E = RIGHT track forward, D = RIGHT track backwards" << endl;
    // S stop, W shoot
    cout << "  S = STAY (stop all movement), W = SHOOT" << endl;
    cout << endl;
    // show player2 control explain
    cout << "Player 2 controls:" << endl;
    // U and J track forward/back
    cout << "  U = LEFT track forward, J = LEFT track backwards" << endl;
    // O and L track forward/back
    cout << "  O = RIGHT track forward, L = RIGHT track backwards" << endl;
    // K stop, I shoot
    cout << "  K = STAY (stop all movement), I = SHOOT" << endl;
    cout << endl;
    // show movment rules
    cout << "Movement rules:" << endl;
    cout << "- RIGHT track forward ONLY: Rotate counter-clockwise with 8 steps" << endl;
    cout << "- RIGHT track forward + LEFT track backwards: Rotate counter-clockwise with 4 steps" << endl;
    cout << "- LEFT track forward ONLY: Rotate clockwise with 8 steps" << endl;
    cout << "- LEFT track forward + RIGHT track backwards: Rotate clockwise with 4 steps" << endl;
    cout << "- BOTH tracks forward: Move forward in current direction" << endl;
    cout << "- BOTH tracks backward: Move backward (opposite of current direction)" << endl;
    cout << endl;
    // game symbols explain
    cout << "Game stuff:" << endl;
    cout << "O = Tank, - or | or \\ or / = Cannon (shows direction)" << endl;
    cout << "@ = Mine, # = Wall (takes 2 hits), * = Bullet" << endl;
    cout << "! = Wall hit once" << endl;
    cout << "Can shoot every 5 frames, many bullets allowed" << endl;
    cout << endl;
    // edge wrapping explain
    cout << "Tanks and bullets wrap around edges" << endl;
    // pause keys explain
    cout << "Press P or ESC to pause. When paused, ESC to continue or X for menu" << endl;
    // back to main menu prompt
    cout << "Press any key for main menu..." << endl;
    // flush out put immediatly
    cout.flush();
    // wait key press so stay on instructin
    (void)_getch();
}

// this fucntion set game start state before load map
void TankBattle::initializeGame() {
    // no one win
    gameOver = false;
    // not paused start
    paused = false;
    // winner char blank
    winner = ' ';
    // map and tanks will be load by loadScreen later
}



void TankBattle::handleInput() {
    if (!_kbhit()) return;
    int key = _getch();

    // Pause / unpause
    if (key == ESC_KEY) {
        for (int i = 0; i < 10; ++i)
            ConsoleUtility::cleanCharAt(i, 0);
        if (paused) {
            paused = false; clearPauseMessage();
        }
        else {
            paused = true;  showPauseMessage();
        }
        return;
    }
    // swallow extended key marker
    if (key == 0 || key == -32) { (void)_getch(); return; }
    key = tolower(key);

    // while paused: only allow unpause or exit-to-menu
    if (paused) {
        if (key == 'p') { paused = false; clearPauseMessage(); }
        else if (key == 'x') { gameOver = true; winner = ' '; }
        return;
    }

    // --- Controls & switching ---
    switch (key) {
        // Player 1 move/shoot
    case 'q':
        if (!player1Tanks.empty())
            player1Tanks[current1]->setLeftTrack(true, false);
        break;
    case 'a':
        if (!player1Tanks.empty())
            player1Tanks[current1]->setLeftTrack(false, true);
        break;
    case 'e':
        if (!player1Tanks.empty())
            player1Tanks[current1]->setRightTrack(true, false);
        break;
    case 'd':
        if (!player1Tanks.empty())
            player1Tanks[current1]->setRightTrack(false, true);
        break;
    case 's':
        if (!player1Tanks.empty())
            player1Tanks[current1]->stopMoving();
        break;
    case 'w': {
        if (!player1Tanks.empty()) {
            int tx = player2Tanks.empty() ? -1 : player2Tanks[0]->getX();
            int ty = player2Tanks.empty() ? -1 : player2Tanks[0]->getY();
            if (player1Tanks[current1]->fire(gameMap, tx, ty)) {
                gameOver = true; winner = '1';
            }
        }
    } break;

            // Player 2 move/shoot
    case 'u':
        if (!player2Tanks.empty())
            player2Tanks[current2]->setLeftTrack(true, false);
        break;
    case 'j':
        if (!player2Tanks.empty())
            player2Tanks[current2]->setLeftTrack(false, true);
        break;
    case 'o':
        if (!player2Tanks.empty())
            player2Tanks[current2]->setRightTrack(true, false);
        break;
    case 'l':
        if (!player2Tanks.empty())
            player2Tanks[current2]->setRightTrack(false, true);
        break;
    case 'k':
        if (!player2Tanks.empty())
            player2Tanks[current2]->stopMoving();
        break;
    case 'i': {
        if (!player2Tanks.empty()) {
            int tx = player1Tanks.empty() ? -1 : player1Tanks[0]->getX();
            int ty = player1Tanks.empty() ? -1 : player1Tanks[0]->getY();
            if (player2Tanks[current2]->fire(gameMap, tx, ty)) {
                gameOver = true; winner = '2';
            }
        }
    } break;

            // Switch active tank
    case 'z':
        if (!player1Tanks.empty())
            current1 = (current1 + 1) % player1Tanks.size();
        break;
    case 'm':
        if (!player2Tanks.empty())
            current2 = (current2 + 1) % player2Tanks.size();
        break;

        // Pause
    case 'p':
        paused = true; showPauseMessage();
        break;
    } // dividing to some cases 

    // --- Update movement for all tanks ---
    for (auto t : player1Tanks) {
        if (t->isMoving()) {
            int tx = player2Tanks.empty() ? -1 : player2Tanks[0]->getX();
            int ty = player2Tanks.empty() ? -1 : player2Tanks[0]->getY();
            if (!t->updateMovement(gameMap, tx, ty)
                && gameMap.getAt(t->getY(), t->getX()) == '@')
            {
                gameOver = true; winner = '2';
            }
        }
    }
    for (auto t : player2Tanks) { // auto loop for moving
        if (t->isMoving()) {
            int tx = player1Tanks.empty() ? -1 : player1Tanks[0]->getX();
            int ty = player1Tanks.empty() ? -1 : player1Tanks[0]->getY();
            if (!t->updateMovement(gameMap, tx, ty)
                && gameMap.getAt(t->getY(), t->getX()) == '@')
            {
                gameOver = true; winner = '1';
            }
        }
    }

    // restore the score display if any tank.erase() got to it
    ConsoleUtility::gotoxy(legendX, legendY);
    std::cout << "P1:" << scoreP1 << "  P2:" << scoreP2;
    std::cout.flush();
}



void TankBattle::update() {
    if (paused || gameOver) return;

    // 1) Player 1 bullets
    for (auto t : player1Tanks) {
        // target the first P2 tank (or off‐map if none)
        int tx = player2Tanks.empty() ? -1 : player2Tanks[0]->getX();
        int ty = player2Tanks.empty() ? -1 : player2Tanks[0]->getY();
        if (t->updateBullet(gameMap, tx, ty)) {
            gameOver = true;
            winner = '1';
            return;  // stop processing once someone wins
        }
    }

    // 2) Player 2 bullets
    for (auto t : player2Tanks) {
        int tx = player1Tanks.empty() ? -1 : player1Tanks[0]->getX();
        int ty = player1Tanks.empty() ? -1 : player1Tanks[0]->getY();
        if (t->updateBullet(gameMap, tx, ty)) {
            gameOver = true;
            winner = '2';
            return;
        }
    }

    for (auto t : player1Tanks) t->draw();
    for (auto t : player2Tanks) t->draw();

}


void TankBattle::runGame() {
    while (!gameOver) {
        handleInput();
        update();
        cout.flush();
        Sleep(50);
    }

    if (winner == '1') ++scoreP1;
    else if (winner == '2') ++scoreP2;

    ConsoleUtility::clearScreen();

    // Redisplay running score at the legend position:
    ConsoleUtility::gotoxy(legendX, legendY);
    std::cout << "P1:" << scoreP1 << "  P2:" << scoreP2;


    if (winner == '1') {
        cout << "Game Over! Player 1 wins." << endl;
    }
    else if (winner == '2') {
        cout << "Game Over! Player 2 wins." << endl;
    }
    else {
        cout << "Game Over! (No winner)" << endl;
    }
    cout << "Press any key for main menu..." << endl;
    cout.flush();
    (void)_getch();
}

void TankBattle::run() {
    while (!exitProgram) {
        char choice = showMainMenu();

        if (choice == '1' || choice == '2') {
            // 1) Set color‐mode based on choice
            colorMode = (choice == '2');

            // 2) Discover all .screen files
            auto screens = discoverScreens();

            if (screens.empty()) {
                ConsoleUtility::clearScreen();
                std::cout << "No tanks-game*.screen files found.\n";
                std::cout << "Press any key to return to menu...";
                (void)_getch();
                continue;              // back to main menu
            }

            // 3) Show the list of available levels
            ConsoleUtility::clearScreen();
            std::cout << "Available levels:\n";
            for (size_t i = 0; i < screens.size(); ++i)
                std::cout << "  " << (i + 1) << ". " << screens[i] << "\n";
            std::cout << "Select level (1-" << screens.size()
                << ") or press ENTER to play all:\n";

            // 4) Read a single key
            char c = _getch();
            size_t startIdx = 0, endIdx = screens.size();
            if (c >= '1' && c < char('1' + screens.size())) {
                startIdx = c - '1';
                endIdx = startIdx + 1;
            }

            // 5) Loop through chosen screens
            for (size_t si = startIdx; si < endIdx; ++si) {
                initializeGame();             // reset map & tanks
                loadScreen(screens[si]);      // (you’ll write this next)
                runGame();                    // play until one side dies
            }
        }

        else if (choice == '3') {
            showInstructions();
        }
        else if (choice == '4') {
            exitProgram = true;
        }
    }
}