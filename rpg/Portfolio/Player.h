#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Vocation.h"
#include "Character.h"

class Player: public Character {
 public:
    Player();
    Player(const std::string& inName, const Vocation& vocation);
    explicit Player(const std::string& line);
    void attack(const std::vector<Character*>& targets, COORD* log);
};

std::ostream& operator<<(std::ostream& os, const Player& c);
