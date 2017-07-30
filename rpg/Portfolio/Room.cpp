#include "Room.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include "readInt.h"

using namespace std;


Room::Room(const vector<Event>& possible_events, const bool first) {
    if (!first) {
        int i;
        for (i = 0; i < rand()%2; i++) {
            events.push_back(possible_events[rand()%possible_events.size()]);
        }

        if (rand()%2 == 0) {
            for (i = 0; i < (rand()%5+1); i++) {
                enemies.push_back(new Enemy());
            }
        }
    } else {
        visited = true;
        contains_user = true;
    }
}


Room::Room(ifstream& file) {
    string line;
    int tmp_int;

    getline(file, line);
    istringstream tmp(line);

    readInt(tmp, tmp_int, ',');
    visited = (tmp_int != 0);

    readInt(tmp, tmp_int, ',');
    contains_user = (tmp_int != 0);

    while (getline(file, line) && !line.empty()) {
        enemies.push_back(new Enemy(line));
    }
    while (getline(file, line) && !line.empty()) {
        events.push_back(Event(line));
    }
}


Room::~Room() {
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
}


bool speedSort(const Character* lhs, const Character* rhs) {return lhs->speed > rhs->speed;}


bool removeCondition(const Character* c) {return (c->health <= 0);}


bool Room::fight(Player* user, COORD* log) {
    if (user->health > 0 && enemies.size() > 0) {
        int i, j, change;
        string clear_log = "                                               ";
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        vector<Character*> participants(enemies.begin(), enemies.end());

        // Clear log
        while ((*log).Y > 0) {
            --(*log).Y;
            SetConsoleCursorPosition(out, *log);
            cout << clear_log;
        }

        participants.push_back(user);

        sort(participants.begin(), participants.end(), speedSort);

        SetConsoleCursorPosition(out, *log);
        cout << "You have encountered some enemies!";
        ++(*log).Y;

        while (participants.size() > 1 && user->health > 0) {
            for (i = 0; i < participants.size();) {
                participants[i]->attack(participants, log);

                // Use change to account for erasing elements within loop
                change = 1;
                for (j = 0; j < i; j++) {
                    if (participants[j]->health <= 0) {
                        --change;
                    }
                }
                i += change;

                // Stop fight if user is dead
                if (user->health <= 0) break;

                // Remove any enemies that are dead
                participants.erase(remove_if(participants.begin(),
                                             participants.end(),
                                             removeCondition),
                                   participants.end());
            }
        }

        for (Enemy* enemy : enemies) {
            delete enemy;
        }
        enemies.clear();

        if (user->health < 0) user->health = 0;
        SetConsoleCursorPosition(out, *log);
        cout << "The fight is over, you have " << user->health << " health.";
        ++(*log).Y;
        return true;
    }
    return false;
}


void Room::doEvents(Player* user, COORD* log) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    while (user->health > 0 && events.size() > 0) {
        Event curr_event = events.back();
        events.pop_back();

        SetConsoleCursorPosition(out, *log);
        cout << curr_event.message;
        ++(*log).Y;

        user->health += curr_event.health_mod;
        if (user->health < 0) user->health = 0;

        SetConsoleCursorPosition(out, *log);
        cout << "You " << ((curr_event.health_mod > 0)?"gain ":"lose ") << abs(curr_event.health_mod) << " health.";
        ++(*log).Y;
        SetConsoleCursorPosition(out, *log);
        cout << "You now have " << user->health << " health.";
        ++(*log).Y;
    }
}


ostream& operator<<(ostream& os, const Room& room) {
    // For saving
    os << (room.north != NULL) << "," << (room.east != NULL) << ","
       << (room.south != NULL) << "," << (room.west != NULL) << endl;
    os << room.visited << "," << room.contains_user << endl;
    for (Enemy* enemy : room.enemies) {
        os << *enemy << endl;
    }
    os << endl;
    for (Event event : room.events) {
        os << event << endl;
    }
    return os;
}
