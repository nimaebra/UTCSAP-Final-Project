#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>

#include "json.hpp"
#include "httplib.h"
#include "Board.cpp"

using namespace std;
using namespace httplib;
using namespace nlohmann;

const unsigned int microsecond = 1000000;

bool is_valid_dir(string dir) {
    return dir == "w" || dir == "d" || dir == "a" || dir == "s";
}

int main() {
    string name;
    
    // TODO: read this from argv!
    Client cli("localhost", 8080);

    cout << "Enter yout name: ";
    cin >> name;

    cli.set_default_headers({
        { "name", name }
    });

    if (auto res = cli.Post("/join")) {
        if (res->status == 200) {
            cout << "succesfully!" << endl;
        }
    }

    string current_board, last_board;

    while (auto res = cli.Post("/game-data", move_items)) {
        if (res->status == 200) {
            auto p_res = json::parse(res->body);
            // cout << p_res["status"] << ", " << p_res["current_player"] << endl;
            system("clear");
            current_board = p_res["board"].get<std::string>();
            if (current_board != last_board) {
                cout << current_board endl;
                last_board = current_board;
            }
            if (p_res["status"] == "801") {
                cout << p_res["message"] << endl;
            }
            else if (p_res["current_player"] == name) {
                cout << "It's your turn!" << endl;
                int turn = -1, row = -1, col = -1;
                string dir = "", type = "";
                while (turn != 1 && turn != 2) {
                    cout << "[-] Choose your turn:" << endl;
                    cout << "1- Move" << endl;
                    cout << "2- Wall" << endl;
                    cin >> turn;
                }
                if (turn == 1) {
                    while (!is_valid_dir(dir)) {
                        cout << "Enter direction (w, d, s, a): ";
                        cin >> dir;
                    }
                    Params move_params {
                        { "direction", dir }
                    };
                    auto res = cli.Post("/move", move_params);
                    cout << res->body << endl;
                }
                else if (turn == 2) {
                    while (!(row >= 1 && row <= 11)) {
                        cout << "Enter row (1, 2, ..., 11): ";
                        cin >> row;
                    }
                    while (!(col >= 1 && col <= 11)) {
                        cout << "Enter col (1, 2, ..., 11): ";
                        cin >> col;
                    }
                    while (type != "h" && type != "v") {
                        cout << "Enter type of wall (h, v): ";
                        cin >> type;
                    }
                    Params wall_params {
                        { "row", to_string(row) },
                        { "col", to_string(col) },
                        { "wall-type", type }
                    };
                    auto res = cli.Post("/place-wall", wall_params);
                    cout << res->body << endl;
                }

            }
        }
        usleep(5 * microsecond);
    }

    return 0;
} 

























