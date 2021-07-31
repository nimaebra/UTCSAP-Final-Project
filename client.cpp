#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>

#include "json.hpp"
#include "httplib.h"
#include "colors.h"

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
    int board_size;
    
    // TODO: read this from argv!
    Client cli("localhost", 8080);

    system("clear");

    cout << BOLD("=> Enter your name: ");
    cin >> name;

    while (true) {
        Params new_params {
            { "name", name },
        };
        auto res = cli.Post("/join", new_params);
        if (res->status == 200) {
            auto p_res = json::parse(res->body);
            if (p_res["status"] == "800") {
                cout << FGRN(p_res["message"].get<string>()) << endl;
                cli.set_default_headers({
                    { "name", name }
                });
                board_size = atoi(p_res["board_size"].get<string>().c_str());
                break;
            }
            else {
                cout << FRED(p_res["message"].get<string>()) << endl;
                cout << BOLD("=> Enter your name: ");
                cin >> name;
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
            if (p_res["is_game_finished"] == true) { 
                cout << BOLD("Game finished!") << endl << endl;
                cout << FGRN("Player ") << p_res["winner_player_name"].get<string>() << FGRN(" win the game.") << endl;
                break;
            }
            if (p_res["last_action_type"] == "move" || p_res["last_action_type"] == "place_wall") {
                cout << p_res["last_action_message"].get<string>() << endl << endl; 
            }
            if (p_res["status"] == "801") {
                current_message = p_res["message"].get<string>();
                // if (current_message != last_message) {
                    cout << FRED(current_message) << endl;
                    // last_message = current_message;
                // }
            }
            else if (p_res["current_player"] == name) {
                cout << "[#] It's your turn!" << endl << endl;
                string turn = "-1";
                while (turn != "1" && turn != "2") {
                    cout << BOLD("=> Choose your turn (1 or 2):") << endl;
                    cout << "1- Move" << endl;
                    cout << "2- Wall" << endl;
                    cin >> turn;
                }
                if (turn == "1") {
                    string dir = "";
                    while (!is_valid_dir(dir)) {
                        cout << BOLD("=> Enter direction (w, d, s, a): ");
                        cin >> dir;
                    }
                    Params move_params {
                        { "direction", dir }
                    };
                    auto res = cli.Post("/move", move_params);
                    auto p_res = json::parse(res->body);
                    if (p_res["status"] == "800") {
                        cout << FGRN(p_res["message"].get<string>()) << endl;
                    }
                    else {
                        cout << FRED(p_res["message"].get<string>()) << endl;
                    }
                }
                else if (turn == "2") {
                    string i_row, i_col;
                    int row, col;
                    string type = "";
                    while (true) {
                        cout << BOLD("=> Enter row (1, 2, ..., " + to_string(board_size) + "): ");
                        cin >> i_row;
                        row = atoi(i_row.c_str());
                        if (row >= 1 && row <= board_size) {
                            break;
                        }
                    }
                    while (true) {
                        cout << BOLD("=> Enter col (1, 2, ..., " + to_string(board_size) + "): ");
                        cin >> i_col;
                        col = atoi(i_col.c_str());
                        if (col >= 1 && col <= board_size) {
                            break;
                        }
                    }
                    while (type != "h" && type != "v") {
                        cout << BOLD("=> Enter type of wall (h, v): ");
                        cin >> type;
                    }
                    Params wall_params {
                        { "row", to_string(row) },
                        { "col", to_string(col) },
                        { "wall-type", type }
                    };
                    auto res = cli.Post("/place-wall", wall_params);
                    auto p_res = json::parse(res->body);
                    if (p_res["status"] == "800") {
                        cout << FGRN(p_res["message"].get<string>()) << endl;
                    }
                    else {
                        cout << FRED(p_res["message"].get<string>()) << endl;
                    }
                }
            }
            else {
                cout << "[#] It's " + p_res["current_player_name_colored"].get<string>() + "'s turn!" << endl;
            }
        }
        usleep(5 * microsecond);
    }
    return 0;
} 
