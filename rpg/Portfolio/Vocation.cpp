#include "Vocation.h"
#include <sstream>
#include "readInt.h"
using namespace std;


Vocation::Vocation() {}
Vocation::Vocation(const string& line) {
    istringstream ss(line);
    getline(ss, name, ',');
    if (readInt(ss, health, ',') ||
        readInt(ss, strength, ',') ||
        readInt(ss, speed, ',')) {
        cout << "The \"" << name << "\" vocation is invalid." << endl;
        throw 1;
    }
}


ostream& operator<<(ostream& os, const Vocation& c) {
    // Overloads the output operator to allow easy printing of the the vocation
    return os << c.name << " \t" << c.health << "\t" << c.strength << "\t\t" << c.speed;
}
