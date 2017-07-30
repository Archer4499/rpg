#include "Game.h"

#include <windows.h>
#include <conio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

#include "readInt.h"
#include "Vocation.h"
#include "Room.h"
#include "Enemy.h"
#include "Event.h"

using namespace std;

#define SAVEFILE "profile.sav"
#define VOCATIONS_FILE "vocations.cfg"
#define EVENTS_FILE "events.cfg"
#define WIDTH 10
#define HEIGHT 10

Game::Game() {
    string tmp;
    bool load = false;

    srand(static_cast<unsigned>(time(NULL)));

    // Check if non empty save file exists
    if (ifstream(SAVEFILE).peek() != ifstream::traits_type::eof()) {
        cout << "Save file detected," << endl;
        cout << "do you want to load the saved game? ([y]/n): ";
        getline(cin, tmp);
        load = (tmp.compare("n") != 0);
    }
    if (load) {
        // Load character
        loadGame();
        cout << "Loaded character " << user.name << " with " << user.health << " health." << endl;
    } else {
        createPlayer();
        generateDungeon();
    }
}


Game::~Game() {
    for (vector<Room*> column : dungeon) {
        for (Room* room : column) {
            delete room;
        }
    }
}


void Game::loadGame() {
    int i, j, tmp_int;
    int walls[WIDTH][HEIGHT][4];
    string line;

    ifstream file(SAVEFILE);
        getline(file, line);
        user = Player(line);

        for (i = 0; i < WIDTH; ++i) {
            vector<Room*> column;
            for (j = 0; j < HEIGHT; ++j) {
                    getline(file, line);
                    istringstream tmp(line);
                    readInt(tmp, tmp_int, ',');
                    walls[i][j][0] = tmp_int;
                    readInt(tmp, tmp_int, ',');
                    walls[i][j][1] = tmp_int;
                    readInt(tmp, tmp_int, ',');
                    walls[i][j][2] = tmp_int;
                    readInt(tmp, tmp_int, ',');
                    walls[i][j][3] = tmp_int;

                    column.push_back(new Room(file));
            }
            dungeon.push_back(column);
        }

        for (i = 0; i < WIDTH; ++i) {
            for (j = 0; j < HEIGHT; ++j) {
                    if (walls[i][j][0]) dungeon[i][j]->north = dungeon[i][j-1];
                    if (walls[i][j][1]) dungeon[i][j]->east = dungeon[i+1][j];
                    if (walls[i][j][2]) dungeon[i][j]->south = dungeon[i][j+1];
                    if (walls[i][j][3]) dungeon[i][j]->west = dungeon[i-1][j];
                    if (dungeon[i][j]->contains_user) {
                        curr_room = dungeon[i][j];
                    }
            }
        }
    file.close();
}


void Game::createPlayer() {
    int i;
    int choice;
    string name, line;
    vector<Vocation> vocations;

    do {
        cout << "Enter a name for your character: ";
        getline(cin, name);
    } while (name.empty());
    cout << "Welcome, " << name << "." << endl;

    cout << endl << "Choose one of the following vocations:" << endl;
    cout << "   Name \tHealth\tStrength\tSpeed" << endl;

    // Indents on file open for clarity
    ifstream fVocations(VOCATIONS_FILE);
        while (getline(fVocations, line)) vocations.push_back(Vocation(line));
    fVocations.close();

    i = 0;
    for (Vocation vocation : vocations) {
        cout << i++ << ": " << vocation << endl;
    }

    do {
        cout << "Pick a number from 0 to " << vocations.size()-1 << ": ";
    } while (readInt(cin, choice) || choice < 0 || choice >= vocations.size());

    user = Player(name, vocations[choice]);

    cout << "You have chosen the " << vocations[choice].name << " class." << endl;
    cout << "The stats of you character are:" << endl;
    cout << "\tHealth: " << user.health << endl;
    cout << "\tStrength: " << user.strength << endl;
    cout << "\tSpeed: " << user.speed << endl;
}


