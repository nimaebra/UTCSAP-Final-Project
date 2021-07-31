#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>

#include "Quoridor.h"
#include "../utils/colors.h"

using namespace std;

const int TOP_X = BOARD_SIZE, BOTTOM_X = 1;
const int RIGHT_Y = BOARD_SIZE, LEFT_Y = 1;

bool is_valid_wall_type(string wall_type) {
    return wall_type == "v"|| wall_type == "h";
}

bool is_valid_row(int row) {
    return row >= 1 && row <= BOARD_SIZE;
}

bool is_valid_col(int col) {
    return col >= 1 && col <= BOARD_SIZE;
}

bool is_valid_dir(string dir) {
    return dir == "w" || dir == "d" || dir == "a" || dir == "s";
}

string get_dir_title(string dir) {
    if (dir == "a") {
        return "left";
    }
    else if (dir == "s") {
        return "down";
    }
    else if (dir == "d") {
        return "right";
    }
    else {
        return "top";
    }
}

string Quoridor::get_colored_name(int player_index, bool first_letter = false) {
    string name = players[player_index].name;
    if (first_letter) {
        name = name.substr(0, 1);
    }
    if (player_index == 0) { 
        return BOLD(FYEL(name));
    }
    else if (player_index == 1) {
        return BOLD(FCYN(name));
    }
    else if (player_index == 2) {
        return BOLD(FMAG(name));
    }
    else {
        return BOLD(FBLU(name));
    }
}

Quoridor::Quoridor(int players_num) {
    players_number = players_num;
    players = new Player[players_number];
    current_player = 0;
    players_index = 0;
    winner_player_index = -1;
    last_action_type = "";
    last_action = "";
}

bool Quoridor::is_game_finished() {
    return winner_player_index != -1;
}

string Quoridor::add_player(string name) {
    if (players_number <= players_index) {
        return "802";
    }
    for (int i = 0; i < players_index; i++) {
        if (players[i].name == name) {
            return "803";
        }
    }
    if (players_index == 0) {
        players[players_index].x = BOARD_SIZE;
        players[players_index].y = 1;
    }
    else if (players_index == 1) {
        players[players_index].x = 1;
        players[players_index].y = BOARD_SIZE;
    }
    else if (players_index == 2) {
        players[players_index].x = 1;
        players[players_index].y = 1;
    }
    else if (players_index == 3) {
        players[players_index].x = BOARD_SIZE;
        players[players_index].y = BOARD_SIZE;
    }
    players[players_index].name = name;
    players[players_index].show_name = name[0];
    players_index++;
    return "800";
}

string Quoridor::move_player(int player_index, string dir) {
    if (is_game_finished()) { 
        return "813";
    }
    if (!is_valid_dir(dir)) {
        return "809";
    }
    if (current_player != player_index) {
        return "810";
    }
    if (players_index < players_number - 1) {
        return "801";
    }
    if (!is_valid_move(player_index, dir)) {
        return "812";
    }
    if (dir == "w") {
        players[player_index].x += 1;
    } 
    else if (dir == "d") {
        players[player_index].y += 1;
    } 
    else if (dir == "s") {
        players[player_index].x -= 1;
    }
    else if (dir == "a") {
        players[player_index].y -= 1;
    }
    if ((players[player_index].x == (BOARD_SIZE + 1) / 2) && 
        (players[player_index].y == (BOARD_SIZE + 1) / 2)) {
        winner_player_index = player_index;
    }
    else {
        last_action_type = "move";
        last_action = get_dir_title(dir);
        current_player = (current_player + 1) % players_number;
    }
    return "800";
}

string Quoridor::place_wall(int i, int j, string type) {
    if (is_game_finished()) { 
        return "813";
    }
    if (!is_valid_row(i)) {
        return "805";
    }
    if (!is_valid_col(j)) {
        return "806";
    }
    if (!is_valid_wall_type(type)) {
        return "807";
    }
    for (int w = 0; w < walls.size(); w++) {
        if (walls[w].x == i && walls[w].y == j && walls[w].type == type) {
            return "804";
        }
    }
    Wall new_wall = {.x = i, .y = j, .type = type};
    walls.push_back(new_wall);
    last_action_type = "place_wall";
    current_player = (current_player + 1) % players_number;
    return "800";
}

