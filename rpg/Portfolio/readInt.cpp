#include "readInt.h"
#include <sstream>
using namespace std;

int readInt(istream& ins, int& value, const char sep) {
    // Reads a line from ins and converts it into an int
    // Does not modify value if ins doesn't contain a valid int
    string s;
    getline(ins, s, sep);
    if (!(ins.fail() || s.empty())) {
        int i;
        istringstream ss(s);
        ss >> i;
        if (ss.eof()) {
            value = i;
            return 0;
        }
    }
    ins.clear();
    return 1;
}
