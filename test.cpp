#include <iostream>

#include "src/Quoridor.cpp"
#include "utils/json.hpp"

using namespace std;
using namespace nlohmann;

int main(int argc, char *argv[]) {
    Quoridor quoridor(4);
    quoridor.add_player("nima");
    quoridor.add_player("ali");
    quoridor.add_player("mohsen");
    quoridor.add_player("sara");
    cout << quoridor.move_player(0, "d") << endl;
    cout << quoridor.move_player(1, "w") << endl;
    cout << quoridor.get_board() << endl;
    cout << "winner player: " << quoridor.winner_player_index << endl;
    return 0;
}