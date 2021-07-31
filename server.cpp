#include "httplib.h"
#include <iostream>
#include <string>
#include "Board.cpp"
#include "json.hpp"
 
using namespace std;
using namespace httplib;
using namespace nlohmann;

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
            { "status", result },
            { "board_size", to_string(BOARD_SIZE) }
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
        string result = "";
        if (!req.has_param("direction")) {
            result = "808";
        }
        string dir = req.get_param_value("direction");
        if (!req.has_header("name")) {
            result = "808";
        }
        string name = req.get_header_value("name");
        int player_index = board.get_player_index(name);
        if (player_index == -1) {
            result = "811";
        }
        else {
            result = board.move_player(player_index, dir);
        }
        json j_res = {
            { "message", response_codes[result] },
            { "status", result }
        };
        res.set_content(j_res.dump(), "application/json");
    });

    // 801 => wait for other players to join!
    srv.Get("/game-data", [&](const auto& req, auto& res) {
        string message = "", status = "800";
        if (board.players_index < board.players_number) {
            message = "Wait for other players to join...";
            status = "801";
        }
        json j_res = {
            { "current_player", board.players[board.current_player].name },
            { "current_player_name_colored", board.get_colored_name(board.current_player) },
            { "board", board.get_board() },
            { "message", message },
            { "status", status },
            { "player_numbers", board.players_number },
            { "last_action_type", board.last_action_type },
            { "last_action_message", "" }
        };
        int last_player_index = (board.current_player - 1 + board.players_number) % board.players_number;
        if (board.last_action_type == "move") {
            j_res["last_action_message"] = "Player " + board.get_colored_name(last_player_index) + " move " + board.last_action;
        }
        else if (board.last_action_type == "place_wall") {
            string wall_type = (board.walls[board.walls.size() - 1].type == "h") ? "horizontal" : "vertical";
            j_res["last_action_message"] = "Player " + board.get_colored_name(last_player_index) + " place " + wall_type + " wall at (" + to_string(board.walls[board.walls.size() - 1].x) + ", " + to_string(board.walls[board.walls.size() - 1].y) + ")";
        }
        if (board.is_game_finished()) { 
            j_res["is_game_finished"] = true;
            j_res["winner_player_name"] = board.get_colored_name(board.winner_player_index);
        }
        res.set_content(j_res.dump(), "application/json");
    });

    srv.listen("localhost", 8080);
}