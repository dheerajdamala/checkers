#include "checkers.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0)); 
    int gm;

    cout << "Choose Game Mode:\n";
    cout << "1. PvP\n";
    cout << "2. PvC\n";
    cout << "3. CvC\n";
    cout << "Enter your choice: ";
    cin >> gm;

    try {
        if (gm < 1 || gm > 3) {
            throw invalid_argument("Invalid game mode. Exiting.");
        }

        Game game(gm);
        game.play();
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

