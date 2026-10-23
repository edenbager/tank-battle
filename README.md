# tank-battle

A two-player tank battle game for the Windows console, written in C++. Each player drives a tank with two independent tracks, aims an 8-direction cannon and shoots through destructible walls. Built with **Tomer Yad Shalom** for an Object-Oriented Programming in C++ course at The Academic College of Tel Aviv–Yaffo.

**Stack:** C++17 · Windows Console API · Visual Studio

## Build and run

Windows only (it uses `windows.h` and `conio.h` for cursor control and keyboard input).

1. Open the `.cpp` and `.h` files in a Visual Studio C++ console project, or build from a Developer Command Prompt:
   ```
   cl /std:c++17 /EHsc *.cpp /Fe:tank-battle.exe
   ```
2. Run it from the folder that holds the `tanks-game_*.screen` level files (three are included).
3. Run `tank-battle.exe`, pick classic or colored mode, then a level.

## Controls

| | Player 1 | Player 2 |
|---|---|---|
| Left track forward / back | Q / A | U / J |
| Right track forward / back | E / D | O / L |
| Stop | S | K |
| Shoot | W | I |
| Switch tank | Z | M |

P or ESC pauses. While paused, ESC continues and X returns to the menu.

## How it works

- **Track-based movement.** Like a real tank, each track is controlled separately: both forward drives straight, one track rotates 45°, opposite tracks rotate 90°.
- **8 directions.** The cannon is drawn as `| / - \` and bullets travel in the cannon's direction, including diagonals.
- **Destructible walls.** Walls take two hits: `#` becomes `!` on the first hit and disappears on the second. Map edges can't be destroyed.
- **Friendly fire.** A bullet that wraps around the map can hit its own tank. Hitting your own cannon breaks it, and the tank can no longer shoot.
- **Mines.** Driving onto a mine (`@`) loses the round. Bullets clear mines.
- **Levels from files.** Every `tanks-game*.screen` file in the folder is a level: `#` wall, `@` mine, `1` / `2` tank start positions, `L` score position. A level can give each player several tanks; Z and M switch which one you drive.

## Code structure

| File | Responsibility |
|---|---|
| `TankBattle` | Game loop, menu, input handling, level loading, score |
| `Tank` | Movement, cannon, firing, bullet updates and hit detection |
| `Bullet` | Position, direction, drawing |
| `GameMap` | Map cells, wall health, boundaries |
| `ConsoleUtility` | Windows console wrapper: cursor, colors, window size |
| `PauseUtils` | Pause message |

## Fixes after submission

- **Wall damage bug.** Two parts of the code damaged walls differently: one marked a hit wall `!`, the other `%`, and tanks could drive through a `%` wall. All wall hits now go through `GameMap::damageWall`.
- **Multiple tanks per player.** Shots and collisions only checked the opponent's first tank, so the other tanks couldn't be hit and teammates drove through each other. Every tank now counts.
- **Indestructible map edges.** The border loaded from a level file became ordinary 2-hit walls that a tank could shoot away. Edges now always stay solid.
- **Corner wrapping.** A diagonal bullet leaving through a corner only wrapped on one axis. It now wraps on both.
- **Build on case-sensitive systems.** File names didn't match their `#include`s (for example `Gamemap.h` vs `GameMap.h`), which only works on Windows.
- Translated the remaining Hebrew comments and corrected the in-game instructions (tanks stop at the map edge; only bullets wrap).
- Levels: added cover and mines to level A (it was an almost empty arena) and mirrored level B's mine so both sides are equal.

The fixes are covered by a small logic test (wall hits, multi-tank hits, teammate collisions, map edges) run against the game classes.

## What I'd improve

- **Losing one tank shouldn't end the round.** Any hit currently wins the round, even when the other player has more tanks left.
- **Automated tests in the repo.** The logic test I used runs against stub Windows headers; adding it with a CMake target would make it repeatable.
- **Remove duplication.** The 8-direction offset table is repeated in several places; one shared helper would remove it.
- **Smart pointers.** Tanks are managed with raw `new` / `delete`; `std::unique_ptr` would make ownership explicit.
- **Cross-platform console.** Replacing `windows.h` / `conio.h` with a small terminal layer (for example ncurses on Linux and macOS) would let it run anywhere.
