#include "httplib.h"
#include <iostream>
#include <string>
#include "Board.cpp"
#include "json.hpp"
 
using namespace std;
using namespace httplib;
using namespace nlohmann;

bool is_valid_dir(string dir) {
    return dir == "w" || dir == "d" || dir == "a" || dir == "s";
}

int main(int argc, char *argv[]) {
    ifstream i("response_codes.json");
    json response_codes;
    i >> response_codes;

    Server srv;

    if (argc != 2 || strlen(argv[1]) != 1) {
        cout << "Invalid players number. (chosse between 2, 3, 4)";
        return 0;
    }

    int players_number = int(argv[1][0] - '0');

    if (players_number > 4 || players_number < 2) {
        cout << "Invalid players number. (chosse between 2, 3, 4)";
        return 0;
    }

    Board board(players_number);

    srv.Post("/join", [&](const auto& req, auto& res) {
        string result;
        if (!req.has_param("name")) {
            result = "808";
        }
        string name = req.get_param_value("name");
        result = board.add_player(name);
        string message = response_codes[result];
        if (result == "800") {
            message = "You joined the game successfully!";
        }
        json j_res = {
            { "message", message },
            { "status", result }
        };
        res.set_content(j_res.dump(), "application/json");
    });

    srv.Post("/place-wall", [&](const auto& req, auto& res) {
        string result = "";
        if (!req.has_param("row") && !req.has_param("col") && !req.has_param("wall-type")) {
            result = "808";
        }
        int row = atoi(req.get_param_value("row").c_str());
        int col = atoi(req.get_param_value("col").c_str());
        string wall_type = req.get_param_value("wall-type");
        result = board.place_wall(row, col, wall_type);
        json j_res = {
            { "message", response_codes[result] },
            { "status", result }
        };
        res.set_content(j_res.dump(), "application/json");
    });

    srv.Post("/move", [&](const auto& req, auto& res) {
        if (!req.has_param("direction")) {
            res.set_content("[!] Invalid direction!", "text/plain");
        }
        string dir = req.get_param_value("direction");
        if (!req.has_header("name")) {
            res.set_content("[!] Invalid player name!", "text/plain");
        }
        string name = req.get_header_value("name");
        int player_index = board.get_player_index(name);
        if (board.players_index < board.players_number - 1) {
            res.set_content("[!] wait for other players to join!", "text/plain");
        }
        else if (player_index == -1) {
            res.set_content("[!] Invalid player name!", "text/plain");
        }
        else if (!is_valid_dir(dir)) {
            res.set_content("[!] Invalid direction!", "text/plain");
        }
        else if (board.current_player != player_index) {
            res.set_content("[!] It's not your turn!", "text/plain");
        }
        else {
            board.move_player(player_index, dir);
            res.set_content(board.get_board(), "text/plain");
        }
    });

    // 801 => wait for other players to join!
    srv.Get("/game-data", [&](const auto& req, auto& res) {
        string message = "", status = "800";
        if (board.players_index < board.players_number) {
            message = "Wait for other players to join!";
            status = "801";
        }
        // if (board.check_winner()) { 

        // }
        json j_res = {
            { "current_player", board.players[board.current_player].name },
            { "board", board.get_board() },
            { "message", message },
            { "status", status },
            { "player_numbers", board.players_number },
        };
        res.set_content(j_res.dump(), "application/json");
    });

    srv.listen("localhost", 8080);
}