#include "Enemy.h"
#include <sstream>
#include <fstream>
#include "readInt.h"

using namespace std;


static const string names[10] = {"Spider", "Wolf", "Boar", "Bear", "Brute",
                    "Druid", "Wizard", "Witch", "Warrior", "Knight"};


Enemy::Enemy() {
    int i = rand()%10;
    name = names[i];
    // Stats based on enemy type/name
    health = rand()%100 + 10*i + 1;
    strength = rand()%40 + 2*i + 1;
    speed = rand()%40 + 2*(10-i) + 1;
}


Enemy::Enemy(const string& line) {
    istringstream enemy(line);

    getline(enemy, name, ',');

    if (readInt(enemy, health, ',') ||
        readInt(enemy, strength, ',') ||
        readInt(enemy, speed, ',')) {
        cout << "The savefile is invalid." << endl;
        throw 1;
    }
}


void Enemy::attack(const vector<Character*>& targets, COORD* log) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    // Target character with highest health
    int i = 0, choice = 0;
    if (targets[i] == this) {
        i++;
        choice++;
    }
    for (; i < targets.size(); i++) {
        if (targets[i] != this && targets[i]->health > targets[choice]->health) {
            choice = i;
        }
    }
    targets[choice]->health -= this->strength;
    ++(*log).Y;
    SetConsoleCursorPosition(out, *log);
    cout << targets[choice]->name << " has taken " << this->strength << " damage from a " << this->name << '.';
    ++(*log).Y;
    if (targets[choice]->health > 0) {
        SetConsoleCursorPosition(out, *log);
        cout << targets[choice]->name << " now has " << targets[choice]->health << " health.";
        ++(*log).Y;
    }
}


ostream& operator<<(ostream& os, const Enemy& enemy) {
    // For saving
    return os << enemy.name << "," << enemy.health << "," << enemy.strength << "," << enemy.speed;
}
