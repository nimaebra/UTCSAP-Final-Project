#include <iostream>
#include "Board.cpp"
#include "json.hpp"

using namespace std;
using namespace nlohmann;

int main(int argc, char *argv[]) {
    json j;
    j["nima"] = "1\n";
    cout << j.get<std::string>() << endl;
    // Board board(4);
    // board.view();
    // board.place_wall(5, 5, 'v');
    // board.place_wall(5, 4, 'v');
    // board.place_wall(2, 5, 'v');
    // board.place_wall(10, 9, 'v');
    // board.add_player("nima");
    // board.add_player("ali");
    // board.add_player("mohsen");
    // board.add_player("sara");
    // // board.move_player(0, 'a');
    // cout << endl << board.get_board();
    return 0;
}