#pragma once
#include <windows.h>
#include <fstream>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Event.h"

class Room {
 public:
    Room(const std::vector<Event>& possible_events, bool first);
    explicit Room(std::ifstream& file);
    ~Room();
    bool fight(Player* user, COORD* log);
    void doEvents(Player* user, COORD* log);
    Room* north = NULL;
    Room* east = NULL;
    Room* south = NULL;
    Room* west = NULL;
    bool visited = false;
    bool contains_user = false;
    std::vector<Enemy*> enemies;
    std::vector<Event> events;
};

std::ostream& operator<<(std::ostream& os, const Room& room);
