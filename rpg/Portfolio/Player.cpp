#include "Player.h"
#include <conio.h>
#include <sstream>
#include <fstream>
#include "readInt.h"
using namespace std;


Player::Player() {}


Player::Player(const string& inName, const Vocation& vocation) {
    name = inName;
    health = vocation.health;
    strength = vocation.strength;
    speed = vocation.speed;
}


Player::Player(const string& line) {
    istringstream player(line);

    getline(player, name, ',');

    if (readInt(player, health, ',') ||
        readInt(player, strength, ',') ||
        readInt(player, speed, ',')) {
        cout << "The savefile is invalid." << endl;
        throw 1;
    }
}


void Player::attack(const vector<Character*>& targets, COORD* log) {
    int choice, self;
    int i = 0;
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    ++(*log).Y;
    SetConsoleCursorPosition(out, *log);
    cout << "Choose one of the following enemies to attack:";
    ++(*log).Y;
    SetConsoleCursorPosition(out, *log);
    cout << "   Name \tHealth\tStrength\tSpeed";
    ++(*log).Y;
    for (Character* character : targets) {
        if (character != this) {
            SetConsoleCursorPosition(out, *log);
            cout << i++ << ": " << *character;
            ++(*log).Y;
        } else {
            self = i;
        }
    }
    SetConsoleCursorPosition(out, *log);
    ++(*log).Y;
    cout << "Pick a number from 0 to " << i-1 << ": ";
    while ((choice = _getch() - '0') < 0 || choice > i-1) {}

    if (choice >= self) choice++;

    targets[choice]->health -= this->strength;

    ++(*log).Y;
    SetConsoleCursorPosition(out, *log);
    cout << "You did " << this->strength << " damage to a " << targets[choice]->name << '.';
    ++(*log).Y;
    SetConsoleCursorPosition(out, *log);
    if (targets[choice]->health > 0) {
        cout << "they now have " << targets[choice]->health << " health.";
    } else {
        cout << "they are now dead.";
    }
    ++(*log).Y;
}


ostream& operator<<(ostream& os, const Player& c) {
    // Overloads the output operator to allow easy printing of the the player
    return os << c.name << "," << c.health << "," << c.strength << "," << c.speed;
}