void Game::generateDungeon() {
    int i, j;
    string line;
    vector<Event> possible_events;

    ifstream fevents(EVENTS_FILE);
        while (getline(fevents, line)) {
            possible_events.push_back(Event(line));
        }
    fevents.close();

    // TODO(Derek): possibly make maze harder by changing binary tree
    // Generate 10 x 10 room layout
    for (i = 0; i < WIDTH; ++i) {
        vector<Room*> column;
        for (j = 0; j < HEIGHT; ++j) {
            // Set visited and don't create enemies or events in first room
            if (i == 0 && j == 0) {
                column.push_back(new Room(possible_events, true));
            } else {
                column.push_back(new Room(possible_events, false));
            }
        }
        dungeon.push_back(column);
    }
    curr_room = dungeon[0][0];

    // Implementation of random binary tree maze generation algorithm
    for (i = 0; i < WIDTH-1; i++) {
        for (j = 0; j < HEIGHT-1; j++) {
            if (rand()%2) {
                dungeon[i][j]->east = dungeon[i+1][j];
                dungeon[i+1][j]->west = dungeon[i][j];
            } else {
                dungeon[i][j]->south = dungeon[i][j+1];
                dungeon[i][j+1]->north = dungeon[i][j];
            }
        }
    }

    for (i = 0; i < WIDTH; i++) {
        if (i < WIDTH-1) dungeon[i][HEIGHT-1]->east = dungeon[i+1][HEIGHT-1];
        if (i > 0) dungeon[i][HEIGHT-1]->west = dungeon[i-1][HEIGHT-1];
    }
    for (i = 0; i < HEIGHT; i++) {
        if (i < HEIGHT-1) dungeon[WIDTH-1][i]->south = dungeon[WIDTH-1][i+1];
        if (i > 0) dungeon[WIDTH-1][i]->north = dungeon[WIDTH-1][i-1];
    }
}


// Use extended ASCII characters for printing map
static const unsigned char hori = 196;
static const unsigned char vert = 179;
static const unsigned char cross = 197;

static const unsigned char top_cross = 194;
static const unsigned char bottom_cross = 193;
static const unsigned char left_cross = 195;
static const unsigned char right_cross = 180;

static const unsigned char top_left = 218;
static const unsigned char top_right = 191;
static const unsigned char bottom_left = 192;
static const unsigned char bottom_right = 217;

static const unsigned char player_icon = 64;
static const unsigned char enemy_icon = 155;

static const unsigned char hidden = 177;

void Game::printMap() {
    int i, j;

    // Print top wall
    cout << top_left;
    for (i = 0; i < WIDTH-1; i++) cout << hori << hori << top_cross;
    cout << hori << hori << top_right << endl;

    for (j = 0; j < HEIGHT; j++) {
        cout << vert;
        for (i = 0; i < WIDTH; i++) {
            bool hide = (!cheat && !dungeon[i][j]->visited);
            bool adj_hide = hide;
            if (adj_hide) {
                if (dungeon[i][j]->north != NULL && dungeon[i][j]->north->visited) adj_hide = false;
                if (dungeon[i][j]->east != NULL && dungeon[i][j]->east->visited) adj_hide = false;
                if (dungeon[i][j]->south != NULL && dungeon[i][j]->south->visited) adj_hide = false;
                if (dungeon[i][j]->west != NULL && dungeon[i][j]->west->visited) adj_hide = false;
            }
            // Print cell
            if (adj_hide) {
                cout << hidden << hidden;
            } else {
                if (dungeon[i][j]->contains_user) {
                    cout << player_icon;
                } else {
                    cout << ' ';
                }
                if (dungeon[i][j]->enemies.size()) {
                    cout << enemy_icon;
                } else {
                    cout << ' ';
                }
            }
            // Print vertical wall/gap
            if (i < WIDTH-1) {
                if (hide && !dungeon[i+1][j]->visited) {
                    cout << hidden;
                } else if (dungeon[i][j]->east == NULL) {
                    cout << vert;
                } else {
                    cout << ' ';
                }
            }
        }
        cout << vert << endl;
        // Print horizontal wall/gap
        if (j < HEIGHT-1) {
            cout << left_cross;
            for (i = 0; i < WIDTH; i++) {
                if (!cheat && !(dungeon[i][j]->visited || dungeon[i][j+1]->visited)) {
                    cout << hidden << hidden;
                } else if (dungeon[i][j]->south == NULL) {
                    cout << hori << hori;
                } else {
                    cout << ' ' << ' ';
                }
                if (i < WIDTH-1) {
                    cout << cross;
                }
            }
            cout << right_cross << endl;
        }
    }

    // Print bottom wall
    cout << bottom_left;
    for (i = 0; i < WIDTH-1; i++) cout << hori << hori << bottom_cross;
    cout << hori << hori << bottom_right << endl;
}


