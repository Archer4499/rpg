#pragma once
#include <iostream>
#include <string>

class Vocation {
 public:
    std::string name;
    int health;
    int strength;
    int speed;
    Vocation();
    explicit Vocation(const std::string& line);
};

std::ostream& operator<<(std::ostream& os, const Vocation& c);
