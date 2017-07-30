#include "Event.h"
#include <iostream>
#include <sstream>
#include "readInt.h"
using namespace std;


Event::Event() {}

Event::Event(const string& line) {
    istringstream event(line);

    getline(event, message, ',');

    if (readInt(event, health_mod, ',')) {
        cout << "Events file contains an invalid health modifier number" << endl;
        throw 1;
    }
}


ostream& operator<<(ostream& os, const Event& event) {
    // For saving
    return os << event.message << "," << event.health_mod;
}
