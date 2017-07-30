#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "Vocation.h"
#include "Character.h"

class Enemy: public Character {
 public:
    Enemy();
    explicit Enemy(const std::string& line);
    void attack(const std::vector<Character*>& targets, COORD* log);
};

std::ostream& operator<<(std::ostream& os, const Enemy& enemy);
