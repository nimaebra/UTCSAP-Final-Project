#include <vector>

using namespace std;

const int BOARD_SIZE = 11;

struct Player {
    std::string name, show_name, color;
    int x, y;
};

struct Wall {
    string type;
    int x, y;
};

class Quoridor {
    public:
        Quoridor(int players_num);
        Player* players;
        vector<Wall> walls;
        int players_number, players_index, current_player;
        int winner_player_index;
        string last_action, last_action_type;
        void place_players();
        string get_board();
        string place_wall(int, int, string);
        string move_player(int, string);
        string add_player(string);
        string get_colored_name(int, bool);
        bool is_valid_move(int, string);
        int get_player_index(string);
        bool is_game_finished();

    private:
};

