#ifndef WORLD_H
#define WORLD_H
#include <math.h>
#include <vector>
#include <queue>
#include <set>
#include "player.h"
#include "public_state.h"

class world{
    protected:
    
        std::vector<player> players;
        //vector of players in the game
        std::queue<int> turn_order;
        // queue representing the turn order of players
        std::vector<int> _deck;
        // vector containing the number of each type of influence remaining in the deck
        public_state _public_state;
        public:
        world(int num_players);
        player get_next_player();
        std::vector<player>& get_players();
        std::vector<int>& get_deck();
        void remove_player(int player_id);
        void lose_influence(int player_id, bool swap);
        public_state get_public_state() const;
        void carry_out_action(int current_player_id, int target_player_id, int action_id);
        void pay(int player_id);
        bool is_not_finished();
        void player_draw_influence(int player_id);
        int draw_from_deck();
        void add_to_deck(int id);

        // we want a system which lets the player choose which removes the influence relating to the action just made, 
        // then draws a new card from the deck and adds that influence to the player.
    };
#endif // WORLD_H
