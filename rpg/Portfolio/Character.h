#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "Vocation.h"

class Character {
 public:
    std::string name;
    int health;
    int strength;
    int speed;

    Character();
    virtual void attack(const std::vector<Character*>& targets, COORD* log);
};

std::ostream& operator<<(std::ostream& os, const Character& c);
