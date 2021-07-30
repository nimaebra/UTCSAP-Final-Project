#include <vector>

using namespace std;

const int BOARD_SIZE = 9;

struct Player {
    std::string name, show_name;
    int x, y;
};

struct Wall {
    string type;
    int x, y;
};

class Board {
    public:
        Board(int players_num);
        Player* players;
        vector<Wall> walls;
        int players_number, players_index, current_player;
        void place_players();
        string get_board();
        string place_wall(int, int, string);
        string move_player(int, string);
        bool is_valid_move(int, string);
        string add_player(string name);
        int get_player_index(string);

    private:
};

