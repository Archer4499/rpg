#pragma once
#include <string>
#include <iostream>
#include <vector>

#include "Character.h"
#include "Player.h"
#include "Room.h"

bool speedSort(const Character* lhs, const Character* rhs);
bool removeCondition(const Character* c);

class Game {
 private:
    Player user;
    std::vector<std::vector<Room*>> dungeon;
    Room* curr_room;
    bool cheat = false;
    void createPlayer();
    void generateDungeon();
    void printMap();
    void fight();
    void loadGame();
    void saveGame();

 public:
    Game();
    ~Game();
    void gameLoop();

    friend std::ostream& operator<<(std::ostream &os, const Game& game);
};

std::ostream& operator<<(std::ostream& os, const Game& game);