void Game::saveGame() {
    ofstream profile(SAVEFILE);
        profile << *this;
    profile.close();
}


void Game::gameLoop() {
    char key;
    bool valid, did_fight;
    bool cont = true;
    string clear_line = "                              ";
    COORD start = {0, 1};
    COORD controls1 = {0, 22};
    COORD controls2 = {0, 23};
    COORD log = {33, 0};
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    Room* new_room;

    cout << endl << "You find yourself in the north-west corner of a dungeon" << endl;
    cout << "there is an exit in the south-east corner" << endl;
    cout << "but there are monsters and possibly traps" << endl;
    cout << "blocking your path, can you fight your way to the exit?" << endl;
    cout << "Press any key to start...";
    _getch();

    // Disable cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    system("cls");
    cout << player_icon << " = Player, " << enemy_icon << " = Enemy";
    SetConsoleCursorPosition(out, start);
    printMap();

    while (user.health > 0 && cont) {
        SetConsoleCursorPosition(out, controls1);
        if (curr_room->north != NULL) cout << "[w]North ";
        if (curr_room->west != NULL) cout << "[a]West ";
        if (curr_room->south != NULL) cout << "[s]South ";
        if (curr_room->east != NULL) cout << "[d]East ";
        SetConsoleCursorPosition(out, controls2);
        cout << "[c]Cheat [f]Save [q]Quit";

        new_room = NULL;
        do {
            key = _getch();
            switch (key) {
                case 'w':
                    new_room = curr_room->north;
                    break;
                case 'a':
                    new_room = curr_room->west;
                    break;
                case 's':
                    new_room = curr_room->south;
                    break;
                case 'd':
                    new_room = curr_room->east;
                    break;
                case 'c':
                    cheat = !cheat;
                    SetConsoleCursorPosition(out, log);
                    if (cheat) {
                        cout << "Cheat mode enabled.";
                    } else {
                        cout << "Cheat mode disabled.";
                    }
                    ++log.Y;
                    SetConsoleCursorPosition(out, start);
                    printMap();
                    break;
                case 'f':
                    saveGame();
                    SetConsoleCursorPosition(out, log);
                    cout << "Game saved.";
                    ++log.Y;
                    break;
                case 'q':
                    // Clear lines containing the controls
                    SetConsoleCursorPosition(out, controls1);
                    cout << clear_line;
                    SetConsoleCursorPosition(out, controls2);
                    cout << clear_line;
                    SetConsoleCursorPosition(out, controls1);
                    cout << "Quitting...";
                    SetConsoleCursorPosition(out, controls2);
                    return;
            }
            valid = (new_room != NULL);
        } while (!valid);

        curr_room->contains_user = false;
        curr_room = new_room;
        curr_room->visited = true;
        curr_room->contains_user = true;

        SetConsoleCursorPosition(out, start);
        printMap();

        did_fight = curr_room->fight(&user, &log);
        curr_room->doEvents(&user, &log);

        if (did_fight) {
            SetConsoleCursorPosition(out, log);
            cout << "Press any key to continue...";
            _getch();
            ++log.Y;
        }

        SetConsoleCursorPosition(out, start);
        printMap();

        // Clear lines containing the controls
        SetConsoleCursorPosition(out, controls1);
        cout << clear_line;
        SetConsoleCursorPosition(out, controls2);
        cout << clear_line;
        SetConsoleCursorPosition(out, controls1);

        if (user.health > 0 && curr_room == dungeon[WIDTH-1][HEIGHT-1]) {
            cout << "You have reached the end." << endl;
            cout << "You win!" << endl;
            cont = false;
        }
    }
    if (user.health <= 0) cout << "You are dead..." << endl;
}

ostream& operator<<(ostream& os, const Game& game) {
    // Overloads the output operator to allow easy saving of the game
    os << game.user << endl;
    for (vector<Room*> column : game.dungeon) {
        for (Room* room : column) {
            os << *room << endl;
        }
    }
    return os;
}