bool Quoridor::is_valid_move(int player_index, string dir) {
    int posX = players[player_index].x;
    int posY = players[player_index].y;
    if (dir == "w") {
        if (posX == TOP_X) {
            return false;
        }
        for (int i = 0; i < walls.size(); i++) {
            if ((walls[i].x == posX) && (walls[i].y == posY) && (walls[i].type == "h")) {
                return false;
            }
        }
        for (int i = 0; i < players_number; i++) {
            if ((player_index != i) && (players[i].x == posX + 1) && (players[i].y == posY)) {
                return false;
            }
        }
        return true;
    }
    else if (dir == "d") {
        if (posY == RIGHT_Y) {
            return false;
        }
        for (int i = 0; i < walls.size(); i++) {
            if ((walls[i].x == posX) && (walls[i].y == posY) && (walls[i].type == "v")) {
                return false;
            }
        }
        for (int i = 0; i < players_number; i++) {
            if ((player_index != i) && (players[i].x == posX) && (players[i].y == posY + 1)) {
                return false;
            }
        }
        return true;
    }
    else if (dir == "s") {
        if (posX == BOTTOM_X) {
            return false;
        }
        for (int i = 0; i < walls.size(); i++) {
            if ((walls[i].x == posX - 1) && (walls[i].y == posY) && (walls[i].type == "h")) {
                return false;
            }
        }
        for (int i = 0; i < players_number; i++) {
            if ((player_index != i) && (players[i].x == posX - 1) && (players[i].y == posY)) {
                return false;
            }
        }
        return true;
    }
    else if (dir == "a") {
        if (posY == LEFT_Y) {
            return false;
        }
        for (int i = 0; i < walls.size(); i++) {
            if ((walls[i].x == posX) && (walls[i].y == posY - 1) && (walls[i].type == "v")) {
                return false;
            }
        }
        for (int i = 0; i < players_number; i++) {
            if ((player_index != i) && (players[i].x == posX) && (players[i].y == posY - 1)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

int Quoridor::get_player_index(string name) {
    for (int i = 0; i < players_number; i++) {
        if (players[i].name == name) {
            return i;
        }
    }
    return -1;
}

string Quoridor::get_board() {
    string board_view = "";
    board_view += "   ";
    for (int i = 0; i <= BOARD_SIZE; i++) {
        board_view += to_string(i) + "   ";
    }
    board_view += "\n";
    for (int i = 0; i < BOARD_SIZE; i++) {
        board_view += to_string(BOARD_SIZE - i) + " ";
        if (BOARD_SIZE - i < 10) {
            board_view += " ";
        }
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i == 0) {
                board_view += "*---";
            }
            else {
                bool flag = false;
                for (int w = 0; w < walls.size(); w++) {
                    if ((i == BOARD_SIZE - walls[w].x) && (j == walls[w].y - 1) && (walls[w].type == "h")) {
                        board_view += "*===";
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    board_view += "*   ";
                }
            }
        }
        board_view += "*\n";
        board_view += "   |";
        for (int j = 0; j < BOARD_SIZE; j++) {
            bool flag_1 = false, flag_2 = false;
            for (int p = 0; p < players_number; p++) {
                if ((i == BOARD_SIZE - players[p].x) && (j == players[p].y - 1)) {
                    board_view += ' ' + get_colored_name(p, true) + ' ';
                    flag_1 = true;
                    break;
                }
            }
            for (int w = 0; w < walls.size(); w++) {
                if ((i == BOARD_SIZE - walls[w].x) && (j == walls[w].y - 1) && (walls[w].type == "v")) {
                    if (!flag_1) {
                        board_view += "   |";
                    }
                    else {
                        board_view += "|";
                    }
                    flag_2 = true;
                    break;
                }
            }
            if (!flag_2 && flag_1 && j < BOARD_SIZE - 1) {
                board_view += " ";
            }
            if (!flag_1 && !flag_2) {
                if (j == BOARD_SIZE - 1) {
                    board_view += "   ";
                } 
                else {
                    if (i == (BOARD_SIZE - 1) / 2 && j == (BOARD_SIZE - 1) / 2) {
                        board_view += " 🚩 ";
                    }
                    else {
                        board_view += "    ";
                    }
                }
            }
        }
        board_view += "| " + to_string(BOARD_SIZE - i) + "\n";
    }
    board_view += "0  ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        board_view += "*---";
    }
    board_view += "*\n     ";
    for (int i = 1; i < BOARD_SIZE; i++) {
        board_view += to_string(i) + "   ";   
    }
    board_view += to_string(BOARD_SIZE) + "\n";
    return board_view;
}
