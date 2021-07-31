#include <iostream>
#include <unistd.h>
#include "Board.cpp"
#include "json.hpp"
#include "colors.h"

using namespace std;
using namespace nlohmann;

const string green("\033[1;32m");

int main(int argc, char *argv[]) {
    cout << FCYN("nimaaa") << endl;
    // string test = "sddssd";
    // cout << BOLD(test) << endl;
    // cout << "ali" << endl;
    // cout << BOLD("ddffdf") << endl;
    // cout << "ali" << endl;
    // return 0;
    // json j = {
    //     {"name", "nima"}
    // };
    // cout << j["ali"] << endl;
    Board board(4);
    // board.view();
    // board.place_wall(5, 5, 'v');
    // board.place_wall(5, 4, 'v');
    // board.place_wall(2, 5, 'v');
    // board.place_wall(10, 9, 'v');
    board.add_player("nima");
    board.add_player("ali");
    board.add_player("mohsen");
    board.add_player("sara");
    cout << board.move_player(0, "d") << endl;
    cout << board.move_player(1, "w") << endl;
    // cout << endl << board.get_board();
    cout << board.get_board() << endl;
    cout << "winner player: " << board.winner_player_index << endl;
    return 0;
}