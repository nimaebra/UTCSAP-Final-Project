#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include "Board.h"

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

Board::Board(int players_num) {
    players_number = players_num;
    players = new Player[players_number];
    current_player = 0;
    players_index = 0;
}

string Board::add_player(std::string name) {
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

string Board::move_player(int player_index, string dir) {
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
    current_player = (current_player + 1) % players_number;
    return "800";
}

string Board::place_wall(int i, int j, string type) {
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
    // TODO: chech if the wall doesn't block a player completely!
    Wall new_wall = {.x = i, .y = j, .type = type};
    walls.push_back(new_wall);
    current_player = (current_player + 1) % players_number;
    return "800";
}

bool Board::is_valid_move(int player_index, string dir) {
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
        return true;
    }
    else if (dir == "s") {
        if (posX == BOTTOM_X) {
            return false;
        }
        for (int i = 0; i < walls.size(); i++) {
            if ((walls[i].x == posX) && (walls[i].y == posY) && (walls[i].type == "h")) {
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
            if ((walls[i].x == posX) && (walls[i].y == posY) && (walls[i].type == "v")) {
                return false;
            }
        }
        return true;
    }
    return false;
}

int Board::get_player_index(string name) {
    for (int i = 0; i < players_number; i++) {
        if (players[i].name == name) {
            return i;
        }
    }
    return -1;
}

string Board::get_board() {
    // cout << " im here: " << players_number << " " << players[0].x << endl;
    // cout << walls[0].x << " " << walls[0].y << endl;
    string board_view = "";
    cout << "   ";
    board_view += "   ";
    for (int i = 0; i <= BOARD_SIZE; i++) {
        cout << i << "   ";
        board_view += to_string(i) + "   ";
    }
    cout << endl;
    board_view += "\n";
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << BOARD_SIZE - i << " ";
        board_view += to_string(BOARD_SIZE - i) + " ";
        if (BOARD_SIZE - i < 10) {
            cout << " ";
            board_view += " ";
        }
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i == 0) {
                cout << "*---";
                board_view += "*---";
            }
            else {
                bool flag = false;
                for (int w = 0; w < walls.size(); w++) {
                    if ((i == BOARD_SIZE - walls[w].x) && (j == walls[w].y - 1) && (walls[w].type == "h")) {
                        cout << "*===";
                        board_view += "*===";
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    cout << "*   ";
                    board_view += "*   ";
                }
            }
        }
        cout << "*" << endl;
        board_view += "*\n";
        cout << "   |";
        board_view += "   |";
        for (int j = 0; j < BOARD_SIZE; j++) {
            bool flag_1 = false, flag_2 = false;
            for (int p = 0; p < players_number; p++) {
                if ((i == BOARD_SIZE - players[p].x) && (j == players[p].y - 1)) {
                    cout << " " << players[p].show_name << " ";
                    board_view += " " + players[p].show_name + " ";
                    flag_1 = true;
                    break;
                }
            }
            for (int w = 0; w < walls.size(); w++) {
                if ((i == BOARD_SIZE - walls[w].x) && (j == walls[w].y - 1) && (walls[w].type == "v")) {
                    if (!flag_1) {
                        cout << "   |";
                        board_view += "   |";
                    }
                    else {
                        cout << "|";
                        board_view += "|";
                    }
                    flag_2 = true;
                    break;
                }
            }
            if (!flag_2 && flag_1 && j < BOARD_SIZE - 1) {
                cout << " ";
                board_view += " ";
            }
            if (!flag_1 && !flag_2) {
                if (j == BOARD_SIZE - 1) {
                    cout << "   ";
                    board_view += "   ";
                } 
                else {
                    if (i == (BOARD_SIZE - 1) / 2 && j == (BOARD_SIZE - 1) / 2) {
                        cout << " 🚩 ";
                    }
                    else {
                        cout << "    ";
                        board_view += "    ";
                    }
                }
            }
        }
        cout << "| " << BOARD_SIZE - i << endl;
        board_view += "| " + to_string(BOARD_SIZE - i) + "\n";
    }
    cout << 0 << "  ";
    board_view += "0  ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        cout << "*---";
        board_view += "*---";
    }
    cout << "*" << endl << "     ";
    board_view += "*\n     ";
    for (int i = 1; i < BOARD_SIZE; i++) {
        cout << i << "   ";
        board_view += to_string(i) + "   ";   
    }
    cout << BOARD_SIZE << endl;
    board_view += to_string(BOARD_SIZE) + "\n";
    // system("clear");
    return board_view;
}
