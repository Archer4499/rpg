#include <iostream>
#include <string>
#include <fstream>

#include "Game.h"

using namespace std;


int main() {
    cout << "Welcome to TextQuest!" << endl;

    Game game = Game();

    game.gameLoop();

    cout << "Thanks for playing!" << endl;

    system("pause");
    return 0;
}
