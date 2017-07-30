#include "Character.h"
using namespace std;


Character::Character() : name(""), health(0), strength(0), speed(0) {}


void Character::attack(const vector<Character*>& targets, COORD* log) {}


ostream& operator<<(ostream& os, const Character& c) {
    // Overloads the output operator to allow easy printing of the the character
    return os << c.name << ":\t" << c.health << "\t" << c.strength << "\t\t" << c.speed;
}
