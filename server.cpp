#include <iostream>
#include <string>

#include "utils/httplib.h"
#include "src/Quoridor.cpp"
#include "utils/json.hpp"
 
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

    Quoridor quoridor(players_number);

    srv.Post("/join", [&](const auto& req, auto& res) {
        string result;
        if (!req.has_param("name")) {
            result = "808";
        }
        string name = req.get_param_value("name");
        result = quoridor.add_player(name);
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
        result = quoridor.place_wall(row, col, wall_type);
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
        int player_index = quoridor.get_player_index(name);
        if (player_index == -1) {
            result = "811";
        }
        else {
            result = quoridor.move_player(player_index, dir);
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
        if (quoridor.players_index < quoridor.players_number) {
            message = "Wait for other players to join...";
            status = "801";
        }
        json j_res = {
            { "current_player", quoridor.players[quoridor.current_player].name },
            { "current_player_name_colored", quoridor.get_colored_name(quoridor.current_player) },
            { "board", quoridor.get_board() },
            { "message", message },
            { "status", status },
            { "player_numbers", quoridor.players_number },
            { "last_action_type", quoridor.last_action_type },
            { "last_action_message", "" }
        };
        int last_player_index = (quoridor.current_player - 1 + quoridor.players_number) % quoridor.players_number;
        if (quoridor.last_action_type == "move") {
            j_res["last_action_message"] = "Player " + quoridor.get_colored_name(last_player_index) + " move " + quoridor.last_action;
        }
        else if (quoridor.last_action_type == "place_wall") {
            string wall_type = (quoridor.walls[quoridor.walls.size() - 1].type == "h") ? "horizontal" : "vertical";
            j_res["last_action_message"] = "Player " + quoridor.get_colored_name(last_player_index) + " place " + wall_type + " wall at (" + to_string(quoridor.walls[quoridor.walls.size() - 1].x) + ", " + to_string(quoridor.walls[quoridor.walls.size() - 1].y) + ")";
        }
        if (quoridor.is_game_finished()) { 
            j_res["is_game_finished"] = true;
            j_res["winner_player_name"] = quoridor.get_colored_name(quoridor.winner_player_index);
        }
        res.set_content(j_res.dump(), "application/json");
    });

    srv.listen("localhost", 8080);
}