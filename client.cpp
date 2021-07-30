#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>

#include "json.hpp"
#include "httplib.h"

using namespace std;
using namespace httplib;
using namespace nlohmann;

const unsigned int microsecond = 1000000;

const string green("\033[1;32m");
const string red("\033[0;31m");
const string yellow("\033[1;33m");
const string cyan("\033[0;36m");
const string magenta("\033[0;35m");
const string reset("\033[0m");

bool is_valid_dir(string dir) {
    return dir == "w" || dir == "d" || dir == "a" || dir == "s";
}

int main() {
    string name;
    
    // TODO: read this from argv!
    Client cli("localhost", 8080);

    cout << "Enter your name: ";
    cin >> name;

    while (true) {
        Params new_params {
            { "name", name },
        };
        auto res = cli.Post("/join", new_params);
        cout << name << endl;
        if (res->status == 200) {
            auto p_res = json::parse(res->body);
            cout << p_res["message"].get<string>() << endl;
            if (p_res["status"] == "800") {
                cli.set_default_headers({
                    { "name", name }
                });
                break;
            }
            else {
                cout << "Enter your name: ";
                cin >> name;
                // cout << "nameee: " << name << endl;
            }
        }
    }

    string current_board, last_board;
    string current_message, last_message;

    while (auto res = cli.Get("/game-data")) {
        if (res->status == 200) {
            auto p_res = json::parse(res->body);
            // cout << p_res["status"] << ", " << p_res["current_player"] << endl;
            current_board = p_res["board"].get<string>();
            // if (current_board != last_board) {
                system("clear");
                cout << current_board << endl;
                // last_board = current_board;
            // }
            if (p_res["status"] == "801") {
                current_message = p_res["message"].get<string>();
                if (current_message != last_message) {
                    cout << current_message << endl;
                    last_message = current_message;
                }
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
                    auto p_res = json::parse(res->body);
                    cout << p_res["message"].get<string>() << endl;
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
                    auto p_res = json::parse(res->body);
                    cout << p_res["message"].get<string>() << endl;
                }
            }
        }
        usleep(5 * microsecond);
    }

    return 0;
} 
