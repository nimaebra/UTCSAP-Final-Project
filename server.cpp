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
        const auto& name = req.get_file_value("name");
        // cout << name.content << endl;
        board.add_player(name.content);
        string response = "succesfully joined. ";
        if (board.players_number != players_number) {
            response += "waiting for other players!";
        }
        res.set_content(response, "text/plain");
    });

    srv.Post("/put-wall", [&](const auto& req, auto& res) {
        if (!req.has_param("row") && !req.has_param("col") && !req.has_param("wall-type")) {
            res.set_content("[!] Invalid parameter!", "text/plain");
        }
        int row = atoi(req.get_param_value("row").c_str());
        int col = atoi(req.get_param_value("col").c_str());
        string wall_type = req.get_param_value("wall-type");
        board.place_wall(row, col, wall_type);
    });

    srv.Post("/move", [&](const auto& req, auto& res) {
        if (!req.has_param("direction")) {
            res.set_content("[!] Invalid direction!", "text/plain");
        }
        string dir = req.get_param_value("direction");
        cout << "dir: " << dir << endl;
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
    srv.Post("/game-data", [&](const auto& req, auto& res) {
        string message = "", status = "800";
        if (board.players_index < board.players_number) {
            message = "Wait for other players to join!";
            status = "801";
        }
        json j_res = {
            { "current_player", board.players[board.current_player].name },
            { "board", board.get_board() },
            { "message", message },
            { "status", status }
        };
        res.set_content(j_res.dump(), "application/json");
    });

    srv.listen("localhost", 8080);
}